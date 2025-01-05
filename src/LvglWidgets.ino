#include "setupDevice.h"
#include <Arduino.h>
#include <genQrCode.h>
#include <mqtt.h>
#include <rfid.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("LVGL Demo");

  setup_wifi_mqtt();

  setupRfid_init();

  setupDevice_init();
  if(get_disp_draw_buf()) {
    create_qr_code_screen();
  }
}

void loop()
{
  setupDevice_loop();
  checkRfid_loop();
  loop_mqtt();
  display_loop();
}