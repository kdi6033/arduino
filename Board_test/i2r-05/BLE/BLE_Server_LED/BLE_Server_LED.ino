#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Output pin numbers
const int outputPins[21] = {7,6,5,4,3,2,1,8,9,10,11,12,13,14,15,16,21,38,47,18,17};

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
//uint32_t count = 0;  // count 변수 선언

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// count 값을 받아서 핀을 ON/OFF하는 함수
void controlPins(int count) {
  for (int i = 0; i < 21; i++) {
    digitalWrite(outputPins[i], HIGH);
    delay(10);  
    digitalWrite(outputPins[i], LOW);
    delay(10); 
  }
}

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String rxValue = String((char*)pCharacteristic->getValue().c_str()); // 변환
        rxValue.trim();  // 개행 문자 및 공백 제거

        if (rxValue.length() > 0) {
            Serial.print("Received Value: ");
            Serial.println(rxValue);

            // 숫자 변환
            int count = rxValue.toInt();
            controlPins(count); // 핀 제어 함수 호출

            // count 증가 후 응답 메시지 생성
            /*
            count++;
            String response = "Server: " + rxValue + " " + String(count);
            // 클라이언트에게 응답 전송
            pTxCharacteristic->setValue(response.c_str());
            pTxCharacteristic->notify();
            Serial.println("Sent Response: " + response);
            */
        }
    }
};

void setup() {
    Serial.begin(115200);
    // 모든 핀을 출력으로 설정
    for (int i = 0; i < 21; i++) {
        pinMode(outputPins[i], OUTPUT);
        digitalWrite(outputPins[i], LOW); // 초기 상태 OFF
    }
    delay(100);
    controlPins(0);

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

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting for a client connection to notify...");
}

void loop() {
    // 연결된 경우
    if (deviceConnected) {
        // 필요할 경우 추가 기능 구현 가능
    }

    // 연결이 끊어진 경우 재광고 시작
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);  // Bluetooth 스택을 준비할 시간 제공
        pServer->startAdvertising();
        Serial.println("Restarting advertising...");
        oldDeviceConnected = deviceConnected;
    }

    // 새로운 연결이 이루어진 경우
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }

    delay(500);
}
