#include <ArduinoJson.h>

char* FLORA_DEVICES[] = { "C4:7C:8D:6D:E8:02", };//첫번째
//char* FLORA_DEVICES[] = { "C4:7C:8D:63:92:E5", };
#define SLEEP_DURATION 5
#define BATTERY_INTERVAL 3
#define RETRY 3
#include "BLEDevice.h"
#include "config.h"

RTC_DATA_ATTR int readCount = 0;
int battery=0;
String sMac;
char mqttBuffer[100];

// device count
static int deviceCount = sizeof FLORA_DEVICES / sizeof FLORA_DEVICES[0];

// the remote service we wish to connect to
static BLEUUID serviceUUID("00001204-0000-1000-8000-00805f9b34fb");

// the characteristic of the remote service we are interested in
static BLEUUID uuid_version_battery("00001a02-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_sensor_data("00001a01-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_write_mode("00001a00-0000-1000-8000-00805f9b34fb");

TaskHandle_t hibernateTaskHandle = NULL; 

BLEClient* getFloraClient(BLEAddress floraAddress) {
  BLEClient* floraClient = BLEDevice::createClient();

  if (!floraClient->connect(floraAddress)) {
    //Serial.println("- Connection failed, skipping");
    return nullptr;
  }

  //Serial.println("- Connection successful");
  return floraClient;
}
//연결됐을 시 서비스 정보 받음
BLERemoteService* getFloraService(BLEClient* floraClient) {
  BLERemoteService* floraService = nullptr;

  try {
    floraService = floraClient->getService(serviceUUID);
  }
  catch (...) {
    // something went wrong
  }
  /*
  if (floraService == nullptr) {
    Serial.println("- Failed to find data service");
  }
  else {
    Serial.println("- Found data service");
  }
  */

  return floraService;
} //여기까지 연결됐을 시 서비스정보 받는 내용

bool forceFloraServiceDataMode(BLERemoteService* floraService) {
  BLERemoteCharacteristic* floraCharacteristic;
  
  // get device mode characteristic, needs to be changed to read data
  //Serial.println("- Force device in data mode");
  floraCharacteristic = nullptr;
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_write_mode);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    Serial.println("-- Failed, skipping device");
    return false;
  }

  // write the magic data
  uint8_t buf[2] = {0xA0, 0x1A};
  floraCharacteristic->writeValue(buf, 2, true);

  delay(500);
  return true;
}

bool readFloraDataCharacteristic(BLERemoteService* floraService, String baseTopic) {
  BLERemoteCharacteristic* floraCharacteristic = nullptr;

  // get the main device data characteristic
  //Serial.println("- Access characteristic from device");
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_sensor_data);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    Serial.println("-- Failed, skipping device");
    return false;
  }

  // read characteristic value
  //Serial.println("- Read value from characteristic");
  std::string value;
  try{
    value = floraCharacteristic->readValue();
  }
  catch (...) {
    // something went wrong
    Serial.println("-- Failed, skipping device");
    return false;
  }
  const char *val = value.c_str();

  /*
  Serial.print("Hex: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((int)val[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
  */

  uint16_t temperature = (val[0] + val[1]*256) / ((float)10.0);
  uint8_t moisture = val[7];
  uint16_t light = val[3] + val[4] *256;
  int conductivity = val[8] + val[9] * 256;

  StaticJsonDocument<200> doc;
  doc["mac"] = sMac;
  doc["temperature"] = temperature;
  doc["moisture"] = moisture; 
  doc["light"] = light; 
  doc["conductivity"] = conductivity;
  doc["battery"] = battery;
  serializeJson(doc, mqttBuffer);
  Serial.println(mqttBuffer);
  

  if (temperature > 200) {
    Serial.println("-- Unreasonable values received, skip publish");
    return false;
  }
  return true;
}
 //배터리 잔량 표시 관련함수
bool readFloraBatteryCharacteristic(BLERemoteService* floraService, String baseTopic) {
  BLERemoteCharacteristic* floraCharacteristic = nullptr;

  // get the device battery characteristic
  //Serial.println("- Access battery characteristic from device");
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_version_battery);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    //Serial.println("-- Failed, skipping battery level");
    return false;
  }

  // read characteristic value
  //Serial.println("- Read value from characteristic");
  std::string value;
  try{
    value = floraCharacteristic->readValue();
  }
  catch (...) {
    // something went wrong
    //Serial.println("-- Failed, skipping battery level");
    return false;
  }
  const char *val2 = value.c_str();
  battery = val2[0];

  char buffer[64];

  //Serial.print("-- Battery: ");
  //Serial.println(battery);
  snprintf(buffer, 64, "%d", battery);
  //client.publish((baseTopic + "battery").c_str(), buffer);

  return true;
}

bool processFloraService(BLERemoteService* floraService, char* deviceMacAddress, bool readBattery) {
  // set device in data mode
  if (!forceFloraServiceDataMode(floraService)) {
    return false;
  }

  String baseTopic = deviceMacAddress;
  bool dataSuccess = readFloraDataCharacteristic(floraService, baseTopic);

  bool batterySuccess = true;
  if (readBattery) {
    batterySuccess = readFloraBatteryCharacteristic(floraService, baseTopic);
  }
   
  return true;
}

bool processFloraDevice(BLEAddress floraAddress, char* deviceMacAddress, bool getBattery, int tryCount) {
  //Serial.print("Processing Flora device at ");
  //Serial.print(floraAddress.toString().c_str());
  //Serial.print(" (try ");
  //Serial.print(tryCount);
  //Serial.println(")");

  // connect to flora ble server
  BLEClient* floraClient = getFloraClient(floraAddress);
  if (floraClient == nullptr) {
    return false;
  }

  // connect data service
  BLERemoteService* floraService = getFloraService(floraClient);
  if (floraService == nullptr) {
    floraClient->disconnect();
    return false;
  }

  // process devices data
  bool success = processFloraService(floraService, deviceMacAddress, getBattery);

  // disconnect from device
  floraClient->disconnect();

  return success;
}

void readXaiomi() {
   readCount++;
  //Serial.println("Initialize BLE client...");
  BLEDevice::init("");
  BLEDevice::setPower(ESP_PWR_LVL_P7);   
  // check if battery status should be read - based on boot count
  //bool readBattery = ((readCount % BATTERY_INTERVAL) == 0);
  bool readBattery = ((readCount % BATTERY_INTERVAL) == 0);

  // process devices
  for (int i=0; i<deviceCount; i++) {
    int tryCount = 0;
    char* deviceMacAddress = FLORA_DEVICES[i];
    BLEAddress floraAddress(deviceMacAddress);
    sMac=String(deviceMacAddress);
    sMac.replace(":","");
    //Serial.println(sMac);

    while (tryCount < RETRY) {
      tryCount++;
      if (processFloraDevice(floraAddress, deviceMacAddress, readBattery, tryCount)) {
        break;
      }
      delay(1000);
    }
    delay(1500);
  }  
}

void setup() {
  // all action is done when device is woken up
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  readXaiomi();
  delay(5000);
}
