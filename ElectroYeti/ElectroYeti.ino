/**
    Copyright ElectroYeti Team. All Right Resereved.
*/

#include <Time.h>
#include <TimeLib.h>
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT11PIN A1

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

dht11 DHT11;
float temperature;
float humidity;
float amps = 0.0;
float watts = 0.0;


void setup()
{
  Serial.begin(9600);

  lcd.init();
}

void loop()
{
  Serial.println();
  measureTH();
  measureCurrentPower();
  displayOnLCD();
  //  sendIFTTTSMS();
  delay(5000);

}

void measureTH() {
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  humidity = (float)DHT11.humidity;
  Serial.println(humidity, 2);

  Serial.print("Temperature (F): ");
  temperature = ((float)DHT11.temperature) * 1.8 + 32;
  Serial.println(temperature, 2);

}

void measureCurrentPower() {

}

void displayOnLCD() {
  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Amps: ");
  lcd.print(amps);

  lcd.setCursor(0, 1);
  lcd.print("Watts: ");
  lcd.print(watts);

  lcd.setCursor(0, 2);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print("F ");
  lcd.print("H: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("Pwred By ElectroYeti");

}

void sendIFTTTSMS() {
}
