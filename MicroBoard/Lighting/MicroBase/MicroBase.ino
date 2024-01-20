// Working libraries include Dabbble 1.5.1, DFRobotDFPlayerMini 1.0.2, and EPS32 Analogwrite 3.4.2

#define CUSTOM_SETTINGS         //setting up Dabble
#define INCLUDE_GAMEPAD_MODULE  //setting up Dabble
#include <DabbleESP32.h>             //setting up Dabble
#include <Arduino.h>          //DFplayer setup
#include <HardwareSerial.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
HardwareSerial DFSerial(1);
#include "DFRobotDFPlayerMini.h" //DFplayer setup
DFRobotDFPlayerMini myDFPlayer;          //Start the DFPlayer
#include <pwmWrite.h>
Pwm pwm = Pwm();

void printDetail(uint8_t type, int value); //DFPlayer subroutine

int speedA = 0; //Set up the speed for forward
int speedB = 0; //Set up the speed for reverse
int Steer =82; //Set default position for steering


void setup()
{
  Serial.begin(115200);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("LunasMiniMouse");
  Serial.println("Hello, This is the Mini Mouse Code");
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  DFSerial.begin(9600,SERIAL_8N1,9,10,false);  // speed
  myDFPlayer.begin(DFSerial); //Initialize DFPlayer
  Serial.println("DF Player Started");
  myDFPlayer.volume(28);  //Set volume value. From 0 to 30,30 max
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); //----Set different EQ----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);  //----Set device we use SD as default----
  delay (1000); //Pause 1 second
  myDFPlayer.play(1);  //Play the first mp3 to confirm online
}

void loop()
{

  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
 

  float c = GamePad.getXaxisData();
  //Serial.print("x_axis: ");
  //Serial.print(c);
  //Serial.print('\t');
  float d = GamePad.getYaxisData();
  //Serial.print("y_axis: ");
  //Serial.println(d);
  //Serial.println();
  //Serial.print("KeyPressed: ");

if (d > -.05) {
 speedA= map(d,0,7,60,255);
 speedB=0;
}

if (d < .05) {
 speedB= map(-d,0,6,60,255);
 speedA=0;
}

if (d==0) {
speedA=0;
speedB=0;
}
Steer = map (c,-7,6,115,50);
pwm.writeServo(D2, Steer); //Steering is on pin 0
analogWrite(D0,speedA); //Drive forward is on pin 1 
analogWrite(D1,speedB); //Drive backwards is on pin 2 

  

 if (GamePad.isTrianglePressed())
 {
   Serial.println("Play first sound (Triangle)");
   myDFPlayer.play(1); //play mp3
   delay (75);
 }

if (GamePad.isCrossPressed())
 {
   Serial.println("Pause sounds(Cross)");
   myDFPlayer.pause();  //pause the mp3
 }

 if (GamePad.isCirclePressed())
 {
   Serial.println("Play sounds(Circle)");
   myDFPlayer.play(2);  //play mp3
   delay (75);
 }

 if (GamePad.isSquarePressed())
 {
   Serial.println("Play sounds(Square)");
   myDFPlayer.play(3);  //play mp3
   delay (75);
 }

  if (GamePad.isSelectPressed())
 {
   Serial.println("Play sounds(Select)");
   myDFPlayer.play(4);  //play mp3
   delay (75);
 }

  if (GamePad.isStartPressed())
 {
   Serial.println("Play sounds(Start)");
   myDFPlayer.play(5);  //play mp3
   delay (75);
 }
 
 
}
