#ifndef GENQRCODE_h
#define GENQRCODE_h

#include <lvgl.h>

void create_qr_code_screen();
void update_qr_code_MQTT(const char *text);
void updateNoticeLable(const char *textLable);
extern bool accessQr;

#endif
