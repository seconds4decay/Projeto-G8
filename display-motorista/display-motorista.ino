#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(9,10);

String data;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
}

void loop() {
  lcd.noBacklight();
  lcd.print("teste");
  if (mySerial.available()) {
    Serial.println("recebeu");
    Serial.println(data);
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("teste");
  }
}
