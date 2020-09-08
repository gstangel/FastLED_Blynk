#include "FastLED.h"
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, Tx
#include <BlynkSimpleStream.h>
#include <Thread.h>

#define BLYNK_PRINT SwSerial
#define LED_DT 7                                            // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812B                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 300                                           // Number of LED's
 
// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
int currentRed,currentBlue,currentGreen;
int mode = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V1)
{
  currentRed= param[0].asInt();
  currentBlue = param[1].asInt();
  currentGreen = param[2].asInt();

  setColors(currentRed,currentBlue,currentGreen);

  // process received value
}

//brightness
BLYNK_WRITE(V2)
{
  int pinValue = param[0].asInt(); // assigning incoming value from pin V1 to a variable

  setBrightness(pinValue);
}

//change mode
BLYNK_WRITE(V3)
{
  mode = param[0].asInt(); // assigning incoming value from pin V1 to a variable
  
  changeMode(mode);
}

//set colors
void setColors(int r, int g, int b)
{
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

//change brightness
void setBrightness(int brightness)
{
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void changeMode(int mode)
{
  if(mode == 0)
  {
    setColors(currentRed,currentBlue,currentGreen);
  }
  else if(mode == 1)
  {
      rainbow_wave(100, 10);
  }
}
  


void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
 
// uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 uint8_t thisHue = beat8(thisSpeed,255);                     // A simple rainbow march.
  
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);
 FastLED.show();// Use FastLED's fill_rainbow routine.
 
} // rainbow_wave()


void setup()
{
  // Debug console
  SwSerial.begin(9600);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS); 
  FastLED.setBrightness(max_bright);
}

void loop()
{
  Blynk.run();
  
}
