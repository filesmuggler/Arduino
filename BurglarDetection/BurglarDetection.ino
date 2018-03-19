/*
 * Arduino Burglar Detection System (ABDS)
 * by Krzysztof Stężała
 * Copyright(c) 2017 Krzysztof Stężała. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * DISCLAIMER:
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * -->> INSTRUCTION <<--
 * FIRST RUN:
 * To run program properly you need to asign the value of the alarm mode by uncommenting the correct line, where:
 */
 const int MODE = 0; // for developer mode - provides extra info in the serial output; silent alarm, resetted by button;
 // const int MODE = 1; // for watchman mode - provides standard alarm; could be reset by the button;
 // const int MODE = 2; // for silent mode - provides alarm only through serial; could be reset by the button;
 

// Global variables
int buzzerPin = 7; //Buzzer's positive terminal is connected to digital PIN 9
int sensorPin = 2; //PIR's sensor reading terminal is connected to digital PIN 2
int lightPin = 8; //LED's terminal connected to PIN 8
int buttonPin = 9; //Button's terminal connected to PIN 7
int calibrationTime; //Safe time for the user to move away from the sensor 
String chosenMode; //Holds info about chosen mode

// Lights up the LED when intruder detected
void LightUp(){
  digitalWrite(lightPin,HIGH);
}

//Sounds up the buzzer when intruder detected
void SoundUp(){
  digitalWrite(buzzerPin,HIGH);
}

//Turns off the alarm after pressing the button
void TurnOffAlarm(){
  digitalWrite(lightPin,LOW);
  digitalWrite(buzzerPin,LOW);
}

//Rises the alarm and holds it till button is not pressed
void SignalIntrusion(){
  while(digitalRead(buttonPin)!=1){
    LightUp();
    SoundUp();
    Serial.println("Intruder detected!");
  }
  TurnOffAlarm();
}

//Scans the area for movement
void DetectIntruder(const int m){
  if(digitalRead(sensorPin)==HIGH){
    SignalIntrusion();
  }
}

//Sets up all required settings
void SetupDetector(){
  
  //putting pins in correct mode, so they do not have random values by accident
  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin,LOW);
  pinMode(sensorPin,INPUT);
  pinMode(buttonPin,INPUT);
  digitalWrite(buttonPin,LOW);
  pinMode(lightPin,OUTPUT);
  digitalWrite(lightPin,LOW);
  
  // if compiler says <'MODE' was not declared in this scope>, look up to the instruction :)
  switch (MODE){
    case 0:
    {
      chosenMode = "DEVELOPER MODE";
      calibrationTime = 5;
      break;
    }
    case 1:
    {
      chosenMode = "STANDARD MODE";
      calibrationTime = 30;
      break;
    }
    case 2:
    {
      chosenMode = "SILENT MODE";
      calibrationTime = 30;
      break;
    }
  }
  //Starting serial communication
  Serial.begin(9600);
  for(int i=0;i<calibrationTime;i++)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Calibration complete");
  Serial.print("Sensor in ");
  Serial.println(chosenMode);
  delay(1000); 
}

void setup() {
  SetupDetector();
}

void loop() {
  DetectIntruder(MODE);
}

