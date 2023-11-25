#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Configuração da tela LCD 16x2
SoftwareSerial mySerial(9, 10); // Configuração da comunicação entre os dois arduinos

String data;

void setup() {
  mySerial.begin(9600); // Inicialização da comunicação entre os dois arduinos
  Serial.begin(9600);  // Inicialização do Serial
  lcd.init(); // Inicialização da tela LCD
  lcd.clear(); 
}

void loop() {
    if (mySerial.available()) { // Quando receber um sinal pela comunicação, continua.
      lcd.backlight();
      Serial.println("recebeu");
      Serial.println(data);
      lcd.backlight();
      lcd.setCursor(1, 0);
      lcd.print("PCD na parada.");
      delay(7000);
      lcd.clear();
      lcd.noBacklight();
      mySerial.end(); // Finalizar comunicação 
    mySerial.begin(9600); // Renicializar comunicação e aguardar um proximo sinal
  }
}
