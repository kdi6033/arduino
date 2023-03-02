/*
 BLE 데이터 수신 및 RS-232 RS-485 신호 송신용
 */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
uint32_t value = 0;
unsigned int counter232, counter485 = 0;

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
          if(rxValue=="232"){
            Serial1.println("232test"+String(counter232));
            counter232++;
          }
          else if(rxValue=="485"){
            Serial2.println("485test"+String(counter485));
            counter485++;
          }
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 26, 27); //RS-232
  Serial2.begin(115200, SERIAL_8N1, 16, 17); //RS-485


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
    delay(1000);
}
