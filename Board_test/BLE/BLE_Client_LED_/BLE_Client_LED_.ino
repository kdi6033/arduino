#include "BLEDevice.h"

static BLEUUID serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID rxUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID txUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

// 연결 및 장치 검색 상태 변수
static boolean isConnected = false;
static boolean connected = false;
static boolean doScan = false;

static BLERemoteCharacteristic* pRxCharacteristic;
static BLERemoteCharacteristic* pTxCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
    BLERemoteCharacteristic* pBLERemoteCharacteristic,
    uint8_t* pData,
    size_t length,
    bool isNotify) {

    String receivedData = "";
    for (size_t i = 0; i < length; i++) {
        if (pData[i] != '\r' && pData[i] != '\n') { // 개행 문자 제거
            receivedData += (char)pData[i];
        }
    }
    Serial.println("Received: " + receivedData);  // 수신된 데이터 출력
}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {}

    void onDisconnect(BLEClient* pclient) {
        connected = false;
        doScan = true;
        Serial.println("Disconnected from Server");
    }
};

bool connectToServer() {
    Serial.print("Connecting to: ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient* pClient = BLEDevice::createClient();
    Serial.println(" - Created BLE Client");
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);
    Serial.println(" - Connected to Server");

    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.println("Failed to find service UUID");
        pClient->disconnect();
        return false;
    }

    // Tx characteristic 설정
    pTxCharacteristic = pRemoteService->getCharacteristic(txUUID);
    if (pTxCharacteristic == nullptr) {
        Serial.println("Failed to find Tx characteristic");
        pClient->disconnect();
        return false;
    }

    if (pTxCharacteristic->canNotify()) {
        pTxCharacteristic->registerForNotify(notifyCallback);
    }

    // Rx characteristic 설정
    pRxCharacteristic = pRemoteService->getCharacteristic(rxUUID);
    if (pRxCharacteristic == nullptr) {
        Serial.println("Failed to find Rx characteristic");
        pClient->disconnect();
        return false;
    }

    if (!pRxCharacteristic->canWrite()) {
        Serial.println("Rx characteristic does not support write");
        pClient->disconnect();
        return false;
    }

    connected = true;
    return true;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.print("Found BLE Device: ");
        Serial.println(advertisedDevice.toString().c_str());

        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
            Serial.println("Found a matching BLE device. Connecting...");
            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            isConnected = true;
            doScan = false;
        }
    }
};

void sendCommand(String command) {
    if (isConnected) {
        if (connectToServer()) {
            Serial.println("Connected to BLE Server.");
        } else {
            Serial.println("Failed to connect to BLE Server.");
        }
        isConnected = false;
    }

    if (connected) {
        command += '\0'; // 문자열 끝을 명확히 하기 위해 NULL 문자 추가
        pRxCharacteristic->writeValue(command.c_str(), command.length() + 1);
        Serial.print("Sent: ");
        Serial.println(command);
    } else if (doScan) {
        BLEDevice::getScan()->start(0);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE Client...");

    BLEDevice::init("TheOneSystem");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->start(0);
}

void loop() {
    if (isConnected) {
        if (connectToServer()) {
            Serial.println("Connected to BLE Server.");
        } else {
            Serial.println("Failed to connect to BLE Server.");
        }
        isConnected = false;
    }

    // 연결되었을 때만 메시지 전송
    if (connected) {
        sendCommand("Turn off");
        delay(1000);
        sendCommand("Turn on");
        delay(1000);
    }
}

