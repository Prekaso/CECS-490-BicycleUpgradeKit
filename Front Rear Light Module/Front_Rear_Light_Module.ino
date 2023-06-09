#include <avr/pgmspace.h>
#include "Arduino.h"
#include <FastLED.h>
#include <EEPROM.h>

#define LED_PIN 2         // LED Strip Signal Connection 
#define ParkSignal 3      // Park Signal Connection
#define BrakeSignal 5     // Brake Signal Connection
#define LeftSignal 7      // Left Blinker Signal Connection
#define RightSignal 9     // Right Blinker Signal Connection
#define ReverseSignal 11  // Reverse Signal Connection
#define NUM_LEDS 64

#define BRIGHTNESS 200    // Adjust the System Brightness here ; Value Min - 10 , Max - 255

int BlinkerSpeed = 75;
int HazardSpeed = 100;
int VIPSpeed = 50;
int KITTSpeed = 75;
int StartupSpeed = 100;

int ParkDetect = 0;

CRGB leds[NUM_LEDS];

const long ParkAnimation_Frame1[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame2[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x4b0000, 0x000000, 0x000000, 0x4b0000, 0x000000, 0x000000,
0x000000, 0x000000, 0x4b0000, 0x000000, 0x000000, 0x4b0000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame3[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000, 
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000,
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000, 
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000, 
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame4[] PROGMEM =
{
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000,  
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000,
0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000,
0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000,
0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000,
0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 
0x000000, 0x4b0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x4b0000, 0x000000,
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame5[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000,
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame6[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000,
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame7[] PROGMEM =
{
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7 
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7 
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7  
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000,
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
};

const long ParkAnimation_Frame8[] PROGMEM =
{
0x000000, 0x000000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0x000000, 0x000000, 
0x000000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0x000000, 
0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 
0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 
0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000,  
0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 
0x000000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0x000000,
0x000000, 0x000000, 0xc80000, 0xc80000, 0xc80000, 0xc80000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame9[] PROGMEM =
{
0x000000, 0x000000, 0x960000, 0x960000, 0x960000, 0x960000, 0x000000, 0x000000, 
0x000000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x000000, 
0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 
0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 
0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000,  
0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 
0x000000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x960000, 0x000000,
0x000000, 0x000000, 0x960000, 0x960000, 0x960000, 0x960000, 0x000000, 0x000000,
};

const long ParkAnimation_Frame10[] PROGMEM =
{
0x000000, 0x000000, 0x640000, 0x640000, 0x640000, 0x640000, 0x000000, 0x000000, 
0x000000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x000000, 
0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 
0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 
0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000,  
0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 
0x000000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x640000, 0x000000,
0x000000, 0x000000, 0x640000, 0x640000, 0x640000, 0x640000, 0x000000, 0x000000,
};

const long LeftArrow_Frame1[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long LeftArrow_Frame2[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
};

const long LeftArrow_Frame3[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,  
};

const long LeftArrow_Frame4[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,  
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long LeftArrow_Frame5[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long LeftArrow_Frame6[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
};

const long LeftArrow_Frame7[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0xffa500, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0xffa500, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long LeftArrow_Frame8[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0xffa500, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0xffa500, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long RightArrow_Frame1[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,  
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const long RightArrow_Frame2[] PROGMEM =
{
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const long RightArrow_Frame3[] PROGMEM =
{
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const long RightArrow_Frame4[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,  
};

const long RightArrow_Frame5[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const long RightArrow_Frame6[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long RightArrow_Frame7[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0xffa500, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0xffa500, 
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long RightArrow_Frame8[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,  
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0xffa500, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0xffa500, 
0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 0x000000, 0xffa500, 0xffa500, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
};

const long Hazard_Frame1[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long Hazard_Frame2[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0xffa500, 0xffa500, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long Hazard_Frame3[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const long Hazard_Frame4[] PROGMEM =
{
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 
0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 
0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 
0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500,  
0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 
0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000,
0x000000, 0x000000, 0xffa500, 0xffa500, 0xffa500, 0xffa500, 0x000000, 0x000000,
};


const long Brake[] PROGMEM =
{
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000,  
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7 
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7  
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7 
0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, //7  
0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 
0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 
};

const long Park[] PROGMEM =
{
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 
0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000,  
0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 
0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000,  
0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 
0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000,  
0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 
0x000000, 0x000000, 0x4b0000, 0x4b0000, 0x4b0000, 0x4b0000, 0x000000, 0x000000, 
};

const long ReverseLight[] PROGMEM =
{
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000, 
0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000,  
0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 
0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff,  
0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff,  
0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 
0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000,
};

const long Blank[] PROGMEM =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};


void setup() 
{
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
FastLED.setBrightness(BRIGHTNESS);
pinMode(ParkSignal, INPUT);
pinMode(BrakeSignal, INPUT);
pinMode(LeftSignal, INPUT);
pinMode(RightSignal, INPUT);
pinMode(ReverseSignal, INPUT);
}


void loop() 
{
if(digitalRead(ReverseSignal)==1)
{
if(digitalRead(BrakeSignal)==0) //Reverse Light
{   
Reverse();
}

if(digitalRead(BrakeSignal)==1) //Brake Light
{   
BrakeFull();
}
}

if(ParkDetect == 0)
{
if(digitalRead(ParkSignal)==1) 
{
ParkDetect = 1;
}
}

if(ParkDetect == 2)
{
if(digitalRead(ParkSignal)==0) 
{
ParkDetect = 3;
}
}

if(ParkDetect == 1)
{
ParkON();
ParkDetect = 2;  
}

if(ParkDetect == 3)
{
ParkOFF();
ParkDetect = 0;  
}


if(digitalRead(ParkSignal)==0)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(BrakeSignal)==0)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //No Signal
{
AllOff();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
AllOff();
LeftBlinker();
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
AllOff();
RightBlinker();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
BrakeFull();
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
BrakeFull();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
AllOff();
DualBlinker();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
BrakeFull();
}
}
}


if(ParkDetect == 2)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Park Light
{   
ParkFull();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
LeftBlinker();
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
RightBlinker();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
BrakeFull();
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
BrakeFull();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
AllOff();
DualBlinker();
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
BrakeFull();
}
}
}
}


void AllOff()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();  
}


void Reverse()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ReverseLight[i]));  // Read array from Flash
  }
FastLED.show(); 
}

void BrakeFull()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Brake[i]));  // Read array from Flash
  }
FastLED.show();
}

void ParkFull()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Park[i]));  // Read array from Flash
  }
FastLED.show();

}

void LeftBlinker()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame4[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame5[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame6[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame7[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(LeftArrow_Frame8[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Blank[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);
}


void RightBlinker()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame4[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame5[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame6[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame7[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(RightArrow_Frame8[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Blank[i]));  // Read array from Flash
  }
FastLED.show();
delay(BlinkerSpeed);
}

void DualBlinker()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Hazard_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(HazardSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Hazard_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(HazardSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Hazard_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(HazardSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Hazard_Frame4[i]));  // Read array from Flash
  }
FastLED.show();
delay(HazardSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Blank[i]));  // Read array from Flash
  }
FastLED.show();
delay(HazardSpeed);
}


void ParkON()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame4[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame5[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame6[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame7[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame8[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame9[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame10[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Park[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);
}


void ParkOFF()
{
FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame10[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame9[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame8[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame7[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame6[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame5[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame4[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame3[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame2[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(ParkAnimation_Frame1[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);

FastLED.clear();
for(int i = 0; i < NUM_LEDS; i++) 
  {
  leds[i] = pgm_read_dword(&(Blank[i]));  // Read array from Flash
  }
FastLED.show();
delay(StartupSpeed);
}
