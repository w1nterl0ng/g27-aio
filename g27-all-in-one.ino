#include <Joystick.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// X = Throttle
// Y = Brake
// Z = Clutch

Joystick_ Joystick;

int RxAxis_ = 0;                    
int RyAxis_ = 0;  
int RzAxis_ = 0; 

const bool initAutoSendState = true; 


void setup() {
  // put your setup code here, to run once:
  pinMode(4, INPUT_PULLUP); // Button Black Left
  pinMode(5, INPUT_PULLUP); // Button Black Right
  pinMode(6, INPUT_PULLUP); // Button Yellow Right
  pinMode(7, INPUT_PULLUP); // Button Yellow Left
  pinMode(8, INPUT_PULLUP); // Button Green Left
  pinMode(9, INPUT_PULLUP); // Button green Right
  pinMode(10, INPUT_PULLUP); // SPST Left
  pinMode(14, INPUT_PULLUP); // Ignition
  pinMode(15, INPUT_PULLUP); // Starter
  pinMode(16, INPUT_PULLUP); // SPST Right

  Joystick.begin();
  Serial.begin(115200);

}

int buttonPins[10] = {4,5,6,7,8,9,10,14,15,16};
int lastButtonState[10] = {0,0,0,0,0,0,0,0,0,0};


void loop() {
  // put your main code here, to run repeatedly:
  RzAxis_ = analogRead(A1);  
  Joystick.setRzAxis(1024 - RzAxis_);  
  RyAxis_ = analogRead(A2);  
  Joystick.setRyAxis(1024 - RyAxis_);  
  RxAxis_ = analogRead(A3);
  Joystick.setRxAxis(1024 - RxAxis_);

  for (int index = 0; index < 10; index++){
    int currentButtonState = !digitalRead(buttonPins[index]);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
    Serial.print(lastButtonState[index]);
  }
  // print pedal values
  Serial.print(1024 - RzAxis_);
  Serial.print(" ");
  Serial.print(1024 - RyAxis_);
  Serial.print(" ");
  Serial.print(1024 - RxAxis_);
 
  Serial.println();
}
