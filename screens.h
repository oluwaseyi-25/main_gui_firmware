#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *home_screen;
    lv_obj_t *settings_screen;
    lv_obj_t *class_details_screen;
    lv_obj_t *capture_screen;
    lv_obj_t *current_user_screen;
    lv_obj_t *home_time_label;
    lv_obj_t *home_time_content;
    lv_obj_t *home_date_label;
    lv_obj_t *home_date_content;
    lv_obj_t *home_add_user_btn;
    lv_obj_t *home_start_class_btn;
    lv_obj_t *home_settings_btn;
    lv_obj_t *settings_back_btn;
    lv_obj_t *wifi_ssid_dropdown;
    lv_obj_t *wifi_pwd_textarea_content;
    lv_obj_t *settings_save_btn;
    lv_obj_t *keyboard_settings;
    lv_obj_t *class_details_back_btn;
    lv_obj_t *class_venue_textarea_content;
    lv_obj_t *course_code_textarea_content;
    lv_obj_t *start_time_textarea_content;
    lv_obj_t *class_duration_textarea_content;
    lv_obj_t *start_class_btn;
    lv_obj_t *keyboard_class;
    lv_obj_t *class_department_dropdown;
    lv_obj_t *class_level_dropdown;
    lv_obj_t *start_time_dropdown;
    lv_obj_t *capture_back_btn;
    lv_obj_t *student_name_textarea_content;
    lv_obj_t *student_matric_no_year;
    lv_obj_t *capture_fprint_btn;
    lv_obj_t *capture_image_btn;
    lv_obj_t *capture_submit_btn;
    lv_obj_t *keyboard_capture;
    lv_obj_t *student_matric_no_reg_no;
    lv_obj_t *student_department_dropdown;
    lv_obj_t *student_level_dropdown;
    lv_obj_t *current_user_info_panel;
    lv_obj_t *current_user_matric_no;
    lv_obj_t *current_user_level;
    lv_obj_t *current_user_dept;
    lv_obj_t *current_user_image;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_HOME_SCREEN = 1,
    SCREEN_ID_SETTINGS_SCREEN = 2,
    SCREEN_ID_CLASS_DETAILS_SCREEN = 3,
    SCREEN_ID_CAPTURE_SCREEN = 4,
    SCREEN_ID_CURRENT_USER_SCREEN = 5,
};

void create_screen_home_screen();
void tick_screen_home_screen();

void create_screen_settings_screen();
void tick_screen_settings_screen();

void create_screen_class_details_screen();
void tick_screen_class_details_screen();

void create_screen_capture_screen();
void tick_screen_capture_screen();

void create_screen_current_user_screen();
void tick_screen_current_user_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();
extern bool network_scanned;


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/