/*
  MFRC522 RFID tag reader - ESP32-C3 (SuperMini)
  Library needed: "MFRC522" by GithubCommunity / miguelbalboa
  (Arduino IDE: Sketch > Include Library > Manage Libraries... > search "MFRC522")

  Wiring (default SPI pins on ESP32-C3):
    MFRC522   ESP32-C3
    SDA(SS)   GPIO7
    SCK       GPIO4
    MOSI      GPIO6
    MISO      GPIO5
    RST       GPIO10
    GND       GND
    3.3V      3V3   (NIET 5V, de module werkt op 3.3V!)

  Pas de pinnen hieronder aan als je andere GPIO's gebruikt.
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   7
#define RST_PIN  10

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);          // wacht tot seriële monitor open is
  SPI.begin();               // start SPI met default pins (SCK=4, MISO=5, MOSI=6)
  rfid.PCD_Init();

  Serial.println("Plaats een RFID tag bij de lezer...");
}

void loop() {
  // Is er een nieuwe tag aanwezig?
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Kan de UID gelezen worden?
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // UID uitlezen en printen als hex-string
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print("Tag gedetecteerd - UID: ");
  Serial.println(uidString);

  // Stop communicatie met deze tag zodat de volgende scan proper werkt
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(300); // kleine debounce zodat dezelfde tag niet 50x per seconde gelogd wordt
}
