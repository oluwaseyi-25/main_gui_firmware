#ifdef TEST_MODE
  #define LOG(msg) \
    Serial.println(); \
    Serial.println(msg); \
    Serial.flush();
  #define LOGF(...) \
    Serial.printf(__VA_ARGS__);
#else
  #define LOG(msg)
  #define LOGF(...)
#endif

#ifdef ERROR_LOGGING
  #define LOG_ERR(msg) \
    Serial.println(); \
    Serial.println(msg); \
    Serial.flush();
#else
  #define LOG_ERR(msg)
#endif

//---------------------------------------- Including Libraries.
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "ui.h"
#include "images.h"
#include "actions.h"
#include "vars.h"
#include "WiFi.h"
#include <Arduino_JSON.h>
#include "time.h"
#include "esp_sntp.h"
#include <SPIFFS.h>
//----------------------------------------



#define FORMAT_SPIFFS_IF_FAILED true

// Defines the width and height of the screen, also the font size.
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define SCREEN_ROTATION LV_DISPLAY_ROTATION_90


// Replace with the calibration results on your TFT LCD Touchscreen.
#define touchscreen_Min_X 200
#define touchscreen_Max_X 3700
#define touchscreen_Min_Y 240
#define touchscreen_Max_Y 3800
//----------------------------------------

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint8_t* draw_buf;

// Variables for x, y and z values ​​on the touchscreen.
uint16_t x, y, z;

// Used to track the tick timer.
uint32_t lastTick = 0;
unsigned long previousMillis = 0;

String networks;
bool network_scanned = false;

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 0;

String current_user_str;
JSONVar current_user;
bool is_capture_panel_hidden;


void initSPIFFS();
const char* get_var_networks();
void set_var_networks(const char* value);
String readFile(fs::FS &fs, const char* path);
void writeFile(fs::FS &fs, const char* path, const char* message);
void update_UI();
void getTime();
void timeavailable(struct timeval* t);
void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data);
static uint32_t my_tick();
void startWiFiScan();
void printScannedNetworks(uint16_t networksFound);

// Action Handlers
void hide_keyboard(lv_event_t *e);
void action_settings_text_area_event_cb(lv_event_t* e);
void action_capture_text_area_event_cb(lv_event_t* e);
void action_capture_text_area_numerical_event_cb(lv_event_t* e);
void action_class_details_text_area_event_cb(lv_event_t* e);
void action_class_details_text_area_numerical_event_cb(lv_event_t* e);
void action_home_to_settings(lv_event_t* e);
void action_home_to_capture(lv_event_t* e);
void action_home_to_class_details(lv_event_t* e);
void action_save_settings_handler(lv_event_t* e);
void action_settings_to_home(lv_event_t* e);
void action_class_details_to_home(lv_event_t* e);
void action_start_class_handler(lv_event_t* e);
void action_capture_to_home(lv_event_t* e);
void action_current_user_to_home(lv_event_t* e);
void action_capture_fprint_handler(lv_event_t* e);
void action_capture_image_handler(lv_event_t* e);
void action_capture_submit_handler(lv_event_t* e);
void action_flash_card_handler(lv_event_t* e);
void action_capture_panel_hide(lv_event_t* e);
