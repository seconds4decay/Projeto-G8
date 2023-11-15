#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial mySerial(0,1);

int data;

void setup() {
  mySerial.begin(9600)
}

void loop() {
  if (mySerial.available()) {
    int data = mySerial.read();

    lcd.backlight();
    lcd.setCursor(3, 0)
  }
}
