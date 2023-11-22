#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial mySerial(9, 10);

String data;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
}

void loop() {
    if (mySerial.available()) {
      lcd.backlight();
      Serial.println("recebeu");
      Serial.println(data);
      lcd.backlight();
      lcd.setCursor(1, 0);
      lcd.print("PCD na parada.");
      delay(7000);
      lcd.clear();
      lcd.noBacklight();
      mySerial.end();
    mySerial.begin(9600);
  }
}
