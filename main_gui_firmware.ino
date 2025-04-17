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

void initSPIFFS() {
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    //    Serial.println("An error has occurred while mounting SPIFFS");
  }
  //  Serial.println("SPIFFS mounted successfully");
}

extern const char* get_var_networks() {
  return networks.c_str();
}

extern void set_var_networks(const char* value) {
  networks = value;
}

String readFile(fs::FS& fs, const char* path) {
  static String file_content;
  Serial.printf("Reading file: %s\r\n", path);

  fs::File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return String("");
  }

  Serial.println("- read from file:");
  char read_char;
  while (file.available()) {
    read_char = file.read();
    Serial.write(read_char);
    file_content += read_char;
  }
  file.close();
  return file_content;
}

void writeFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\r\n", path);

  fs::File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

// If logging is enabled, it will inform the user about what is happening in the library.
void log_print(lv_log_level_t level, const char* buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

// Subroutines to update screen displays or widgets.
void update_UI() {
  lv_tick_inc(millis() - lastTick);  //--> Update the tick timer. Tick is new for LVGL 9.
  lastTick = millis();
  lv_timer_handler();  //--> Update the UI.
}

void getTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 10)) {
    Serial.println("No time available (yet)");
    return;
  }
  char date[16], time[16];
  strftime(date, 16, "%d/%m/%Y", &timeinfo);
  strftime(time, 16, "%I:%M %p", &timeinfo);
  Serial.printf("Date: %s \n Time: %s\n", date, time);
  lv_label_set_text(objects.home_date_content, date);
  lv_label_set_text(objects.home_time_content, time);
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  getTime();
}

// Get the Touchscreen data.
void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data) {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    data->point.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    data->point.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static uint32_t my_tick(void) {
  return millis();
}

void action_settings_text_area_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = (lv_obj_t*)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(objects.keyboard_settings);
    lv_keyboard_set_textarea(objects.keyboard_settings, ta);
    lv_obj_clear_flag(objects.keyboard_settings, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(objects.keyboard_settings, NULL);
    lv_obj_add_flag(objects.keyboard_settings, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_capture_text_area_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = (lv_obj_t*)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(objects.keyboard_capture);
    lv_keyboard_set_textarea(objects.keyboard_capture, ta);
    lv_obj_clear_flag(objects.keyboard_capture, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(objects.keyboard_capture, NULL);
    lv_obj_add_flag(objects.keyboard_capture, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_capture_text_area_numerical_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = (lv_obj_t*)lv_event_get_target(e);
  lv_obj_t* obj = objects.keyboard_capture;

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(obj);
    lv_keyboard_set_textarea(obj, ta);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(obj, NULL);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_class_details_text_area_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = (lv_obj_t*)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(objects.keyboard_class);
    lv_keyboard_set_textarea(objects.keyboard_class, ta);
    lv_obj_clear_flag(objects.keyboard_class, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(objects.keyboard_class, NULL);
    lv_obj_add_flag(objects.keyboard_class, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_class_details_text_area_numerical_event_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = (lv_obj_t*)lv_event_get_target(e);
  lv_obj_t* obj = objects.keyboard_class;

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(obj);
    lv_keyboard_set_textarea(obj, ta);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(obj, NULL);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_home_to_settings(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.settings_screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 0, false);
  }
  return;
}

void action_home_to_capture(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.capture_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 300, 0, false);
  }
  return;
}

void action_home_to_class_details(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.class_details_screen, LV_SCR_LOAD_ANIM_OVER_TOP, 300, 0, false);
  }
  return;
}

void action_save_settings_handler(lv_event_t* e) {
  char selected_network[32];
  JSONVar wifi_cred;
  lv_dropdown_get_selected_str(objects.wifi_ssid_dropdown, selected_network, 32);
  wifi_cred["ssid"] = String(selected_network);
  wifi_cred["pwd"] = String(lv_textarea_get_text(objects.wifi_pwd_textarea_content));

  Serial.printf("Connecting to %s ", selected_network);
  WiFi.begin((const char*)wifi_cred["ssid"], (const char*)wifi_cred["pwd"]);
  static uint32_t wifi_timer = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifi_timer > 10000) {
      wifi_timer = millis();
      Serial.printf("Connection to %s timed out!\n", selected_network);
      break;
    }
  }

  Serial.println("CONNECTED");
  writeFile(SPIFFS, "/config.json", JSON.stringify(wifi_cred).c_str());
  wifi_cred["cmd"] = "change_wifi";
  Serial.println(JSON.stringify(wifi_cred).c_str());
  return;
}

void action_settings_to_home(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 300, 0, false);
  }
  return;
}

void action_class_details_to_home(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
  }
  return;
}

void action_start_class_handler(lv_event_t* e) {
  JSONVar class_details;
  class_details["venue"] = lv_textarea_get_text(objects.class_venue_textarea_content);
  class_details["code"] = lv_textarea_get_text(objects.course_code_textarea_content);

  char level[4];
  lv_dropdown_get_selected_str(objects.class_level_dropdown, level, 4);
  class_details["level"] = level;

  char dept[4];
  lv_dropdown_get_selected_str(objects.class_department_dropdown, dept, 4);
  class_details["dept"] = dept;

  char time[4];
  lv_dropdown_get_selected_str(objects.start_time_dropdown, time, 4);

  class_details["start_time"] = String(lv_textarea_get_text(objects.start_time_textarea_content)) + " " + time;
  class_details["duration"] = lv_textarea_get_text(objects.class_duration_textarea_content);

  Serial.println(JSON.stringify(class_details).c_str());
  lv_scr_load_anim(objects.current_user_screen, LV_SCR_LOAD_ANIM_OVER_TOP, 300, 0, false);

  return;
}

void action_capture_to_home(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);  //--> Get the event code.

  if (code == LV_EVENT_CLICKED) {
    Serial.println("");
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 0, false);
  }
  return;
}

void action_capture_fprint_handler(lv_event_t* e) {

  return;
}

void action_capture_submit_handler(lv_event_t* e) {

  return;
}

void startWiFiScan() {
  WiFi.scanNetworks(true);  // 'true' turns Async Mode ON
}

void printScannedNetworks(uint16_t networksFound) {
  if (networksFound == 0) return;
  networks = "";
  for (int i = 0; i < networksFound; ++i) {
    networks += WiFi.SSID(i) + '\n';
    delay(10);
  }
  WiFi.scanDelete();
}


void setup() {
  Serial.begin(921600);
  while (!Serial)
    ;

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // TODO: Error proof
  String config_ = readFile(SPIFFS, "/config.json");
  if (config_ && !config_.isEmpty()) {
    JSONVar config = JSON.parse(config_);

    WiFi.begin((const char*)config["ssid"], (const char*)config["pwd"]);
    static uint32_t wifi_timer = 0;

    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - wifi_timer > 10000) {
        wifi_timer = millis();
        Serial.printf("Connection to %s timed out!\n", (const char*)config["ssid"]);
        break;
      }
    }
  }

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);

  // Start LVGL.
  lv_init();

  draw_buf = (uint8_t*)heap_caps_malloc(DRAW_BUF_SIZE / 4, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  lv_display_t* disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, (size_t)DRAW_BUF_SIZE / 4);
  lv_display_set_rotation(disp, SCREEN_ROTATION);

  // Initialize an LVGL input device object (Touchscreen).
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

  // Set the callback function to read Touchscreen input.
  lv_indev_set_read_cb(indev, touchscreen_read);

  lv_tick_set_cb(my_tick);

  // Integrate EEZ Studio GUI.
  ui_init();

  // Test ssid
  lv_obj_add_event_cb(objects.home_settings_btn, action_home_to_settings, LV_EVENT_ALL, NULL);

  // Settings screen
  lv_obj_add_event_cb(objects.wifi_pwd_textarea_content, action_settings_text_area_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_settings);

  // Class details screen
  lv_obj_add_event_cb(objects.class_venue_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_class);
  lv_obj_add_event_cb(objects.course_code_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_class);
  lv_obj_add_event_cb(objects.start_time_textarea_content, action_class_details_text_area_numerical_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_class);
  lv_obj_add_event_cb(objects.class_duration_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_class);

  // Capture screen
  lv_obj_add_event_cb(objects.student_name_textarea_content, action_capture_text_area_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_capture);
  lv_obj_add_event_cb(objects.student_matric_no_year, action_capture_text_area_numerical_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_capture);
  lv_obj_add_event_cb(objects.student_matric_no_reg_no, action_capture_text_area_numerical_event_cb, LV_EVENT_ALL, (void*)objects.keyboard_capture);

  lv_obj_add_event_cb(
    objects.settings_screen,
    [](lv_event_t* e) -> void {
      lv_event_code_t code = lv_event_get_code(e);

      if (code == LV_EVENT_SCREEN_LOADED)
        network_scanned = false;
      return;
    },
    LV_EVENT_SCREEN_LOADED,
    NULL);

  lv_dropdown_set_options(objects.wifi_ssid_dropdown, get_var_networks());

  // Time keeping
  if (WiFi.isConnected()) {
    esp_sntp_servermode_dhcp(1);  // (optional)
    sntp_set_time_sync_notification_cb(timeavailable);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  }
}



void loop() {
  // put your main code here, to run repeatedly:

  update_UI();

  if (lv_scr_act() == objects.settings_screen && !network_scanned) {
    int16_t WiFiScanStatus = WiFi.scanComplete();
    if (WiFiScanStatus < 0) {  // it is busy scanning or got an error
      if (WiFiScanStatus == WIFI_SCAN_FAILED) {
        Serial.println("WiFi Scan has failed. Starting again.");
        startWiFiScan();
      }
      // other option is status WIFI_SCAN_RUNNING - just wait.
    } else {  // Found Zero or more Wireless Networks
      printScannedNetworks(WiFiScanStatus);
      lv_dropdown_set_options(objects.wifi_ssid_dropdown, networks.c_str());
      network_scanned = true;
      startWiFiScan();  // start over...
    }
  }

  if (lv_scr_act() == objects.home_screen) {
    static uint32_t home_screen_timer = millis();
    if (millis() - home_screen_timer > 1000) {
      getTime();
      home_screen_timer = millis();
    }
  }
  delay(5);
}
