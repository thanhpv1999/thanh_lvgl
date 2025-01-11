#include "rfid.h"
#include <mqtt.h>
#include <genQrCode.h>

String rfIdMaster = "898B313";
bool checkMaster = false;
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
Preferences preferences;

void clean(){
    preferences.begin("rfid", false);
    preferences.clear();
    preferences.end(); 
}

bool checkIfUidExists(const String& uid) {
  preferences.begin("rfid", true);
  String storedUid = preferences.getString(uid.c_str(), "");
  if (storedUid == uid) {
    Serial.println("UID already exists in flash.");
    preferences.end();
    return true; 
  }
  preferences.end(); 
  return false;
}

void writeAndDeleteUidToFlash(const String& uid) {
  if (checkIfUidExists(uid)) {
    preferences.begin("rfid", false);
    preferences.remove(uid.c_str());
    Serial.println("UID removed from flash.");
    updateNoticeLable_rf("Removed card", true);
    preferences.end();
    return; 
  }
  preferences.begin("rfid", false);
  preferences.putString(uid.c_str(), uid);
  Serial.println("UID written to flash.");
  updateNoticeLable_rf("Written card", true);
  preferences.end(); 
}

String readUidFromFlash() {
  preferences.begin("rfid", true);
  String storedUid = preferences.getString("uid", "not found");
  preferences.end();
  return storedUid; 
}

String byteArrayToString(byte* byteArray, byte size) {
  String str = "";
  for (byte i = 0; i < size; i++) {
    str += String(byteArray[i], HEX);
  }
  str.toUpperCase();
  return str;
}

void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}

void setupRfid_init(){
    Serial.println("thanhpv2499.");
    // clean();
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
    rfid.PCD_Init();

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    Serial.println("This code scan the MIFARE Classsic NUID.");
    Serial.print("Using the following key:");
    printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void checkRfid_loop(){

    if (!rfid.PICC_IsNewCardPresent())
    {
        return;
    }

    if (!rfid.PICC_ReadCardSerial())
    {
        return;
    }

    String uidString = byteArrayToString(rfid.uid.uidByte, rfid.uid.size);
    Serial.print("uid : "); 
    Serial.println(uidString);
    if(uidString == rfIdMaster){
        checkMaster = true;
        Serial.println("master card"); 
        updateNoticeLable_rf("Master card", true);
    }else{
        Serial.println("not master card"); 
    }

    if(checkMaster && uidString != rfIdMaster){
        writeAndDeleteUidToFlash(uidString);
        checkMaster = false;
    }else if(uidString != rfIdMaster){
        if(checkIfUidExists(uidString)){
            Serial.println("saved"); 
            updateNoticeLable("rfid: done", true, true);
            publishMessage(MQTT_STATUS_TOPIC, "rfid: done");
        }else{
            updateNoticeLable("rfid: fail", true, false);
            publishMessage(MQTT_STATUS_TOPIC, "rfid: fail");
            Serial.println("not found"); 
        }
    }

    // //Serial.print("PICC type: ");
    // MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    // Serial.println(rfid.PICC_GetTypeName(piccType));

    // if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    //     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    //     piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    // {
    //     //Serial.println("Your tag is not of type MIFARE Classic.");
    //     return;
    // }

    // if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    //     rfid.uid.uidByte[1] != nuidPICC[1] ||
    //     rfid.uid.uidByte[2] != nuidPICC[2] ||
    //     rfid.uid.uidByte[3] != nuidPICC[3])
    // {
    //     //Serial.println("A new card has been detected.");

    //     for (byte i = 0; i < 4; i++)
    //     {
    //         nuidPICC[i] = rfid.uid.uidByte[i];
    //     }

    //     // Serial.println("The NUID tag is:");
    //     // Serial.print("In hex: ");
    //     // printHex(rfid.uid.uidByte, rfid.uid.size);
    //     // Serial.println();
    //     // Serial.print("In dec: ");
    //     // printDec(rfid.uid.uidByte, rfid.uid.size);
    //     // Serial.println();
    // }
    // else
    //     // Serial.println("Card read previously.");

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}