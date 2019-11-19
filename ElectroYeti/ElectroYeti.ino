/**
    Copyright ElectroYeti Team. All Right Resereved.
*/

#include <Time.h>
#include <TimeLib.h>
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h" // Include Emon Library
#include <SoftwareSerial.h>

#define VOLT_CAL 148.7
#define CURRENT_CAL 62.6//1.1 //62.6

EnergyMonitor emon1; // Create an instance

SoftwareSerial ss(5, 6);//RX, TX

int delayMillis = 500;

#define DHT11PIN A1

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

dht11 DHT11;
float temperature;
float humidity;
float currentDraw = 0.0;
float powerUsage = 0.0;

//int data; //Initialized variable to store recieved data


void setup()
{
  ss.begin(9600);
  Serial.begin(9600);

  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(0, CURRENT_CAL);       // Current: input pin, calibration.

  lcd.init();
}

void loop()
{
  Serial.println();
  measureTH();
  measureCurrentPower();
  displayOnLCD();
  writeToNodeMCU();
  //  readSerialData();
  sendIFTTTSMS();
  delay(delayMillis);

}

void measureCurrentPower() {
  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out

  currentDraw = emon1.Irms;             //extract Irms into Variable
  float supplyVoltage = emon1.Vrms;                    //extract Vrms into Variable

  //make it fixed 120V for demo
  supplyVoltage = 120;

  Serial.print("Voltage: ");
  Serial.println(supplyVoltage);

  Serial.print("Current: ");
  Serial.println(currentDraw);

  powerUsage = currentDraw * supplyVoltage;
  Serial.print("Watts: ");
  Serial.println(powerUsage);
  Serial.println("");
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

void displayOnLCD() {
  lcd.init(); // initialize the lcd
  lcd.init();

  // Print current & power usage to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Amps: ");
  lcd.print(currentDraw);

  lcd.setCursor(0, 1);
  lcd.print("Watts: ");
  lcd.print(powerUsage);

  // Print temperature & humidity to the LCD.
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

void writeToNodeMCU() {
  Serial.println("Trying to write to node mcu...");
  if (ss.available() > 0)
  {
    Serial.println("Power Usage: " + String(powerUsage));

    int pw = (int)powerUsage;//write data conversion logic
    if (pw < 250) {

    }
    pw = 250;
    Serial.print("Writing power usage to node mcu...");
    Serial.println(pw);

    ss.write(pw);
  }

}

void readSerialData() {
  //  data = Serial.read(); //Read the serial data and store it
  //  Serial.println("Data read from ESP8266: " + String(data));
}

void sendIFTTTSMS() {
}
