#include <Adafruit_NeoPixel.h>
#define PINE D7     // LED Panel 1
#define PINL D8     // LED Panel 2
#define NUM_LEDS 206
#include <Arduino.h>          //DFplayer setup
#include <HardwareSerial.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
HardwareSerial DFSerial(1);
#include "DFRobotDFPlayerMini.h" //DFplayer setup
DFRobotDFPlayerMini myDFPlayer;          //Start the DFPlayer


// pins.  A0-button panel 1, A1-button panel 2, A2-button panel 3, A3-slow fade 1, D4-slow fade 2, D5-flash 1, D6-flash 2, D7-LED Panel 1, D8-LED Panel 2

// ------------------------------------------
int fun;          // variable "fun" is the function to run, set by serial command received
int loopCount;
boolean flash =1;
int flash1;
int flash2;
int flash3;
int flash4;
int dimSetting1;
int dimSetting2;
int b1;
int b2;
int b3;

#define loopNum 9000

//  LEDs strips triggered by serial command like Serial3.print("0")

//  0 = LEDs off
//  1 = Red
//  2 = White
//  3 = Blue
//  4 = Red chase
//  5 = Pulse Red
//  6 = Rainbow fast
//  7 = Purple fade in out
//  8 = Rainbow slow
//  9 = Red wipe, black wipe
//  A = Circling red / White
//  B = Rainbow
//  C = Meteor rain
//  D = Rainbow chase
//  E = Blue chase
//  F = Color wipe
//  G = Running lights
//  H = Fade in/out
//  I = Snow sparkle
//  J = Cylon bounce
//  K = New kitt
//  L = Cirq
//  M = All pixel Rainbow
//  N = All pixels Rainbow faster
//  O = Blue cylon
//  P = White fast chase
//  Q = Snow sparkle blue
//  R = 
//  S = 
//  T = 
//  U = 
//  V = 
//  W = 
//  X = 
//  Y = 
//  Z = 
//  a = 
//  b = 
//  c = 
//  d = 
//  e = 
//  f = 
//  g = 
//  h = 
//  i = 
//  j = 
//  k = 
//  l = 
//  m = 
//  n = 
//  o = 
//  p = 
//  q = 
//  r = 
//  s = 
//  t = 
//  u = 
//  v = 
//  w = 
//  x = 
//  y = 
//  z = 
//  ! = 
//
// ------------------------------------------
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(206, PINE, NEO_GRB + NEO_KHZ800);// #define 32 LEDs for strip
Adafruit_NeoPixel stripLogic = Adafruit_NeoPixel(206, PINL, NEO_GRB + NEO_KHZ800);// #define 16 LEDs for strip

// Here is where you can put in your favorite colors that will appear!
// just add new {nnn, nnn, nnn}, lines. They will be picked out randomly
//                          R   G   B
uint8_t myFire[][3] = {{232, 100, 255},   // purple
                        {200, 200, 20},   // yellow 
                        {30, 200, 200},   // blue
                        {237,159,176},    // pink
                        {255, 0, 0},      // red
                        {108, 87,229},     // Dark blue
                        {162,211,172},     // Lt Green
                          };

uint8_t myColors[][3] = {{232, 100, 255},   // purple
                        {200, 200, 20},   // yellow 
                        {30, 0, 0},       // light red
                        {237,100,100},    // some red
                        {255, 0, 0},      // red
                        {150, 20,20},     // Med red
                        {200,5,5},        // Bright red
                          };

#define FAVCOLORS sizeof(myColors) / 3
#define FIRECOLORS sizeof(myFire) / 3

void setup() {
  pinMode (A3, OUTPUT);
  pinMode (D4, OUTPUT);
  pinMode (D5, OUTPUT);
  pinMode (D6, OUTPUT);
  fun = 1;
  Serial.begin(115200); // Hardware serial for receiving serial input
  strip.begin();  	    // Panel1
  stripLogic.begin();   // Panel2
  
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
 
void loop() {
     //handleSerial();   // Reads serial input looking for commands
     if (loopCount >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
	   handleButtons(); }
     if (fun >13) {fun=1;}
     if (fun <0) {fun=12;}
     loopCount++;
     if (fun == 0 ) // Turn all lights off
     {
     strip.clear(); 
     strip.show();
     stripLogic.clear(); 
     stripLogic.show();
	   analogWrite(A3, 0);
	   analogWrite(D4, 0);
	   digitalWrite(D5,LOW);
	   digitalWrite(D6,LOW);
     flash = 0;
     fun = 13;
     }
     if (fun == 1 ) //  Red and buttons on
     { 
     strip.clear(); 
	   stripLogic.clear();
     strip.fill(strip.Color(255, 0, 0),0,206); 
     stripLogic.fill(strip.Color(255, 0, 0),0,206); 
	   strip.show();
	   stripLogic.show();
	   flash = 1;
	   fun = 13;
     }
     if (fun == 2 ) // White and buttons on
     {
     strip.clear(); 
	   stripLogic.clear();
     strip.fill(strip.Color(255, 255, 255),0,206); 
     stripLogic.fill(strip.Color(255, 255, 255),0,206); 
	   strip.show();
	   stripLogic.show();
	   flash = 1;
	   fun = 13;
     }
     if (fun == 3 ) // Blue and buttons on
     {
	   strip.clear(); 
	   stripLogic.clear();
     strip.fill(strip.Color(0, 0, 255),0,206); 
     stripLogic.fill(strip.Color(0, 0, 255),0,206); 
	   strip.show();
	   stripLogic.show();
	   flash = 1;
	   fun = 13;
     }
     if (fun == 4 ) //  Fade Red, White, Blue
     {
     FadeInOut(255, 0, 0,50); 
	   FadeInOut(255, 255, 255,50); 
     FadeInOut(0, 0, 255,50); 
     }    
     if (fun == 5 ) // Fade in out red
     {
     FadeInOut(255, 0, 0,40); 
     }
     if (fun == 6 ) // Fade in out white
     {
     FadeInOut(255, 255, 255,40); 
     }
     if (fun == 7 ) // Fade in out blue
     {
	   FadeInOut(0,0,255,40);
     }
     if (fun == 8 ) // Rainbow slower
     {
     rainbow(50); 
     }
     if (fun == 9 ) // Rainbow faster
     {
	   rainbowstrip(50); 
     }      
     if (fun == 10 ) // Rainbow individual slow
     {
     rainbowCycle(20); 
     }
     if (fun == 11 ) //meteor Rain
     {
     meteorRain(255,0,0,40, 64, true, 25);
     }
     if (fun == 12 ) // color wipe
     {
     colorWipe(0,0,0, 20);
     colorWipe(255,0,0, 45);
     }

  } // end loop


void handleButtons() {
// b1=analogRead(A0);
b2=analogRead(A1);
b3=analogRead(A2);

if ((b2 >= 850) && (b2 <= 1400)) {
	fun=0;
  myDFPlayer.play(2);
	delay(250);
	}
if ((b2 >= 1600) && (b2 <= 2199)) {
	fun=1;
	myDFPlayer.play(3);
  delay(250);
	}
if ((b2 >= 3000) && (b2 <= 3200)) {
	fun=2;
	myDFPlayer.play(4);
  delay(250);
	}
if (b2 >= 4000) {
	fun=3;
	myDFPlayer.play(5);
  delay(250);
	}
 //Serial.println(b2);

if ((b3 >= 1600) && (b3 <= 1800)) {
  fun=4;
  myDFPlayer.play(6);
  delay(250);}
if ((b3 >= 2400) && (b3 <= 3300)) {
  if (fun < 4) {fun=3;}
  if (fun >13) {fun=4;}
  fun++;
  myDFPlayer.play(7);
  delay(250);
  }
if (b3 >= 3900) {
    fun--;
    if (fun < 4) {fun=12;}
  myDFPlayer.play(8);
  delay(250);
  }
  
if (flash==1) {flashR1();}
if (flash==1) {flashR2();}
if (flash==1) {flashR3();}
if (flash==1) {flashR4();}
loopCount = 0; 

}

void rainbow(uint8_t wait) {
  uint16_t i, j;

    for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((i+j) & 255));
     stripLogic.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    stripLogic.show();
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
      handleButtons(); }
    loopCount=loopCount+(wait*35); 
    delay(wait);
  }
}

void rainbowstrip(uint8_t wait) {
  int i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((j) & 255));
     stripLogic.setPixelColor(i, Wheel((j) & 255));
    }
     strip.show();
     stripLogic.show();
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
       handleButtons(); }
    loopCount=loopCount+(wait*35); 
    delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    stripLogic.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    stripLogic.show();
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
    handleButtons(); }
    loopCount=loopCount+(wait*45); 
    delay(wait);
  }
}

//Random flashing to off
void flashRandom(int wait, uint8_t howmany) {
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random favorite color!
    int c = random(FAVCOLORS);
    int red = myColors[c][0];
    int green = myColors[c][1];
    int blue = myColors[c][2]; 
 
    // get a random pixel from the list
    int j = random(strip.numPixels());
    
    // now we will 'fade' it in 3 steps
    for (int x=0; x < 3; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      strip.setPixelColor(j, strip.Color(r, g, b));
	  stripLogic.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
	  stripLogic.show();
    }
    // & fade out in 3 steps
    for (int x=3; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      strip.setPixelColor(j, strip.Color(r, g, b));
      stripLogic.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
	    stripLogic.show();

    }
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
      handleButtons(); }
    loopCount=loopCount+(SpeedDelay*20);
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
    uint32_t oldColor;
    uint8_t r, g, b;
    // int value;
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);
    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    strip.setPixelColor(ledNo, r,g,b);
	stripLogic.setPixelColor(ledNo, r,g,b);
}



byte * Wheel2(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}




void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
       handleButtons(); }
      loopCount=loopCount+(SpeedDelay*20);
      delay(SpeedDelay);
  }
}



void FadeInOut(byte red, byte green, byte blue,int wait){
    float r, g, b;
     
    for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
       handleButtons(); }
    loopCount=loopCount+(wait*20);
    delay(wait);
  }
     
  for(int k = 255; k >= 0; k=k-5) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
    if ((loopCount) >= loopNum) { // this created a non-blocking delay for flashing the lights or reading the buttons
       handleButtons(); }
    loopCount=loopCount+(wait*20);
  }
}


void showStrip() {
  strip.show();
  stripLogic.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
   stripLogic.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void flashR1()
{
  if (flash1 <128) {
  dimSetting1=dimSetting1+2;
  analogWrite(A3, dimSetting1);
  }
  if (flash1 >127) {
  dimSetting1=dimSetting1-2;
  analogWrite(A3, dimSetting1);
  }
  flash1++;
  if (flash1 > 254) {
    flash1 = 1;
    dimSetting1=1;}
}


void flashR2()
{
  if (flash2 <250) {
  dimSetting2++;
  analogWrite(D4, dimSetting2);
  }
  if (flash2 >249) {
  dimSetting2--;
  analogWrite(D4, dimSetting2);
  }
  flash2++;
  if (flash2 > 500) {
    flash2 = 1;
    dimSetting2=1;}
}

void flashR3()
{
  if (flash3 == 1) {
  digitalWrite(D5,LOW);
  }
  if (flash3 == 2) {
  }
  if (flash3 == 3) {
  }
  if (flash3 == 4) {
  }
  if (flash3 == 5) {
  }
  if (flash3 == 6) {
  }
  if (flash3 == 7) {
  }
  if (flash3 == 8) {
  }
  if (flash3 == 9) {
  }
  if (flash3 == 10) {
  }
  if (flash3 == 11) {
  }
  if (flash3 == 12) {
  }
  if (flash3 == 13) {
  }
  if (flash3 == 14) {
  }
  if (flash3 == 15) {
  digitalWrite(D5,HIGH); 
  }
  if (flash3 == 16) {
  }
  if (flash3 == 17) {
  }
  if (flash3 == 18) {
  }
  if (flash3 == 19) {
  }
  if (flash3 == 20) {
  }
  if (flash3 == 21) {
  }
  if (flash3 == 22) {
  }
  if (flash3 == 23) {
  }
  if (flash3 == 24) {
  }
  if (flash3 == 25) {
  }
  if (flash3 == 26) {
  }
  if (flash3 == 27) {
  }
  if (flash3 == 28) {
  }
  if (flash3 == 29) {
  }
  if (flash3 == 30) {
  }
  flash3++;
  if (flash3 > 40) {
  flash3 = 1;
  }
}

void flashR4()
{
  if (flash4 == 1) {
  }
  if (flash4 == 2) {
  digitalWrite(D6,HIGH);
  }
  if (flash4 == 3) {
  }
  if (flash4 == 4) {
  }
  if (flash4 == 5) {
  }
  if (flash4 == 6) {
  }
  if (flash4 == 7) {
  }
  if (flash4 == 8) {
  }
  if (flash4 == 9) {
  }
  if (flash4 == 10) {
  }
  if (flash4 == 11) {
  }
  if (flash4 == 12) { 
  }
  if (flash4 == 13) {
  }
  if (flash4 == 14) {
  }
  if (flash4 == 15) { 
  }
  if (flash4 == 16) {
  }
  if (flash4 == 17) {
  }
  if (flash4 == 18) {
  }
  if (flash4 == 19) {
  }
  if (flash4 == 20) {
  }
  if (flash4 == 21) {
  }
  if (flash4 == 22) {
  digitalWrite(D6,LOW);
  }
  if (flash4 == 23) {
  }
  if (flash4 == 24) {
  }
  if (flash4 == 25) {
  }
  if (flash4 == 26) {
  }
  flash4++;
  if (flash4 > 26) {
  flash4 = 1;
  }
}
