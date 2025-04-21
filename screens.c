#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_home_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.home_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffdedede), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 43);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // home_time_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.home_time_label = obj;
                    lv_obj_set_pos(obj, -5, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Time:");
                }
                {
                    // home_time_content
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.home_time_content = obj;
                    lv_obj_set_pos(obj, 44, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "00:00 AM");
                }
                {
                    // home_date_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.home_date_label = obj;
                    lv_obj_set_pos(obj, 168, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Date:");
                }
                {
                    // home_date_content
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.home_date_content = obj;
                    lv_obj_set_pos(obj, 215, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "02/04/2025");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 29, 54);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, -3);
                    lv_obj_set_size(obj, 232, 80);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Automatic biometric attendance tracking system \nsubmitted by\nOluwaseyi Aderemi\n2020/9643");
                }
            }
        }
        {
            // home_add_user_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.home_add_user_btn = obj;
            lv_obj_set_pos(obj, 10, 173);
            lv_obj_set_size(obj, 80, 50);
            lv_obj_add_event_cb(obj, action_home_to_capture, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Add User");
                }
            }
        }
        {
            // home_start_class_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.home_start_class_btn = obj;
            lv_obj_set_pos(obj, 110, 173);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, 50);
            lv_obj_add_event_cb(obj, action_home_to_class_details, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff53b117), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Start Class");
                }
            }
        }
        {
            // home_settings_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.home_settings_btn = obj;
            lv_obj_set_pos(obj, 230, 173);
            lv_obj_set_size(obj, 80, 50);
            lv_obj_add_event_cb(obj, action_home_to_settings, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff6d6c6c), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Settings");
                }
            }
        }
    }
    
    tick_screen_home_screen();
}

void tick_screen_home_screen() {
}

void create_screen_settings_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffdedede), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 43);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 108, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Settings");
                }
                {
                    // settings_back_btn
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.settings_back_btn = obj;
                    lv_obj_set_pos(obj, 11, -8);
                    lv_obj_set_size(obj, 30, 30);
                    lv_obj_add_event_cb(obj, action_settings_to_home, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            lv_obj_set_pos(obj, -6, -1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_img_back);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 61, 69);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "WiFi SSID:");
        }
        {
            // wifi_ssid_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.wifi_ssid_dropdown = obj;
            lv_obj_set_pos(obj, 148, 59);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 26, 110);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "WiFi Password:");
        }
        {
            // wifi_pwd_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.wifi_pwd_textarea_content = obj;
            lv_obj_set_pos(obj, 148, 100);
            lv_obj_set_size(obj, 150, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 10, 144);
            lv_obj_set_size(obj, 300, 150);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_save_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.settings_save_btn = obj;
            lv_obj_set_pos(obj, 130, 179);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_add_event_cb(obj, action_save_settings_handler, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Save");
                }
            }
        }
        {
            // keyboard_settings
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_settings = obj;
            lv_obj_set_pos(obj, 0, 100);
            lv_obj_set_size(obj, 320, 142);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING|LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_settings_screen();
}

void tick_screen_settings_screen() {
    {
        const char *new_val = get_var_networks();
        const char *cur_val = lv_dropdown_get_options(objects.wifi_ssid_dropdown);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.wifi_ssid_dropdown;
            lv_dropdown_set_options(objects.wifi_ssid_dropdown, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_class_details_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.class_details_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(obj, LV_DIR_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffdedede), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 43);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 88, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Class Details");
                }
                {
                    // class_details_back_btn
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.class_details_back_btn = obj;
                    lv_obj_set_pos(obj, 11, -8);
                    lv_obj_set_size(obj, 30, 30);
                    lv_obj_add_event_cb(obj, action_class_details_to_home, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            lv_obj_set_pos(obj, -6, -1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_img_back);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 40, 69);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Class venue:");
        }
        {
            // class_venue_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.class_venue_textarea_content = obj;
            lv_obj_set_pos(obj, 135, 59);
            lv_obj_set_size(obj, 163, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 34, 106);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Course code:");
        }
        {
            // course_code_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.course_code_textarea_content = obj;
            lv_obj_set_pos(obj, 135, 96);
            lv_obj_set_size(obj, 163, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 90, 144);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Level:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 36, 181);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
            lv_label_set_text(obj, "Department:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 52, 219);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Start time:");
        }
        {
            // start_time_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.start_time_textarea_content = obj;
            lv_obj_set_pos(obj, 135, 209);
            lv_obj_set_size(obj, 83, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 24, 256);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Class duration:");
        }
        {
            // class_duration_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.class_duration_textarea_content = obj;
            lv_obj_set_pos(obj, 135, 246);
            lv_obj_set_size(obj, 83, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 10, 344);
            lv_obj_set_size(obj, 300, 150);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // start_class_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.start_class_btn = obj;
            lv_obj_set_pos(obj, 110, 336);
            lv_obj_set_size(obj, 100, 37);
            lv_obj_add_event_cb(obj, action_start_class_handler, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff53b117), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Start Class");
                }
            }
        }
        {
            // keyboard_class
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_class = obj;
            lv_obj_set_pos(obj, 1, 98);
            lv_obj_set_size(obj, 320, 142);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING|LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // class_department_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.class_department_dropdown = obj;
            lv_obj_set_pos(obj, 135, 171);
            lv_obj_set_size(obj, 161, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "BME\nCEN\nCVE\nEEE\nMCT\nMEE\nTCE");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // class_level_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.class_level_dropdown = obj;
            lv_obj_set_pos(obj, 135, 134);
            lv_obj_set_size(obj, 161, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "100\n200\n300\n400\n500");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // start_time_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.start_time_dropdown = obj;
            lv_obj_set_pos(obj, 233, 209);
            lv_obj_set_size(obj, 63, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "AM\nPM");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 224, 256);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "hour(s)");
        }
        {
            // class_auth_mode_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.class_auth_mode_dropdown = obj;
            lv_obj_set_pos(obj, 135, 286);
            lv_obj_set_size(obj, 161, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "NONE\nFINGERPRINT\nFACE\nHYBRID");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 21, 289);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Authentication\nMode:");
        }
    }
    
    tick_screen_class_details_screen();
}

void tick_screen_class_details_screen() {
}

void create_screen_capture_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.capture_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(obj, LV_DIR_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffdedede), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 43);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 71, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Student Biodata");
                }
                {
                    // capture_back_btn
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.capture_back_btn = obj;
                    lv_obj_set_pos(obj, 11, -8);
                    lv_obj_set_size(obj, 30, 30);
                    lv_obj_add_event_cb(obj, action_capture_to_home, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            lv_obj_set_pos(obj, -6, -1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_img_back);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 79, 61);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Name:");
        }
        {
            // student_name_textarea_content
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.student_name_textarea_content = obj;
            lv_obj_set_pos(obj, 130, 51);
            lv_obj_set_size(obj, 163, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 53, 99);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Matric No:");
        }
        {
            // student_matric_no_year
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.student_matric_no_year = obj;
            lv_obj_set_pos(obj, 130, 89);
            lv_obj_set_size(obj, 65, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 85, 137);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Level:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 31, 175);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
            lv_label_set_text(obj, "Department:");
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 10, 287);
            lv_obj_set_size(obj, 300, 131);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // capture_fprint_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.capture_fprint_btn = obj;
            lv_obj_set_pos(obj, 187, 212);
            lv_obj_set_size(obj, 100, 48);
            lv_obj_add_event_cb(obj, action_capture_fprint_handler, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Capture\nFingerprint");
                }
            }
        }
        {
            // capture_image_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.capture_image_btn = obj;
            lv_obj_set_pos(obj, 39, 212);
            lv_obj_set_size(obj, 100, 48);
            lv_obj_add_event_cb(obj, action_capture_image_handler, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Capture \nImage");
                }
            }
        }
        {
            // capture_submit_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.capture_submit_btn = obj;
            lv_obj_set_pos(obj, 78, 334);
            lv_obj_set_size(obj, 164, 48);
            lv_obj_add_event_cb(obj, action_capture_submit_handler, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff53b117), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Submit");
                }
            }
        }
        {
            // flash_card_btn
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.flash_card_btn = obj;
            lv_obj_set_pos(obj, 78, 273);
            lv_obj_set_size(obj, 164, 48);
            lv_obj_add_event_cb(obj, action_flash_card_handler, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb12c17), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Flash Card");
                }
            }
        }
        {
            // keyboard_capture
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard_capture = obj;
            lv_obj_set_pos(obj, 0, 98);
            lv_obj_set_size(obj, 320, 142);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING|LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // student_matric_no_reg_no
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.student_matric_no_reg_no = obj;
            lv_obj_set_pos(obj, 214, 89);
            lv_obj_set_size(obj, 79, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 201, 96);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "/");
        }
        {
            // student_department_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.student_department_dropdown = obj;
            lv_obj_set_pos(obj, 132, 167);
            lv_obj_set_size(obj, 161, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "BME\nCEN\nCVE\nEEE\nMCT\nMEE\nTCE");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // student_level_dropdown
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.student_level_dropdown = obj;
            lv_obj_set_pos(obj, 132, 127);
            lv_obj_set_size(obj, 161, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "100\n200\n300\n400\n500");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // capture_info_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.capture_info_panel = obj;
            lv_obj_set_pos(obj, 11, 24);
            lv_obj_set_size(obj, 300, 200);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING|LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // capture_info_panel_close_btn
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.capture_info_panel_close_btn = obj;
                    lv_obj_set_pos(obj, 238, 1);
                    lv_obj_set_size(obj, 30, 30);
                    lv_obj_add_event_cb(obj, action_capture_panel_hide, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            lv_obj_set_pos(obj, -6, -1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_image_set_src(obj, &img_img_close);
                        }
                    }
                }
                {
                    // capture_info_text
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.capture_info_text = obj;
                    lv_obj_set_pos(obj, 10, 76);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "capture_info_text");
                }
            }
        }
    }
    
    tick_screen_capture_screen();
}

void tick_screen_capture_screen() {
}

void create_screen_current_user_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.current_user_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_current_user_to_home, LV_EVENT_GESTURE, (void *)0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(obj, LV_DIR_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffdedede), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 71, 104);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Awaiting Student");
        }
        {
            // current_user_info_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.current_user_info_panel = obj;
            lv_obj_set_pos(obj, 10, 26);
            lv_obj_set_size(obj, 300, 200);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, -5, 30);
                    lv_obj_set_size(obj, 148, 110);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 1, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Current Student");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 30);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Matric No:");
                        }
                        {
                            // current_user_matric_no
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.current_user_matric_no = obj;
                            lv_obj_set_pos(obj, 65, 30);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "2020/9643");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 60);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Level:");
                        }
                        {
                            // current_user_level
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.current_user_level = obj;
                            lv_obj_set_pos(obj, 39, 60);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "500");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 89);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Dept:");
                        }
                        {
                            // current_user_dept
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.current_user_dept = obj;
                            lv_obj_set_pos(obj, 39, 89);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "MCT");
                        }
                    }
                }
                {
                    // current_user_image
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.current_user_image = obj;
                    lv_obj_set_pos(obj, 141, 23);
                    lv_obj_set_size(obj, 134, 123);
                }
            }
        }
    }
    
    tick_screen_current_user_screen();
}

void tick_screen_current_user_screen() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_home_screen,
    tick_screen_settings_screen,
    tick_screen_class_details_screen,
    tick_screen_capture_screen,
    tick_screen_current_user_screen,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_home_screen();
    create_screen_settings_screen();
    create_screen_class_details_screen();
    create_screen_capture_screen();
    create_screen_current_user_screen();
}
