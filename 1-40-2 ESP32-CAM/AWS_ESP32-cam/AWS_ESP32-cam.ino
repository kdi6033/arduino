/////////////////////////////////////////////////////////////////
/*
  AWS IoT | ESP32CAM working as a publisher on MQTT
  Video Tutorial: https://youtu.be/7_3qbou_keg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include "WiFi.h"
#include <ArduinoJson.h>
#include "esp_camera.h"

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define ESP32CAM_PUBLISH_TOPIC   "out"
#define ESP32CAM_SUBSCRIBE_TOPIC   "in"

const int bufferSize = 1024 * 23; // 23552 bytes

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(bufferSize);


void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("\n\n=====================");
  Serial.println("Connecting to Wi-Fi");
  Serial.println("=====================\n\n");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  client.setCleanSession(true);         //연결이 끊길 때 클라이언트에 대한 모든 구독 제거

  client.onMessage(messageHandler);

  Serial.println("\n\n=====================");
  Serial.println("Connecting to AWS IOT");
  Serial.println("=====================\n\n");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    ESP.restart();
    return;
  }

  client.subscribe(ESP32CAM_SUBSCRIBE_TOPIC);

  Serial.println("\n\n=====================");
  Serial.println("AWS IoT Connected!");
  Serial.println("=====================\n\n");
}

int picIn=0;
void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  int macTo = doc["pic"];
  picIn=macTo;
  Serial.println(picIn);

  if(macTo==1){
    grabImage();
  }
  else if(macTo==0){
    return; 
  }
}
void cameraInit(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA; // 640x480
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
    return;
  }
}

void grabImage(){
  camera_fb_t * fb = esp_camera_fb_get();
  if(fb != NULL && fb->format == PIXFORMAT_JPEG && fb->len < bufferSize){ 
    //null이 아니고 형식이 jpeg인 경우에만 데이터를 보낸다, 이미지 크기가 버퍼 크기보다 크면 무시하겠다
    Serial.print("Image Length: ");
    Serial.print(fb->len);
    Serial.print("\t Publish Image: "); //콘솔에 인쇄하여 이미지 크기를 확인
    bool result = client.publish(ESP32CAM_PUBLISH_TOPIC, (const char*)fb->buf, fb->len);
    //(1st topic, 2nd data, 3rd data length)
    Serial.println(result);

    if(!result){
      ESP.restart();   //false반환이 발생하면 클라이언트 연결에 문제가 있을 수 있으므로 시스템 다시 시작
    }
  }
  esp_camera_fb_return(fb);  //메모리에서 프레임 버퍼를 해제하여 기능 종료
  delay(1);
}

void setup() {
  Serial.begin(115200);
  cameraInit();
  connectAWS();
}

void loop() {
  client.loop();
 // if(client.connected()) 
}
