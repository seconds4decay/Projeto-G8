  #include <Wire.h>
  #include <SPI.h>
  #include <Adafruit_PN532.h>
  #include <LiquidCrystal_I2C.h>
  #include <SoftwareSerial.h>

  #define PN532_IRQ (2)
  #define PN532_RESET (3)

  LiquidCrystal_I2C lcd(0x27, 20, 4); // Configurações da Tela LCD
  Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET); // Configurações da Placa PN532
  SoftwareSerial mySerial(9,10); // Configurações da Comunicação Serial entre os dois arduinos

  int buzzer = 8;
  int buttonback = 3;
  int buttonselect = 4;
  int buttonnext = 5;

  int option = 0;
  int linha_selecionada;

  String linhas[] = { "DANTAS BARRETO", "PIEDADE OPCIONAL", "RIO DOCE - CDU" }; // Linhas de onibus da parada

  void setup(void) {
    Serial.begin(9600); // Inicialização do Serial
    mySerial.begin(9600); // Inicialização da comunicação entre os dois arduinos
    nfc.begin(); // Inicialização da placa PN532
    lcd.init(); // Inicialização do LCD
    pinMode(buzzer, OUTPUT);
    pinMode(buttonback, INPUT);
    pinMode(buttonselect, INPUT);
    pinMode(buttonnext, INPUT);

    uint32_t versiondata = nfc.getFirmwareVersion(); // Detecção de Presença da Placa PN53x
    if (!versiondata) {
      Serial.print("Placa PN53x não encontrada");
      while (1);
    }
    Serial.print("Placa PN53x encontrada");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);
    Serial.println("Aguardando o cartão...");
  }

  void loop(void) {
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    uint8_t pcard[] = { 0x7D, 0xC6, 0x7C, 0x89 };
    uint8_t pcphone[] = { 0x91, 0x38, 0x50, 0xD3 };
    uint8_t uidLength;
    bool igual = true;

    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength); // Leitura do Cartão
    if (success) {
      Serial.println("Encontrei um cartão");
      Serial.print("  UID Length: ");
      Serial.print(uidLength, DEC);
      Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);
      Serial.println("");
      for (int x = 0; x < 4; x++) {
        if (uid[x] != pcard[x] && uid[x] != pcphone[x]) igual = false; // Validação do cartão
      }
      if (igual) {
        tone(8, 392);
        delay(1000);
        noTone(8);
        selecionar(); 
      }
      delay(2000);
      lcd.noBacklight();
      lcd.clear();
      igual = true;
      Serial.read();
      Serial.flush();
    }
  }
  void atualizar() { // Função de Atualizar a tela sempre que necessário
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Linha:");
    lcd.setCursor(3, 1);
    lcd.print(linhas[option]);
  }
  void selecionar() { // Função da Tela LCD, escolher sua linha de onibus.
    option = 0;
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print("Bem-Vindo");
    lcd.setCursor(6, 1);
    lcd.print("Usuario");
    lcd.setCursor(0, 2);
    lcd.print("Selecione Sua Linha");
    lcd.setCursor(0, 3);
    delay(3000);
    atualizar();

    while (true) {
      if (digitalRead(buttonback) == HIGH) { // Voltar na seleção de linha
        while (digitalRead(buttonback) == HIGH) {}
        if (option != 0) {
          Serial.print("back: ");
          Serial.println(option);
          option--;
          atualizar();
        }
      }
      if (digitalRead(buttonnext) == HIGH) { // Avançar na seleção de linha
        while (digitalRead(buttonnext) == HIGH) {}
        if (option != 2) {
          Serial.print("next: ");
          Serial.println(option);
          option++;
          atualizar();
        }
      }
      if (digitalRead(buttonselect) == HIGH) { // Selecionar Linha
        while (digitalRead(buttonselect) == HIGH) {}
        Serial.print("select: ");
        linha_selecionada = option;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Linha Selecionada!");
        Serial.print(linha_selecionada);
        mySerial.write(linha_selecionada); // Comunicar com o segundo arduino
        break;
      }
    }
  }
