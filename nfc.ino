#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal_I2C.h>

#define PN532_IRQ (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup(void) {
  Serial.begin(115200);
  nfc.begin();
  pinMode(8, OUTPUT);

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Não encontrei a placa PN53x");
    while (1)
      ;
  }
  Serial.print("Encontrei o PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  Serial.println("Aguardando o cartão...");

  lcd.init();
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t pcard[] = { 0x7D, 0xC6, 0x7C, 0x89 };
  uint8_t uidLength;
  bool igual = true;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    Serial.println("Encontrei um cartão");
    Serial.print("  UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    for (int x = 0; x < 4; x++) {
      if (uid[x] != pcard[x]) igual = false;
    }
    if (igual) {
      tone(8, 392);
      delay(1000);
      noTone(8);
      selecionar();
    }
    delay(2000);

    igual = true;
    Serial.read();
    Serial.flush();
  }
}

void selecionar() {
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Bem-Vindo");
  lcd.setCursor(6, 1);
  lcd.print("Usuario");
  lcd.setCursor(0, 2);
  lcd.print("Selecione Sua Linha");
  lcd.setCursor(9, 3);
  lcd.print("<>");
}
