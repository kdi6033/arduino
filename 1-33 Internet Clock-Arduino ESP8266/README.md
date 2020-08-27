## 인터넷 시계 만들기

인터넷에 연결된 아두이노로 현재 시간을 가져와 표시한다.
아두이노 OTA 메뉴얼 : https://github.com/kdi6033/arduino/tree/master/1-33%20Internet%20Clock-Arduino%20ESP8266

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

