#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal_I2C.h>

#define PN532_IRQ (2)
#define PN532_RESET (3)

LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int buzzer = 8;
int buttonback = 2;
int buttonselect = 3;
int buttonnext = 4;
int sttsbtnback = LOW;
int sttsbtnbacklast = LOW;
int clickCount = 0;



String linhas[] = { "1967 - DANTAS BARRETO", "064 - PIEDADE OPCIONAL", "1980 - CID. TABAJARA/IGARASSU" };


void setup(void) {
  Serial.begin(115200);
  nfc.begin();
  pinMode(buzzer, OUTPUT);

  pinMode(buttonback, INPUT);
  pinMode(buttonselect, INPUT);
  pinMode(buttonnext, INPUT);

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
  
  sttsbtnback = digitalRead(buttonback);
  if(sttsbtnback == HIGH && sttsbtnbacklast == LOW){
    clickCount++;
    Serial.print(clickCount);
  }
  

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

    lcd.noBacklight();
    igual = true;
    Serial.read();
    Serial.flush();
  }
}

void selecionar() {
  int option = 0;
  while (true) {

    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print("Bem-Vindo");
    lcd.setCursor(6, 1);
    lcd.print("Usuario");
    lcd.setCursor(0, 2);
    lcd.print("Selecione Sua Linha");
    lcd.setCursor(9, 3);
    delay(3000);
    
    lcd.print(linhas[option]);

    /*if (buttonStateBack == HIGH) {
      if (option != 0) {
        option -= 1;
      }
    }
    if (buttonStateNext == HIGH) {
      if (option != 3) {
        option += 1;
      }
    }
    if (buttonStateSelect == HIGH) {
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("Linha Selecionada!");
      break;
    }
    */
  }
}
