# ESP32+Xiaomi Flower Care BLE통신
![(Xiaomi)유튜브첫장](https://user-images.githubusercontent.com/37902752/152269554-bc0dd611-9414-4271-b45a-37518d1041bb.png)

## 준비물
---
1. **ESP32 DEVKIT V1  1개**
2. **Xiaomi Flower Care**
3. **nRF Connect (스마트폰 앱)**
4. **Flower Care (스마트폰 앱)**

## 동작원리
---
1. **Flower Care( server )가 자신의 정보( UUID, Mac Adress )를 계속해서 advertising 한다.**
2. **ESP32( client )가 필요한 UUID, Mac Adress를 발견하면 연결을 시도한다.**
3. **연결점에서 Flower Care는 advertising을 중지하고 ESP32와 연결한다.**
4. **Flower Care에 내장된 센서들의 real-time data들을 ESP32가 Tx로 수신한다.**
5. **일정 주기마다 데이터를 수신.출력하고, Deep-Sleep( 저전력 )모드에 들어간다.**


## 관련링크
---
* Flower Care( real-time data ) : <https://github.com/vrachieru/xiaomi-flower-care-api>

