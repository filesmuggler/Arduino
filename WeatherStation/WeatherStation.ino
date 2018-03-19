/*
 * ArduinoWeatherStationProject.ino
 * version: 1.0
 * 
 * Arduino Weather Station Project (AWSP)
 * 
 * Purpose of the project is to collect data about
 * temperature and humidity of air, and moisture of the soil
 * using 3 sensors.
 * 
 * Code distributed under GNU GPL v3.0
 * Copyright(c) Krzysztof Stezala, 2017
 *  
 * LiquidCrystal_I2C and its dependencies 
 * are distributed under GNU GPL v3.0
 * Copyright (c) Tod E. Kurt, 2010-2015
 * 
 */

// libraries neccessary to run the program
// if something does not work, make sure that
// you installed all libraries specified in the instruction
#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define LCD_ADDRESS 0x3F                                      // LCD screen address over I2C
#define DHT_PIN 8                                             // DHT11 signal pin defined as 8
#define DAL_PIN 3                                             // Dallas Temp Sensor pin defined as 3
#define SOIL_A_PIN A2                                         // Soil moisture sensor analog pin defined as A2

LiquidCrystal_I2C lcd(LCD_ADDRESS,2,1,0,4,5,6,7,3,POSITIVE);  // LCD screen initialization
SimpleDHT11 dht11;                                            // creating global variable of type SimpleDHT11 to handle DHT11 signal input

byte dhtTemperature = 0;                                      // set temperature from DHT11 to zero; make variable global
byte dhtHumidity = 0;                                         // set humidity from DHT11 to zero; make variable global
int dhtError = SimpleDHTErrSuccess;                           // set dhtError to constant value SimpleDHTErrSuccess

OneWire oneWire(DAL_PIN);                                     // creating global variable of type OneWire with DAL_PIN as argument
DallasTemperature sensors(&oneWire);                          // attaching OneWire object to DallasTemperature sensors, to get data

double avTemp = 0;                                            // average temperature variable
int soWater = 0;                                              // water level got from soil moisture sensor
String waterLevel;                                            // water level presented as string

void LcdSetup(){                                              // LcdSetup() prepares the LCD screen at the beginning
  lcd.begin(16,2);
  delay(250);
  lcd.noBacklight();
  delay(250);
  lcd.backlight();
  lcd.setCursor(0,0);
}

void SensorSetup(){                                           // SensorSetup() prepares sensors to get data and print error if something goes wrong
  pinMode(SOIL_A_PIN,INPUT);
  if((dhtError = dht11.read(DHT_PIN, &dhtTemperature, &dhtHumidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.begin(9600);
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(dhtError);
    delay(1000);
  }
}

double ABS(double a){                                         // ABS(double a) is a simple function to return absolute value of "a"
  if (a<0){
    return -a;
  }
  else{
    return a;
  }
}

void setup() {                                                // regular arduino setup() function definition
  LcdSetup();
  SensorSetup();
}

void loop() {                                                 // regular arduino loop() function definition
  dht11.read(DHT_PIN, &dhtTemperature, &dhtHumidity, NULL);   // read data from DHT11 sensor
  lcd.print("Temp: Hum: Soil:");                              // print headlines to LCD
  lcd.setCursor(0,1);                                         // set cursor to the next line
  
  sensors.requestTemperatures();                              // temperature request from Dallas sensor
  double dal = sensors.getTempCByIndex(0);                    // getting exact value from Dallas sensor
  avTemp = (dal+dhtTemperature)/2;                            // calculating average value for two sensors
  soWater = analogRead(SOIL_A_PIN);                           // obtaining data from soil moisture sensor
  if(soWater > 900){                                          // depending on the value returned by the sensor the correct label is attached
    waterLevel = "dry";
  }
  else if(soWater>750 && soWater<=900){
    waterLevel = "low";
  }
  else if(soWater>400 && soWater<=750){
    waterLevel = "med";
  }
  else if(soWater>300 && soWater<=400){
    waterLevel = "wet";
  }
  
  
  lcd.print((int)avTemp);                                     // printing all data to the LCD screen and setting cursor back to (0,0) position
  lcd.print("^C  ");
  lcd.print(dhtHumidity);
  lcd.print("%  ");
  lcd.print(waterLevel);
  delay(1000);                                                // delay is used for convience reason, no needed explicitly
  lcd.clear();
  lcd.setCursor(0,0);
}

