
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

---

## ⚙️ lv_conf.h 주요 설정 (요약)

```c
/* lv_conf.h - Configuration file for LVGL v9.2.2 (for ESP32-S3 + ILI9341) */

#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

/*====================
   MEMORY SETTINGS
 *====================*/
#define LV_USE_STDLIB_MALLOC LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING LV_STDLIB_BUILTIN
#define LV_MEM_SIZE (64 * 1024U)
#define LV_MEM_POOL_EXPAND_SIZE 0
#define LV_MEM_ADR 0

/*====================
   HAL SETTINGS
 *====================*/
#define LV_DEF_REFR_PERIOD 33
#define LV_DPI_DEF 130

/*====================
   OS SETTINGS
 *====================*/
#define LV_USE_OS LV_OS_NONE

/*====================
   DRAW SETTINGS
 *====================*/
#define LV_DRAW_BUF_STRIDE_ALIGN 1
#define LV_DRAW_BUF_ALIGN 4
#define LV_USE_DRAW_SW 1
#define LV_DRAW_SW_COMPLEX 1
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE (24 * 1024)
#define LV_DRAW_THREAD_STACK_SIZE (8 * 1024)

/*====================
   FONT SETTINGS
 *====================*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14
#define LV_USE_FONT_PLACEHOLDER 1

/*====================
   TEXT SETTINGS
 *====================*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_)]}"

/*====================
   WIDGET SETTINGS
 *====================*/
#define LV_WIDGETS_HAS_DEFAULT_VALUE 1
#define LV_USE_LABEL 1
#define LV_USE_BUTTON 1
#define LV_USE_SLIDER 1
#define LV_USE_SWITCH 1
#define LV_USE_LED 1
#define LV_USE_LIST 1
#define LV_USE_DROPDOWN 1
#define LV_USE_CANVAS 1
#define LV_USE_IMAGE 1
#define LV_USE_TEXTAREA 1
#define LV_USE_CHART 1

/*====================
   THEMES
 *====================*/
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 0
#define LV_THEME_DEFAULT_GROW 1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80

/*====================
   LAYOUTS
 *====================*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/*====================
   DISPLAY DRIVER
 *====================*/
#define LV_USE_ILI9341 1
#define LV_USE_TOUCHPAD 0

/*====================
   EXAMPLES & DEMOS
 *====================*/
#define LV_BUILD_EXAMPLES 1
#define LV_USE_DEMO_WIDGETS 0

#endif /* LV_CONF_H */

```

전체 `lv_conf.h`는 [여기에서 참조](https://github.com/lvgl/lvgl/blob/master/lv_conf_template.h)하거나 직접 작성 가능

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
## 🔧 각 라이브러리의 역할

### ✅ TFT_eSPI

- **목적:** ESP32/ESP8266 전용의 고속 디스플레이 드라이버  
- **기능:** SPI 디스플레이(TFT LCD)와 직접 통신하여 픽셀을 그리는 저수준 API 제공  
- **예:**  
  ```cpp
  tft.drawLine();
  tft.drawRect();
  tft.fillScreen();
  tft.setRotation();
  ```

👉 하드웨어 제어와 관련된 모든 것을 직접 처리합니다.

---

### ✅ LVGL (Light and Versatile Graphics Library)

- **목적:** 복잡한 **GUI 위젯**(버튼, 슬라이더, 차트, 그래프, 텍스트 등)을 만들 수 있는 고급 그래픽 프레임워크  
- **기능:** 터치 대응, 테마 관리, 애니메이션, 이벤트 처리 등 포함  
- **예:**  
  ```cpp
  lv_btn_create();
  lv_label_set_text();
  lv_chart_set_next_value();
  ```

👉 UI 구성과 이벤트 관리 등 고급 기능을 담당합니다.

---

## 🤝 두 라이브러리를 함께 쓰는 이유

| 구분      | 이유                                                                 |
|-----------|----------------------------------------------------------------------|
| **TFT_eSPI** | LVGL이 직접 하드웨어에 접근할 수 없기 때문에, 디스플레이 제어를 맡깁니다. |
| **LVGL**     | 복잡한 UI 위젯, 터치 처리, 테마 등 고급 기능을 쉽게 구현할 수 있습니다.      |

> 💡 즉, **TFT_eSPI는 하드웨어**, **LVGL은 UI 구성**이라는 역할 분담 구조입니다.

---

## 🧠 동작 흐름 예시

```
[ 사용자 (터치 입력) ]
         ↓
[ LVGL: UI 구성 및 이벤트 처리 ]
         ↓
[ lv_disp_flush() → TFT_eSPI 호출 ]
         ↓
[ 실제 LCD 화면에 그리기 ]
```

---

## 📌 정리하면

| 항목       | LVGL           | TFT_eSPI         |
|------------|----------------|------------------|
| **목적**    | 고급 GUI 구성     | 하드웨어 제어       |
| **구성요소** | 버튼, 차트, 슬라이더 등 | 픽셀, 선, 사각형 등 |
| **수준**    | 고수준 API       | 저수준 API         |
| **사용 이유** | UI를 쉽게 만들기 위해 | 화면에 빠르게 그리기 위해 |

---

## ✨ 한 줄 요약

> **TFT_eSPI는 손과 발**,  
> **LVGL은 두뇌**입니다.  
> 하나는 **화면에 그리는 도구**, 다른 하나는 **어떻게 그릴지를 결정하는 뇌 역할**을 합니다.



## ✅ LVGL Hello World 예제 (v9.2.2용)

```
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// 하나의 버퍼와 draw_buf 사용
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

  // 버퍼 초기화 (단일 버퍼)
  lv_draw_buf_init(&draw_buf1, 320, 10, LV_COLOR_FORMAT_RGB565, 320, buf1, sizeof(buf1));

  // 디스플레이 생성 및 설정
  lv_display_t *disp = lv_display_create(320, 240);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_draw_buffers(disp, &draw_buf1, NULL);  // draw_buf1만 사용

  // Hello World 표시
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


