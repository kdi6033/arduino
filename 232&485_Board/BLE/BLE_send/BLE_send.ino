/*
 BLE 데이터 송신용 (RS-232)
 BLE 데이터 송신 후 유선으로 RS-232, RS-485 신호를 수신받음
 */
#include "BLEDevice.h"

unsigned long previousMillis = 0;     
const long interval = 3000; 
unsigned int counter=0;
String inString;

static BLEUUID    serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID rxUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID txUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

//연결 및 장치 검색 상태 변수
static boolean doConnect = false;
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
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);  // data
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }
  void onDisconnect(BLEClient* pclient) {
    connected = false;
    doScan = true;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {  
    Serial.print("Forming a connection to ");   
    
    //myDevice는 연결할 정보가 담긴 변수->여기서 정보를 넣어줌
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);
    Serial.println(" - Connected to server");
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    
    //UUID를 못가져올 시 연결해제->종료
    if (pRemoteService == nullptr) {        
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;      
    }
    
    // Tx characteristic 받아오기
    pTxCharacteristic = pRemoteService->getCharacteristic(txUUID);
    if (pTxCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(txUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    
    // Tx characteristic이 notify 지원하는지
    if(pTxCharacteristic->canNotify()) {    
      //데이터를 받을 콜백함수 등록
      pTxCharacteristic->registerForNotify(notifyCallback);
    }

    // Rx charateristic 받아오기
    pRxCharacteristic = pRemoteService->getCharacteristic(rxUUID);
    if (pRxCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(rxUUID.toString().c_str());
      pClient->disconnect();
      return false;
    } 
 
    // Rx characteristic이 write지원하는지
    if(!pRxCharacteristic->canWrite()) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(rxUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    
    // 여기까지 Client,Rx,Tx 연결완료
    // connectToServer()함수 마무리
    connected = true;
    return true;
}

// 주변 장치 검색함수 ( 서버는 수시로 advertising함 )
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");    
    Serial.print(advertisedDevice.haveServiceUUID());
    Serial.print(" , ");
    Serial.println(advertisedDevice.toString().c_str());
    // // advertising정보에 같은 같은 Service UUID가 있는지 확인
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      Serial.print("A device to be connected has been found.");
      BLEDevice::getScan()->stop();
      //해당 장치 정보를 myDevice에 저장
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = false;
    }
  }
};

void serial1Event() { // RS232 read
  if(Serial1.available()) {
    char ch1 = Serial1.read();
    Serial.write(ch1);
  }
}

void serial2Event() { // RS485 read
  if(Serial2.available()) {
    char ch2 = Serial2.read();
    Serial.write(ch2);
  }
}

void tick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendData();
  }
}

void sendData(){   
    if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      }
      doConnect = false;
      }

  if (connected) {
    if(counter==0){
      inString = "232"; 
      counter++; 
    }
    else if(counter==1){
      inString = "485"; 
      counter=0;     
    }
     pRxCharacteristic->writeValue(inString.c_str(), inString.length()); 
     }
  else if(doScan){
    BLEDevice::getScan()->start(0);
    }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 26, 27); //RS-232
  Serial2.begin(115200, SERIAL_8N1, 16, 17); //RS-485

  Serial.println("Starting Arduino BLE Client application...");
  
  //BLE 클래스 초기화
  BLEDevice::init("TheOneSystem");
  //스캔 클래스 생성
  BLEScan* pBLEScan = BLEDevice::getScan();
  //장치 검색되면 호출할 콜백함수 등록
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->start(0);
} 

void loop(){
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      }
      doConnect = false;
  }
  tick();
  serial1Event();
  serial2Event();
}
