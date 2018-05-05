/*
 * License: GPL V3
 * Code by Chris Dagher for ENGR 280 Spring 2018
 *    This code will send commands to the rover based on inputs from joysticks and buttons.
 *    Joysticks connected to the arduino with pull-up resistors will pull to ground, signaling
 *    the arduino to send an integer command over the rx and tx lines.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


LiquidCrystal_I2C lcd(0x27,20,4);  // initialize LCD display

#define lStick1 4
#define lStick2 3
#define rStick1 6
#define rStick2 5
#define button  7

bool lStick1State = true;
bool lStick2State = true;
bool rStick1State = true;
bool rStick2State = true;
bool buttonState = true;

int serial = 5;

int addr = 0;  // eeprom address to restore number of presses
int count = 0;  // initialize the button press value

void setup() {
  count = EEPROM.read(addr);  // read eeprom for button presses
  lcd.init();                         ////////////////////////////////////
  lcd.backlight();                    // Print main text to the LCD
  lcd.setCursor(0,0);                 // 
  lcd.print("  Push the button!  ");  // 
  lcd.setCursor(0,1);                 // 
  lcd.print("--------------------");  // 
  lcd.setCursor(1,2);                 // 
  lcd.print(count);                   // 
  lcd.setCursor(0,3);                 // 
  lcd.print(" Presses & counting!");  ////////////////////////////////////
  pinMode(lStick1, INPUT_PULLUP);
  pinMode(lStick2, INPUT_PULLUP);
  pinMode(rStick1, INPUT_PULLUP);
  pinMode(rStick2, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600); // open serial communication
  while (!Serial) {
    ; // wait for serial to become available
  }
}

void loop() {
  userRead();
  inputProcess();
  communicate();
  //delay(500);  // used for debugging
}

void userRead() {  // read user inputs
  lStick1State = digitalRead(lStick1);
  lStick2State = digitalRead(lStick2);
  rStick1State = digitalRead(rStick1);
  rStick2State = digitalRead(rStick2);
  buttonState = digitalRead(button);
/*
  Serial.print("lStick1State: ");
  Serial.println(lStick1State);
  delay(250);
  Serial.print("lStick1State: ");
  Serial.println(lStick2State);
  delay(250);
  Serial.print("rStick1State: ");
  Serial.println(rStick1State);
  delay(250);
  Serial.print("rStick2State: ");
  Serial.println(rStick2State);
  delay(250);
  Serial.print("buttonState: ");
  Serial.println(buttonState);
  delay(250);
*/
  return;
}

void inputProcess() {  // decide what command to send to the rover
  if (lStick1State == false) {
    if (rStick1State == false) {
      serial = 1;
    }
    else if (rStick2State == false) {
      serial = 3;
    }
    else if (rStick2State == true) {
      serial = 7;
    }
  }
  else if (lStick2State == false) {
    if (rStick2State == false) {
      serial = 2;
    }
    else if (rStick1State == false) {
      serial = 4;
    }
    else if (rStick1State == true) {
      serial= 8;
    }
  }
  else {
    serial = 5;
  }
  if (buttonState == false) {
    buttonRun();
  }
  return;
}

void inputProcess2() {  // same as other, but ignores button presses
                        // useful to prevent infinite loop with the button
  if (lStick1State == false) {
    if (rStick1State == false) {
      serial = 1;
    }
    else if (rStick2State == false) {
      serial = 3;
    }
    else if (rStick2State == true) {
      serial = 7;
    }
  }
  else if (lStick2State == false) {
    if (rStick2State == false) {
      serial = 2;
    }
    else if (rStick1State == false) {
      serial = 4;
    }
    else if (rStick1State == true) {
      serial= 8;
    }
  }
  else if (lStick1State == false) {
    serial = 7;
  }
  else if (lStick2State == false) {
    serial = 8;
  }
  else if (rStick1State == false) {
    serial = 9;
  }
  else if (rStick2State == false) {
    serial = 11;
  }
  else {
    serial = 5;
  }
  return;
}

void communicate() {  // sends serial commands
  Serial.println(serial);
  serial = 5;
  return;
}

void buttonRun() {  // keeps rover moving while button is pressed
                    // also ignores button being held
  lcdWrite();
  while (buttonState == false) {
    userRead();
    inputProcess2();
    communicate();
  }
  return;
}

void lcdWrite() {  // write the button count to the LCD
  count += 1;
  lcd.setCursor(1,2);
  lcd.print(count);
  EEPROM.write(addr, count);  // store the new count to EEPROM
}

