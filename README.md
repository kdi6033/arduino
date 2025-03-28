
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

# LVGL + ESP32 + ILI9341 (2.4") 사용 가이드 (Arduino용)

본 문서는 ESP32-S3 보드와 2.4인치 ILI9341 디스플레이를 사용하여 LVGL v9.2.2 환경을 설정하고 "Hello, World!" 예제를 실행하는 방법을 설명합니다. 학생 교재 및 GitHub 공유용으로 구성되었습니다.

---

## 📦 LVGL 설치 이유

LVGL은 임베디드 시스템에서 고급 GUI를 구현할 수 있는 강력한 오픈소스 라이브러리입니다. 
ESP32와 함께 사용하면 상업용 HMI 수준의 UI를 만들 수 있어, 교육 및 프로젝트 실습에 매우 적합합니다.

---

## 📁 설치 라이브러리 목록

Arduino IDE에서 아래 라이브러리를 설치합니다:

1. **LVGL** (v9.2.2 권장)
   - Arduino IDE → "라이브러리 매니저" → `lvgl` 검색 → 설치

2. **TFT_eSPI**
   - "라이브러리 매니저"에서 `TFT_eSPI` 검색 후 설치
   - 설치 후 `User_Setup.h` 또는 `User_Setup_Select.h`에서 핀 설정 필요

3. **lv_conf.h 설정 파일 생성**
   - `lv_conf_template.h`를 복사하여 `lv_conf.h`로 이름 변경
   - 해당 파일을 다음 경로에 위치시킵니다:

     ```
     D:/Develop/Arduino/libraries/lv_conf.h
     ```
     ※ `lvgl` 폴더 바로 바깥에 위치해야 LVGL이 인식함 (`../../lv_conf.h`)

---

## 🔌 ILI9341 2.4인치 디스플레이 핀 설정 예시

```cpp
#define ILI9341_DRIVER
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_MISO -1
#define TFT_BL   21
```

> `TFT_eSPI/User_Setup.h`에 설정 (보드 환경에 맞게 수정 필요)

---

## ⚙️ lv_conf.h 주요 설정 (요약)

```c
#define LV_COLOR_DEPTH 16
#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240
#define LV_MEM_SIZE (64 * 1024U)
#define LV_USE_LOG 0
#define LV_FONT_DEFAULT &lv_font_montserrat_14
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_USE_LABEL 1
#define LV_USE_BTN 1
#define LV_USE_SLIDER 1
#define LV_USE_SWITCH 1
#define LV_USE_THEME_DEFAULT 1
```

전체 `lv_conf.h`는 [여기에서 참조](https://github.com/lvgl/lvgl/blob/master/lv_conf_template.h)하거나 직접 작성 가능

---

## ✅ LVGL Hello World 예제 (v9.2.2용)

```cpp
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

static lv_color_t buf1[320 * 10];
static lv_draw_buf_t draw_buf1;

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)px_map, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

void setup() {
  Serial.begin(115200);
  lv_init();
  tft.begin();
  tft.setRotation(1);

  lv_draw_buf_init(&draw_buf1, 320, 10, LV_COLOR_FORMAT_RGB565, 320, buf1, sizeof(buf1));

  lv_display_t *disp = lv_display_create(320, 240);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_draw_buffers(disp, &draw_buf1, NULL);

  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello, World!");
  lv_obj_center(label);
}

void loop() {
  lv_timer_handler();
  delay(5);
}
```

---

## 🧠 주의 사항 요약

| 항목 | 설명 |
|------|------|
| LVGL 버전 | v9.2.2 이상 필요 |
| 함수 이름 변경 | `lv_disp_drv_t` → `lv_display_t`, `lv_scr_act()` → `lv_screen_active()` 등 |
| draw_buf 설정 | `lv_draw_buf_init()` 함수에 7개 인자 필요 |
| flush_cb | `uint8_t *px_map` 사용 (v9 기준) |

---

## 🎓 활용 예시

- IoT PLC UI 구성
- 버튼, 그래프, 입력창 학습
- 실제 HMI 장비 시뮬레이션 제작

---

본 README는 김동일 교수님의 IoT 교육 과정에서 사용되며, GitHub에 올려 누구나 참고할 수 있도록 구성되어 있습니다.

추가 예제나 버튼, 차트, 슬라이더 등 실습도 가능합니다. 🎉



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
