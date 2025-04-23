#define TEST_MODE
#define ERROR_LOGGING
#include "main.h"

void setup()
{
  Serial.begin(921600);
  while (!Serial)
    ;

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    LOG_ERR("ERR: SPIFFS Mount Failed");
    return;
  }

  // TODO: Error proof
  String config_ = readFile(SPIFFS, "/config.json");
  if (config_ && !config_.isEmpty())
  {
    JSONVar config = JSON.parse(config_);

    WiFi.begin((const char *)config["ssid"], (const char *)config["pwd"]);
    static uint32_t wifi_timer = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
      if (millis() - wifi_timer > 10000)
      {
        wifi_timer = millis();
        Serial.printf("Connection to %s timed out!\n", (const char *)config["ssid"]);
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

  draw_buf = (uint8_t *)heap_caps_malloc(DRAW_BUF_SIZE / 4, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  lv_display_t *disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, (size_t)DRAW_BUF_SIZE / 4);
  lv_display_set_rotation(disp, SCREEN_ROTATION);

  // Initialize an LVGL input device object (Touchscreen).
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

  // Set the callback function to read Touchscreen input.
  lv_indev_set_read_cb(indev, touchscreen_read);

  lv_tick_set_cb(my_tick);

  // Integrate EEZ Studio GUI.
  ui_init();

  // Test ssid
  lv_obj_add_event_cb(objects.home_settings_btn, action_home_to_settings, LV_EVENT_ALL, NULL);

  // Settings screen
  lv_obj_add_event_cb(objects.wifi_pwd_textarea_content, action_settings_text_area_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_settings);

  // Class details screen
  lv_obj_add_event_cb(objects.class_venue_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_class);
  lv_obj_add_event_cb(objects.course_code_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_class);
  lv_obj_add_event_cb(objects.start_time_textarea_content, action_class_details_text_area_numerical_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_class);
  lv_obj_add_event_cb(objects.class_duration_textarea_content, action_class_details_text_area_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_class);

  // Capture screen
  lv_obj_add_event_cb(objects.student_name_textarea_content, action_capture_text_area_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_capture);
  lv_obj_add_event_cb(objects.student_matric_no_year, action_capture_text_area_numerical_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_capture);
  lv_obj_add_event_cb(objects.student_matric_no_reg_no, action_capture_text_area_numerical_event_cb, LV_EVENT_ALL, (void *)objects.keyboard_capture);

  lv_obj_add_event_cb(
      objects.home_screen,
      [](lv_event_t *e) -> void
      {
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_SCREEN_LOADED)
        {
          Serial.println(F("{\"cmd\": \"change_screen\", \"args\":{\"screen_name\":\"home_screen\"}}"));
        }
        return;
      },
      LV_EVENT_SCREEN_LOADED,
      NULL);

  lv_obj_add_event_cb(
      objects.settings_screen,
      [](lv_event_t *e) -> void
      {
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_SCREEN_LOADED)
        {
          network_scanned = false;
          Serial.println(F("{\"cmd\": \"change_screen\", \"args\":{\"screen_name\":\"settings_screen\"}}"));
        }
        return;
      },
      LV_EVENT_SCREEN_LOADED,
      NULL);

  lv_obj_add_event_cb(
      objects.capture_screen,
      [](lv_event_t *e) -> void
      {
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_SCREEN_LOADED)
        {
          Serial.println(F("{\"cmd\": \"change_screen\", \"args\":{\"screen_name\":\"capture_screen\"}}"));
        }
        return;
      },
      LV_EVENT_SCREEN_LOADED,
      NULL);

  lv_obj_add_event_cb(
      objects.class_details_screen,
      [](lv_event_t *e) -> void
      {
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_SCREEN_LOADED)
        {
          Serial.println(F("{\"cmd\": \"change_screen\", \"args\":{\"screen_name\":\"class_details_screen\"}}"));
        }
        return;
      },
      LV_EVENT_SCREEN_LOADED,
      NULL);

  lv_obj_add_event_cb(
      objects.current_user_screen,
      [](lv_event_t *e) -> void
      {
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_SCREEN_LOADED)
        {
          Serial.println(F("{\"cmd\": \"change_screen\", \"args\":{\"screen_name\":\"current_user_screen\"}}"));
        }
        return;
      },
      LV_EVENT_SCREEN_LOADED,
      NULL);

  lv_obj_add_event_cb(objects.keyboard_capture, hide_keyboard, LV_EVENT_READY, NULL);
  lv_obj_add_event_cb(objects.keyboard_class, hide_keyboard, LV_EVENT_READY, NULL);
  lv_obj_add_event_cb(objects.keyboard_settings, hide_keyboard, LV_EVENT_READY, NULL);
  
  lv_dropdown_set_options(objects.wifi_ssid_dropdown, get_var_networks());
  
  // Time keeping
  if (WiFi.isConnected())
  {
    esp_sntp_servermode_dhcp(1); // (optional)
    sntp_set_time_sync_notification_cb(timeavailable);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  }
}

void loop()
{

  if (lv_scr_act() == objects.settings_screen && !network_scanned)
  {
    int16_t WiFiScanStatus = WiFi.scanComplete();
    if (WiFiScanStatus < 0)
    { // it is busy scanning or got an error
      if (WiFiScanStatus == WIFI_SCAN_FAILED)
      {
        startWiFiScan();
      }
      // other option is status WIFI_SCAN_RUNNING - just wait.
    }
    else
    { // Found Zero or more Wireless Networks
      printScannedNetworks(WiFiScanStatus);
      lv_dropdown_set_options(objects.wifi_ssid_dropdown, networks.c_str());
      network_scanned = true;
      startWiFiScan(); // start over...
    }
  }

  if (lv_scr_act() == objects.home_screen)
  {
    static uint32_t home_screen_timer = millis();
    if (millis() - home_screen_timer > 1000)
    {
      getTime();
      home_screen_timer = millis();
    }
  }

  if (lv_scr_act() == objects.current_user_screen)
  {
    static char read_char;
    read_char = 0;
    current_user_str = "";
    static uint32_t panel_timer = millis();

    while (Serial.available() > 0)
    {
      read_char = Serial.read();
      current_user_str += read_char;
    }

    if (!current_user_str.isEmpty())
    {
      current_user = JSON.parse(current_user_str);
      if (JSON.typeof_(current_user) != "undefined")
      {
        panel_timer = millis();
        lv_label_set_text(objects.current_user_matric_no, current_user["matric_no"]);
        lv_label_set_text(objects.current_user_level, current_user["level"]);
        lv_label_set_text(objects.current_user_dept, current_user["dept"]);
        // TODO: Show no picture for no auth
        if ((bool)current_user["verified"])
          lv_image_set_src(objects.current_user_image, &img_img_accept);
        else
          lv_image_set_src(objects.current_user_image, &img_img_reject);
        lv_obj_clear_flag(objects.current_user_info_panel, LV_OBJ_FLAG_HIDDEN);
      }
    }

    if (millis() - panel_timer > 5000)
    {
      lv_obj_add_flag(objects.current_user_info_panel, LV_OBJ_FLAG_HIDDEN);
      panel_timer = millis();
    }
  }

  if (lv_scr_act() == objects.capture_screen && !is_capture_panel_hidden && Serial.available() > 0)
  {
    static char read_char;
    static String capture_panel_content;

    while (Serial.available() > 0)
    {
      read_char = Serial.read();
      capture_panel_content += read_char;
    }

    lv_label_set_text(objects.capture_info_text, capture_panel_content.c_str());
    capture_panel_content = "";
  }

  update_UI();
  delay(5);
}

void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    LOG_ERR("An error has occurred while mounting SPIFFS");
  }
  LOG("SPIFFS mounted successfully");
}

extern const char *get_var_networks()
{
  return networks.c_str();
}

extern void set_var_networks(const char *value)
{
  networks = value;
}

String readFile(fs::FS &fs, const char *path)
{
  static String file_content;
  LOGF("Reading file: %s\r\n", path);

  fs::File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    LOG_ERR("- failed to open file for reading");
    return String("");
  }

  LOG("- read from file:");
  char read_char;
  while (file.available())
  {
    read_char = file.read();
    file_content += read_char;
  }
  file.close();
  return file_content;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  LOGF("Writing file: %s\r\n", path);

  fs::File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    LOG_ERR("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    LOGF("- file written");
  }
  else
  {
    LOG_ERR("- write failed");
  }
  file.close();
}

// Subroutines to update screen displays or widgets.
void update_UI()
{
  lv_tick_inc(millis() - lastTick); //--> Update the tick timer. Tick is new for LVGL 9.
  lastTick = millis();
  lv_timer_handler(); //--> Update the UI.
}

void getTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 10))
  {
    LOG_ERR("No time available (yet)");
    return;
  }
  char date[16], time[16];
  strftime(date, 16, "%d/%m/%Y", &timeinfo);
  strftime(time, 16, "%I:%M %p", &timeinfo);
  // LOGF("Date: %s \nTime: %s\n", date, time);
  lv_label_set_text(objects.home_date_content, date);
  lv_label_set_text(objects.home_time_content, time);
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  LOG("Got time adjustment from NTP!");
  getTime();
}

// Get the Touchscreen data.
void touchscreen_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  if (touchscreen.tirqTouched() && touchscreen.touched())
  {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    data->point.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    data->point.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static uint32_t my_tick(void)
{
  return millis();
}

void action_settings_text_area_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_move_foreground(objects.keyboard_settings);
    lv_keyboard_set_textarea(objects.keyboard_settings, ta);
    lv_obj_clear_flag(objects.keyboard_settings, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(objects.keyboard_settings, NULL);
    lv_obj_add_flag(objects.keyboard_settings, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_capture_text_area_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_move_foreground(objects.keyboard_capture);
    lv_keyboard_set_textarea(objects.keyboard_capture, ta);
    lv_obj_clear_flag(objects.keyboard_capture, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(objects.keyboard_capture, NULL);
    lv_obj_add_flag(objects.keyboard_capture, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_capture_text_area_numerical_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t *obj = objects.keyboard_capture;

  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_move_foreground(obj);
    lv_keyboard_set_textarea(obj, ta);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(obj, NULL);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_class_details_text_area_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_move_foreground(objects.keyboard_class);
    lv_keyboard_set_textarea(objects.keyboard_class, ta);
    lv_obj_clear_flag(objects.keyboard_class, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(objects.keyboard_class, NULL);
    lv_obj_add_flag(objects.keyboard_class, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_class_details_text_area_numerical_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t *obj = objects.keyboard_class;

  if (code == LV_EVENT_FOCUSED)
  {
    lv_obj_move_foreground(obj);
    lv_keyboard_set_textarea(obj, ta);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(obj, NULL);
    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

void action_home_to_settings(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
    lv_scr_load_anim(objects.settings_screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 0, false);
  return;
}

void action_home_to_capture(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(objects.capture_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 300, 0, false);
  }
  return;
}

void action_home_to_class_details(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(objects.class_details_screen, LV_SCR_LOAD_ANIM_OVER_TOP, 300, 0, false);
  }
  return;
}

void action_save_settings_handler(lv_event_t *e)
{
  char selected_network[32];
  JSONVar ret;
  ret["args"] = JSONVar();
  lv_dropdown_get_selected_str(objects.wifi_ssid_dropdown, selected_network, 32);
  ret["args"]["ssid"] = String(selected_network);
  ret["args"]["pwd"] = String(lv_textarea_get_text(objects.wifi_pwd_textarea_content));

  Serial.printf("Connecting to %s ", selected_network);
  WiFi.begin((const char *)ret["args"]["ssid"], (const char *)ret["args"]["pwd"]);
  static uint32_t wifi_timer = millis();

  // TODO: Adjust this
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - wifi_timer > 10000)
    {
      wifi_timer = millis();
      Serial.printf("Connection to %s timed out!\n", selected_network);
      break;
    }
  }

  LOG("CONNECTED");
  writeFile(SPIFFS, "/config.json", JSON.stringify(ret["args"]).c_str());
  ret["args"]["cmd"] = "change_wifi";
  Serial.println(JSON.stringify(ret).c_str());
  return;
}

void action_settings_to_home(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 300, 0, false);
  }
  return;
}

void action_class_details_to_home(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
  }
  return;
}

void action_start_class_handler(lv_event_t *e)
{
  JSONVar class_details;
  class_details["args"] = JSONVar();
  class_details["args"]["venue"] = lv_textarea_get_text(objects.class_venue_textarea_content);
  class_details["args"]["code"] = lv_textarea_get_text(objects.course_code_textarea_content);

  char level[4];
  lv_dropdown_get_selected_str(objects.class_level_dropdown, level, 4);
  class_details["args"]["level"] = level;

  char dept[4];
  lv_dropdown_get_selected_str(objects.class_department_dropdown, dept, 4);
  class_details["args"]["dept"] = dept;

  char time[4];
  lv_dropdown_get_selected_str(objects.start_time_dropdown, time, 4);

  class_details["args"]["start_time"] = String(lv_textarea_get_text(objects.start_time_textarea_content)) + " " + time;
  class_details["args"]["duration"] = lv_textarea_get_text(objects.class_duration_textarea_content);

  char auth[16];
  lv_dropdown_get_selected_str(objects.class_auth_mode_dropdown, auth, 16);
  class_details["args"]["auth_mode"] = auth;

  class_details["cmd"] = "start_class";
  Serial.println(JSON.stringify(class_details).c_str());
  lv_scr_load_anim(objects.current_user_screen, LV_SCR_LOAD_ANIM_OVER_TOP, 300, 0, false);

  return;
}

void action_capture_to_home(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.

  if (code == LV_EVENT_CLICKED)
  {
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 0, false);
  }
  return;
}

void action_current_user_to_home(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); //--> Get the event code.
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

  if (code == LV_EVENT_GESTURE && dir == LV_DIR_BOTTOM)
  {
    lv_scr_load_anim(objects.home_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 300, 0, false);
  }
  return;
}

void action_capture_fprint_handler(lv_event_t *e)
{
  Serial.println("{\"cmd\":\"capture_fprint\",\"args\":{}}");
  lv_obj_clear_flag(objects.capture_info_panel, LV_OBJ_FLAG_HIDDEN);
  is_capture_panel_hidden = false;
  return;
}

void action_capture_image_handler(lv_event_t *e)
{
  Serial.println("{\"cmd\":\"capture_image\",\"args\":{}}");
  lv_obj_clear_flag(objects.capture_info_panel, LV_OBJ_FLAG_HIDDEN);
  is_capture_panel_hidden = false;
  return;
}

void action_capture_submit_handler(lv_event_t *e)
{
  JSONVar biodata;
  biodata["args"] = JSONVar();

  biodata["args"]["name"] = lv_textarea_get_text(objects.student_name_textarea_content);
  biodata["args"]["matric_no"] = String(lv_textarea_get_text(objects.student_matric_no_year)) + "/" + String(lv_textarea_get_text(objects.student_matric_no_reg_no));

  char level[4], dept[4];
  lv_dropdown_get_selected_str(objects.student_level_dropdown, level, 4);
  biodata["args"]["level"] = level;
  lv_dropdown_get_selected_str(objects.student_department_dropdown, dept, 4);
  biodata["args"]["dept"] = dept;

  biodata["cmd"] = "save_new_student";
  Serial.println(JSON.stringify(biodata));
  return;
}

void action_flash_card_handler(lv_event_t *e)
{
  JSONVar biodata;
  biodata["args"] = JSONVar();

  biodata["args"]["name"] = lv_textarea_get_text(objects.student_name_textarea_content);
  biodata["args"]["matric_no"] = String(lv_textarea_get_text(objects.student_matric_no_year)) + "/" + String(lv_textarea_get_text(objects.student_matric_no_reg_no));

  char level[4], dept[4];
  lv_dropdown_get_selected_str(objects.student_level_dropdown, level, 4);
  biodata["args"]["level"] = level;
  lv_dropdown_get_selected_str(objects.student_department_dropdown, dept, 4);
  biodata["args"]["dept"] = dept;

  biodata["cmd"] = "flash_card";
  Serial.println(JSON.stringify(biodata));
  return;
}

void action_capture_panel_hide(lv_event_t *e)
{
  lv_obj_add_flag(objects.capture_info_panel, LV_OBJ_FLAG_HIDDEN);
  is_capture_panel_hidden = true;
  return;
}

void hide_keyboard(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *keyboard = (lv_obj_t *)lv_event_get_target(e);

  if (code == LV_EVENT_READY)
  {
    lv_keyboard_set_textarea(keyboard, NULL);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  }
}

void startWiFiScan()
{
  WiFi.scanNetworks(true); // 'true' turns Async Mode ON
}

void printScannedNetworks(uint16_t networksFound)
{
  if (networksFound == 0)
    return;
  networks = "";
  for (int i = 0; i < networksFound; ++i)
  {
    networks += WiFi.SSID(i) + '\n';
    delay(10);
  }
  WiFi.scanDelete();
}
