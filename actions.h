#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_home_to_settings(lv_event_t * e);
extern void action_home_to_capture(lv_event_t * e);
extern void action_home_to_class_details(lv_event_t * e);
extern void action_save_settings_handler(lv_event_t * e);
extern void action_settings_to_home(lv_event_t * e);
extern void action_class_details_to_home(lv_event_t * e);
extern void action_start_class_handler(lv_event_t * e);
extern void action_capture_to_home(lv_event_t * e);
extern void action_capture_fprint_handler(lv_event_t * e);
extern void action_capture_submit_handler(lv_event_t * e);
extern void action_text_area_event_cb(lv_event_t * e);
extern void action_flash_card_handler(lv_event_t * e);
extern void action_current_user_to_home(lv_event_t * e);
extern void action_capture_image_handler(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/