/*
  Modified control software to allow use of an ESP32 board in conjusction with Dabble
    
  ///////////////////////////////////////////////////////////////////////////////////////
  //Connections
  ///////////////////////////////////////////////////////////////////////////////////////
  ESP32
  Servo1 = 7  Left drive
  Servo2 = 8  Right drive
  Servo3 = 9  Dome
  DFPlayer Rx = 0
  DFPlayer Tx = 1
*//////////////////////////////////////////////////////////////////////////////////////

//
//Drive motor mixing code taken from        https://www.impulseadventure.com/elec/robot-differential-steering.html
//

#include <Arduino.h>
#include <pwmWrite.h>
Pwm pwm = Pwm();
#include <HardwareSerial.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <DabbleESP32.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include "DFRobotDFPlayerMini.h"

  HardwareSerial DFSerial(1);
  DFRobotDFPlayerMini myDFPlayer;
  bool PlayLabel = false;
  long Player_Timer = 0;
  int sound_called = 0;

int servoPin1 = D5;  //Left Drive
int servoPin2 = D8;  //Right Drive
int servoPin3 = D9;  //Dome


 long previousMillis = 0;
 int timeout = 1000;
 
unsigned long elapsedTime; // time current

byte receivedChar;        // serial data received from bluetooth device

int x_drive = 90;                     // value from joystick
int y_drive = 90;                     // value from joystick

// Differential Steering Variables  ////////////////////////////////////////
// INPUTS
int     nJoyX;              // Joystick X input                     (-128..+127)
int     nJoyY;              // Joystick Y input                     (-128..+127)

// OUTPUTS
int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
int     nMotMixR;           // Motor (right) mixed output           (-128..+127)

// CONFIG
// - fPivYLimt  : The threshold at which the pivot action starts
//                This threshold is measured in units on the Y-axis
//                away from the X-axis (Y=0). A greater value will assign
//                more of the joystick's range to pivot actions.
//                Allowable range: (0..+127)
float fPivYLimit = 32.0;
      
// TEMP VARIABLES
float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
int     nPivSpeed;      // Pivot Speed                          (-128..+127)
float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )

///////////////////////////////////////////////////////////////////////////////


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("MyVashonDroid");       //set bluetooth name of your device

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  DFSerial.begin(9600, SERIAL_8N1, D0, D1);  // speed, type, RX, TX
  myDFPlayer.begin(DFSerial); //Initialize DFPlayer


  Serial.println("Hello, This is the Vashon Droid Code!");
  myDFPlayer.play(1); 

/****************************  Setup DFPlayer ************************************/
/***************************************************************************/
  
  //myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.volume(25);  //Set volume value (0~30).
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

/***************************************************************************/
/***************************************************************************/

}

void loop() {
// put your main code here, to run repeatedly:

 // called functions////////////////////////////////////////////////////////////////////////

  read_dabble();
  drive_servos();
  Dome_servo();
   if (GamePad.isTrianglePressed())
   {
   Serial.println("Triangle");
   myDFPlayer.play(2); 
   delay(100);
   }
   if (GamePad.isCrossPressed())
   {
	 Serial.println("Cross");
   myDFPlayer.play(3);
   delay(100);
   }
   if (GamePad.isSelectPressed())
   {
   Serial.println("Select");
   myDFPlayer.play(4);  
   delay(100);
   }
   if (GamePad.isStartPressed())
   {
   Serial.println("Start");
   myDFPlayer.play(5);
   delay(100);
   }
  
}

void read_dabble() {
// Read the input from the Dabble controller
   Dabble.processInput();
  }
  
void drive_servos(){
	
// Pull Joystick Values
   x_drive = GamePad.getXaxisData();
   y_drive = GamePad.getYaxisData();
   nJoyX = map(x_drive,6,-7,-127,127);  // Map -7 to 6 to -127, 127
   nJoyY = map(y_drive,-6,7,127,-127);  // Map -6 to 7 to -127, 127
   if (x_drive == 0) {nJoyX = 0;}       // Zero out mapping of joystick centered
   if (y_drive == 0) {nJoyY = 0;}       // Zero out mapping of joystick centered
  
// Calculate Drive Turn output due to Joystick X input
if (nJoyY >= 0) {
  // Forward
  nMotPremixL = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
  nMotPremixR = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
} else {
  // Reverse
  nMotPremixL = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
  nMotPremixR = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
}

// Scale Drive output due to Joystick Y input (throttle)
nMotPremixL = nMotPremixL * nJoyY/128.0;
nMotPremixR = nMotPremixR * nJoyY/128.0;

// Now calculate pivot amount
// - Strength of pivot (nPivSpeed) based on Joystick X input
// - Blending of pivot vs drive (fPivScale) based on Joystick Y input
nPivSpeed = nJoyX;
fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);

// Calculate final mix of Drive and Pivot
nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);

  // Do something with x and y
nMotMixL = map(nMotMixL,127,-127,0,180);  // Map from -127 to 127 to servo values of 0-180
nMotMixR = map(nMotMixR,127,-127,180,0);  // Map from -127 to 127 to servo values of 0-180
  pwm.writeServo(servoPin1, nMotMixL);  // Move the Left Servo
  pwm.writeServo(servoPin2, nMotMixR);  // Move the Right Servo 
  //Serial.print("L:");
  //Serial.print(nMotMixL);
  //Serial.print(" R:");
  //Serial.println(nMotMixR);
}

void  Dome_servo(){
  if (GamePad.isSquarePressed())  //Turn Dome Left
   {
   Serial.println("Square");
   pwm.writeServo(servoPin3, 150);
   }
  if (GamePad.isCirclePressed())  //Turn Dome Right
   {
   Serial.println("Circle");
   pwm.writeServo(servoPin3, 30);
   }
  if (!GamePad.isCirclePressed() && !GamePad.isSquarePressed())
   {
   pwm.writeServo(servoPin3, 90.5);  
   }
}

 
