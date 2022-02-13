#include <Wire.h>

#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

/*
  AT24C32 - > 32768 bits.
  32768 bits / 8 = 4096 Bytes.
*/

int EX_EEPROM_I2C_ADDRESS = 0X54;
int MEM_START_ADDRESS = 0;
int MEM_END_ADDRESS  = 32768;
unsigned int CURRUNT_ADDRESS = 0;

boolean START_WRITE = false;
boolean START_READ = false;

int WRITE_BTN = 2;
int READ_BTN = 3;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("External EEPROM");

  pinMode(WRITE_BTN, INPUT);
  pinMode(READ_BTN, INPUT);

}

void loop() {
  if (!digitalRead(WRITE_BTN)) {
    clearLCDrow();
    delay(50);
    if (!START_WRITE) {
      START_WRITE = true;
      Serial.println("start writing !");
      for (int i = 0; i < 10; i++) {
        lcd.setCursor(0, 1);
        lcd.print(String("WR to ADD : ") + String(i));
        Serial.print(String("writing data to address ") + (String("0x0")) + String(i));
        Serial.println();
        CURRUNT_ADDRESS = i;
        writeEEPROM(i);
        delay(500);
        if (i == 9) {
          START_WRITE = false;
          clearLCDrow();
          lcd.setCursor(0, 1);
          lcd.print("writing done !");
          Serial.println("writing done !");
        }
      }
    }
  }
  if (!digitalRead(READ_BTN)) {
    clearLCDrow();
    delay(50);
    if (!START_READ) {
      START_READ = true;
      Serial.println("start reading !");
      for (int i = 0; i < 10; i++) {
        lcd.setCursor(0, 1);
        lcd.print(String("R from ADD : ") + String(i, HEX));
        CURRUNT_ADDRESS = i;
        Serial.print(String("data from address ") + (String("0x0")) + String(i) + String(" : "));
        Serial.print(readEEPROM(i), DEC);
        Serial.println();
        delay(500);
        if (i == 9) {
          START_READ = false;
          clearLCDrow();
          lcd.setCursor(0, 1);
          lcd.print("reading done !");
          Serial.println("reading done !");
        }
      }
    }
  }
}

void writeEEPROM(byte data ) {
  Wire.beginTransmission(EX_EEPROM_I2C_ADDRESS);
  Wire.write((int)(CURRUNT_ADDRESS >> 8));   // MSB
  Wire.write((int)(CURRUNT_ADDRESS & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}

byte readEEPROM(unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(EX_EEPROM_I2C_ADDRESS);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(EX_EEPROM_I2C_ADDRESS, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void clearLCDrow() {
  for (int i = 0; i < 17; i++) {
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
}
