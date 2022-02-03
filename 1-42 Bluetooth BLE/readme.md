# ESP32 BLE통신
![(ESP32 2개)유튜브-첫장-001](https://user-images.githubusercontent.com/37902752/152265721-9fe7b323-81aa-40e5-b0b2-f75dca7434a8.png)

## 준비물
---
1. ESP32 DEVKIT V1  2개

## 동작원리
---
1. Server ESP32가 주변에 자신의 정보(UUID)를 계속해서 advertising을 한다.
2. Client ESP32가 필요한 UUID를 발견하면 연결을 시도한다.
3. 연결점에서 Server ESP32는 advertising을 중지하고 Client와 연결한다.
4. Server, Client가 접속을 유지하며 Tx, Rx로 데이터를 송수신 한다.



