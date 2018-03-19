/*
 * ArduinoSafeBoxProject.ino
 * version: 1.0
 * 
 * Arduino Safe Box Project (ASBP)
 * 
 * Purpose of the project is to set password initially,
 * save it to eeprom memory and control lock.
 * 
 * Code distributed under GNU GPL v3.0
 * Copyright(c) Krzysztof Stezala, 2017
 *  
 * LiquidCrystal_I2C and its dependencies 
 * are distributed under GNU GPL v3.0
 * Copyright (c) Tod E. Kurt, 2010-2015
 */

#include <EEPROMex.h>
#include <EEPROMVar.h>

#include <Servo.h>
#include <Wire.h> 

#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>
  
/*
   LCM1602 & Arduino Uno
   VCC - > 5 V
   GND - GND
   SCL -> A5
   SDA -> A4
*/
Servo myservo;
const int servoPin = 10;

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'*', '0', '#'},
  {'9', '8', '7'},
  {'6', '5', '4'},
  {'3', '2', '1'}
};
byte rowPins[rows] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 8, 7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void ServoSetup(){
  myservo.attach(10);
  myservo.write(0);
}

void LcdSetup(){
  lcd.begin(16, 2);  // Inicjalizacja LCD 2x16
  lcd.backlight(); // zalaczenie podwietlenia
  lcd.setCursor(0, 0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.clear();
}

boolean firstRun = true;
boolean passwordGuessed = false;

byte firstRunAddress = 0;

boolean CheckIfFirstRun(){
  
  return firstRun;
}
char password[4];
int address[4]= {1,2,3,4};
byte output = 0;

boolean SavePassword(){
  boolean ifCorrect;
  for(int i=0;i<4;i++)
  {
    password[i] = keypad.waitForKey();
    byte node = (byte)password[i];
    EEPROM.write(address[i], node);
  }
  Serial.begin(9600);
  for(int i=0;i<4;i++){
    output = EEPROM.read(address[i]);
    Serial.print(" ");
    Serial.print(output);
  }
  
  
  
  Serial.println();
  
  return ifCorrect;
}


void setup()
{  
  //lcd.print("Input new password");
  //if(SavePassword()){
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("Password set");
  //}
  SavePassword();
  

  if(passwordGuessed){
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Success");
      myservo.write(90);
  }
}

void loop()
{
  
}




