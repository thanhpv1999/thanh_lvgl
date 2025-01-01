#ifndef RFID_h
#define RFID_h

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Preferences.h>

#define SCK_PIN 12  // Serial Clock
#define MISO_PIN 13 // Master In Slave Out
#define MOSI_PIN 11 // Master Out Slave In
#define SS_PIN 10
#define RST_PIN 18

void setupRfid_init();
void checkRfid_loop();

#endif