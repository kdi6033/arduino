## SPIFFS 사용하기

SPIFFS doc : https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst
SPIFFS 한글 설명 사이트 : https://postpop.tistory.com/97

SPIFFS는 SPI 연결을 이용하는 FLASH 저장 장치의 파일 시스템을 정의하고 데이터가 저장된 파일의 생성 및 수정 그리고 읽기를 하는 프로그램이다. 
다시 부팅해도 데이터는 지워지지않아 EEPROM을 대체해서 사용하면 편리하다. 

아두이노 프로그램
```
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "i2r";
const char *password = "00000000";

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
//timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, updateInterval); utcOffsetInSeconds=GMT
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600*9, 60000);


void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  Serial.println(timeClient.getFormattedTime());
  /*
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  
  */
  delay(1000);
}
```

