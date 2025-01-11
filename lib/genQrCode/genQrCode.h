#ifndef GENQRCODE_h
#define GENQRCODE_h

#include <lvgl.h>

void create_qr_code_screen();
void update_qr_code_MQTT(const char *text);
void updateNoticeLable(const char *textLable, bool update);
void updateNoticeLable_rf(const char *textLable, bool update);
void updateNoticeLable_mqtt(const char *textLable);
void display_loop();
extern bool accessQr;

#endif
