/*
 BLE 데이터 수신 및 4채널 릴레이 작동보드
 LED와 릴레이가 배선된 보드에 업로드 해주세요
 */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define RELAY1_PIN 33
#define RELAY2_PIN 32
#define RELAY3_PIN 27
#define RELAY4_PIN 26

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
uint32_t value = 0;
unsigned int counter1, counter2, counter3, counter4 = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received BLE Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
          if(rxValue=="relay1"){
            if(counter1 == 0){
            digitalWrite(RELAY1_PIN,1);
            counter1++;
            }
            else{
            digitalWrite(RELAY1_PIN,0);
            counter1=0;
            }
          }
          else if(rxValue=="relay2"){
            if(counter2 == 0){
            digitalWrite(RELAY2_PIN,1);
            counter2++;
            }
            else{
            digitalWrite(RELAY2_PIN,0);
            counter2=0;
            }
          }
          else if(rxValue=="relay3"){
            if(counter3 == 0){
            digitalWrite(RELAY3_PIN,1);
            counter3++;
            }
            else{
            digitalWrite(RELAY3_PIN,0);
            counter3=0;
            }
          }
          else if(rxValue=="relay4"){
            if(counter4 == 0){
            digitalWrite(RELAY4_PIN,1);
            counter4++;
            }
            else{
            digitalWrite(RELAY4_PIN,0);
            counter4=0;
            }
          }
        Serial.println("");
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(RELAY1_PIN, OUTPUT);  //릴레이 출력핀 세팅
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN,0);   //전체 off
  digitalWrite(RELAY2_PIN,0);
  digitalWrite(RELAY3_PIN,0);
  digitalWrite(RELAY4_PIN,0);
  delay(500);
  // Create the BLE Device
  BLEDevice::init("LightTalk");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
//    if (deviceConnected) {
//        String inputString = "test";
//        //pTxCharacteristic->setValue(&txValue, 1);
//        pTxCharacteristic->setValue(inputString.c_str());
//        pTxCharacteristic->notify();
//        //txValue++;
//		delay(10); // bluetooth stack will go into congestion, if too many packets are sent	 
//	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    delay(500);
}
