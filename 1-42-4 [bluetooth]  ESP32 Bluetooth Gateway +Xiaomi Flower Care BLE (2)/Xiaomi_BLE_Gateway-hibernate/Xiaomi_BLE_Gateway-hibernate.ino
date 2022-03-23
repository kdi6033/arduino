//참조사이트
//https://github.com/SusanneThroner/FlowerCareESP32/blob/master/flowerCareESP32/flowerCareESP32.ino
// 센서 프로토콜 설명  https://github.com/ChrisScheffler/miflora/wiki/The-Basics
#include <ArduinoJson.h>

char* FLORA_DEVICES[] = { "C4:7C:8D:6D:E8:02", };//첫번째
#define SLEEP_DURATION 5
#define RETRY 3
#include "BLEDevice.h"
#include "config.h"
RTC_DATA_ATTR int counter = 0;
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

BLEClient* floraClient;
char* deviceMacAddress;
BLERemoteService* floraService;
BLERemoteCharacteristic* floraCharacteristic;

void hibernate() {
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ll);
  Serial.println("Going to sleep now.");
  delay(100);
  esp_deep_sleep_start();
}

bool readXaiomi01() {
  Serial.print(counter++); Serial.print("   ");
  BLEDevice::init("");
  deviceMacAddress = FLORA_DEVICES[0];
  sMac=String(deviceMacAddress);
  sMac.replace(":","");
  BLEAddress floraAddress(deviceMacAddress);
  BLEClient* floraClient = BLEDevice::createClient();
  floraClient->connect(floraAddress);

  // connect data service
  floraService = floraClient->getService(serviceUUID);

  floraCharacteristic = floraService->getCharacteristic(uuid_write_mode);
  // Real-time data read request
  uint8_t buf[2] = {0xA0, 0x1F};
  floraCharacteristic->writeValue(buf, 2, true);
 
  floraCharacteristic = floraService->getCharacteristic(uuid_sensor_data);
  std::string value; 
  value = floraCharacteristic->readValue();
  const char *val = value.c_str();
  Serial.print("Hex: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((int)val[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
  uint16_t temperature = (val[0] + val[1]*256) / ((float)10.0);
  uint8_t moisture = val[7];
  uint16_t light = val[3] + val[4] *256;
  int conductivity = val[8] + val[9] * 256;

  floraCharacteristic = floraService->getCharacteristic(uuid_version_battery);
  std::string value2;
  value2 = floraCharacteristic->readValue();
  const char *val2 = value2.c_str();
  int battery = val2[0];

  StaticJsonDocument<200> doc;
  doc["mac"] = sMac;
  doc["temperature"] = temperature;
  doc["moisture"] = moisture; 
  doc["light"] = light; 
  doc["conductivity"] = conductivity;
  doc["battery"] = battery;
  serializeJson(doc, mqttBuffer);
  Serial.println(mqttBuffer);

  floraClient->disconnect();
  // go to sleep now
  hibernate();
}

void setup() {
  // all action is done when device is woken up
  Serial.begin(115200);
  readXaiomi01();
}

void loop() {
  delay(5000);
}
