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

int xLock = 0; // this is for throrle lock
int xLimit = 0; // this is for throrle limiter
long longPressTime = 1000; //1000 is 1 second
bool lockSetButtonState = 1;
bool lockSetButtonLastState = 1;

const bool initAutoSendState = true; 
Adafruit_7segment matrix = Adafruit_7segment();

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

  matrix.begin(0x70);

  Joystick.begin();
  //Serial.begin(115200);

}

int buttonPins[2] = {14,15};
int lastButtonState[2] = {0,0};


void loop() {
  // put your main code here, to run repeatedly:

  // Throttle lock controls and value
  lockSetButtonState = digitalRead(4);
  if (lockSetButtonState != lockSetButtonLastState) {
    if (lockSetButtonState == HIGH) {
      // Setting the lock value
      xLock = 1024 - analogRead(A3);
    }
  }
  lockSetButtonLastState = lockSetButtonState;
  // increase throttle lock
  if (xLock > 0) {
    if (digitalRead(7) == LOW) {
      xLock++;
    }
    if (digitalRead(8) == LOW) {
      xLock--;
    }
  }
  //press plus and minus to clear
  if ((digitalRead(7) == LOW) && (digitalRead(8) == LOW)) {
    xLock = 0;
  }

  if (digitalRead(5) == LOW) {
    xLimit = 1024 - analogRead(A3);
  }
  // increase throttle lock
  if (xLimit > 0) {
    if (digitalRead(6) == LOW) {
      xLimit++;
    }
    if (digitalRead(9) == LOW) {
      xLimit--;
    }
  }
  //long press minus to clear
  if ((digitalRead(6) == LOW) && (digitalRead(9) == LOW)) {
    xLimit = 0;
  }



  RzAxis_ = analogRead(A1);  
  Joystick.setRzAxis(1024 - RzAxis_);  
  RyAxis_ = analogRead(A2);  
  Joystick.setRyAxis(1024 - RyAxis_);
  if ((xLock > 0) && (digitalRead(10) == LOW)) {
    RxAxis_ = xLock;
  }else if ((xLimit > 0) && (digitalRead(16) == LOW)) {
    RxAxis_ = min(1024 - analogRead(A3),xLimit);
  }else{
    RxAxis_ = (1024 - analogRead(A3));
  }
  Joystick.setRxAxis(RxAxis_);


  // Pass through the Ignition and Starter to the Joystick
  // We will be using the other buttons for our own controlls
  for (int index = 0; index < 2; index++){
    int currentButtonState = !digitalRead(buttonPins[index]);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
  }

  //matrix.blinkRate(1);
  //matrix.drawColon(true);
  matrix.print(RxAxis_);
  matrix.writeDisplay();
}
