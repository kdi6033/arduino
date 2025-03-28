
# ESP32-S3 + ILI9341 + LVGL Hello World (Arduino IDE)

이 프로젝트는 **ESP32-S3 보드와 ILI9341 2.4"/2.8"/3.2" SPI 디스플레이**를 연결하여  
**LVGL로 "Hello, World!" 텍스트를 화면 중앙에 출력**하는 예제입니다.

유튜브 참고 영상: [https://www.youtube.com/watch?v=NvBblQnWhsQ](https://www.youtube.com/watch?v=NvBblQnWhsQ)

---

## ✅ 하드웨어 연결 (핀맵)

| ILI9341 핀 | ESP32-S3 핀 | 설명       |
|------------|-------------|------------|
| VCC        | 3.3V        | 전원       |
| GND        | GND         | 그라운드   |
| CS         | GPIO10      | Chip Select |
| RESET      | GPIO5       | Reset 핀    |
| DC/RS      | GPIO9       | Data/Command |
| SDI(MOSI)  | GPIO11      | SPI MOSI    |
| SCK        | GPIO12      | SPI Clock   |
| LED        | 3.3V        | 백라이트    |

※ T_IRQ, T_DO 등 터치 관련 핀은 추후 추가

---

## 🔧 아두이노 IDE 설정

1. **보드 매니저 설정**
   - 보드: ESP32-S3 Dev Module
   - Upload Speed: 921600 또는 115200
   - Flash Size: 8MB
   - USB CDC On Boot: Enabled
   - PSRAM: Enabled

2. **필요 라이브러리 설치**
   - **lvgl** by LVGL (v8.x 이상)
   - **TFT_eSPI** by Bodmer

---

## 📁 TFT_eSPI 설정 방법

`TFT_eSPI/User_Setup.h` 파일을 수정하거나  
`TFT_eSPI` 라이브러리 안에 `User_Setup_Select.h` 에서  
`#include <User_Setups/SetupXXX.h>` 형식으로 커스텀 설정을 연결하세요.

예시 (User_Setup.h):
```cpp
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



---

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
'''
