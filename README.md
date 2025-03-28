
# ESP32 WROOM + ILI9341 + LVGL + 터치스크린 + SD 카드 연결 예제

이 프로젝트는 **ESP32-S3 보드와 ILI9341 2.4"/2.8"/3.2" SPI 디스플레이**를 연결하여  
**LVGL로 "Hello, World!" 텍스트를 화면 중앙에 출력**하는 예제입니다.

유튜브 참고 영상: [https://www.youtube.com/watch?v=NvBblQnWhsQ](https://www.youtube.com/watch?v=NvBblQnWhsQ)

## 📌 사용 부품

- ESP32-S3 DevKit 보드 (USB-C 포트 있는 보드)
- ILI9341 SPI 디스플레이 (2.4인치 또는 2.8인치, 320x240 해상도)
- 점퍼 케이블

---

### 📺 TFT 디스플레이 연결 (ILI9341)

| 디스플레이 핀 | ESP32 핀 | 설명 |
|---------------|----------|------|
| TFT_VCC       | VIN 또는 3V3 | 전원 공급 |
| TFT_GND       | GND      | 그라운드 |
| TFT_CS        | GPIO15   | 칩 선택 (Chip Select) |
| TFT_RST       | GPIO4    | 디스플레이 리셋 핀 |
| TFT_DC        | GPIO2    | 데이터/명령 전환 핀 |
| TFT_MOSI      | GPIO23   | SPI 데이터 전송 |
| TFT_SCK       | GPIO18   | SPI 클럭 |
| TFT_LED       | 3.3V     | 백라이트 전원 |
| TFT_MISO      | GPIO19   | SPI 데이터 수신 (필요 시) |

---

### 🖐️ 터치스크린 연결 (XPT2046 정압식)

> 디스플레이와 SPI 핀 공유됨

| 터치 핀 | ESP32 핀 | 설명 |
|---------|----------|------|
| T_CLK   | GPIO18   | SPI 클럭 (공유) |
| T_CS    | GPIO21   | 터치 칩 선택 |
| T_DIN   | GPIO23   | SPI 데이터 입력 (공유) |
| T_DO    | GPIO19   | SPI 데이터 출력 (공유) |
| T_IRQ   | GPIO22 (선택) | 터치 인터럽트 (필수 아님) |

---

### 💾 SD 카드 모듈 연결 (SPI 방식)

> TFT와 같은 SPI 핀 사용. CS만 별도로 설정

| SD 핀   | ESP32 핀 | 설명 |
|---------|----------|------|
| SD_SCK  | GPIO18   | SPI 클럭 (공유) |
| SD_MISO | GPIO19   | SPI 수신 (공유) |
| SD_MOSI | GPIO23   | SPI 송신 (공유) |
| SD_CS   | GPIO5    | SD 카드 선택 핀 (독립적 사용) |

---

## 📦 사용 라이브러리 (아두이노 IDE에서 설치)

- [`lvgl`](https://github.com/lvgl/lvgl)
- [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI)
- [`XPT2046_Touchscreen`](https://github.com/PaulStoffregen/XPT2046_Touchscreen) 또는 다른 SPI 터치 드라이버
- [`SD`](https://github.com/espressif/arduino-esp32/tree/master/libraries/SD)

---

## 📁 TFT_eSPI 설정 방법

`TFT_eSPI/User_Setup.h` 파일을 수정하거나  
`TFT_eSPI` 라이브러리 안에 `User_Setup_Select.h` 에서  
`#include <User_Setups/SetupXXX.h>` 형식으로 커스텀 설정을 연결하세요.

예시 (User_Setup.h):
```
#define ILI9341_DRIVER

#define TFT_CS   10  // Chip select
#define TFT_DC    9  // Data/Command
#define TFT_RST   5  // Reset

#define TFT_MOSI 11
#define TFT_SCLK 12

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000
```


## 📄 Arduino IDE용 코드 (`Hello_LVGL.ino`)
```
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; 
static lv_disp_drv_t disp_drv;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10);

  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  lv_disp_drv_register(&disp_drv);

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello, LVGL on ESP32-S3!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler();
  delay(5);
}
```
