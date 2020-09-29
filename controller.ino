#include <SimpleTimer.h>
#include "FastLED.h"
#include <BlynkSimpleStream.h>

SimpleTimer timer;

#define LED_DT 7                                                                                     
#define COLOR_ORDER GRB                                   // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812B                                   // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 150  

//blynk auth token
char auth[] = "";
// Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];// Initialize the LED array.

int currentRed, currentBlue, currentGreen;
int brightness = 255;
int mode = 0;

//color micer virtual pin
BLYNK_WRITE(V1)
{
  currentRed= param[0].asInt();
  currentBlue = param[1].asInt();
  currentGreen = param[2].asInt();

  setColors();
  FastLED.show();
  // process received value
}

//brightness slider virtural pin
BLYNK_WRITE(V2)
{
  brightness = param[0].asInt(); // assigning incoming value from pin V2 to a variable

  setBrightness();
}
void setBrightness(){
  FastLED.setBrightness(brightness);
  FastLED.show();
}

//mode selection virtural pin
BLYNK_WRITE(V3)
{
  mode = param[0].asInt(); // assigning incoming value from pin V1 to a variable
  FastLED.show();
}

//set led colors
void setColors(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(currentRed,currentGreen,currentBlue);
  }
}


void rainbow_march(uint8_t thisdelay, uint8_t deltahue) {     // The fill_rainbow call doesn't support brightness levels.

  uint8_t thishue = millis()*(255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
  
// thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
// thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
  
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue); 
  // Use FastLED's fill_rainbow routine.

}

int   thisdelay =   10;                                       // A delay value for the sequence(s)
uint8_t   count =   0;                                        // Count up to 255 and then reverts to 0
uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.

uint8_t bpm = 30;
void dot_beat() {

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()

void connect(){
  Blynk.run();
}



void showCurrentPallet(){
  Blynk.run();
  switch (mode) {
  case 0:
    setColors();
    break;
  case 1:
    rainbow_march(200,10);
    break;
  case 2:
    dot_beat();
    break;
    }
  
  FastLED.show();
}

void setup()
{
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS); 
  FastLED.setBrightness(brightness);
  timer.setInterval(45, connect);
  timer.setInterval(20, showCurrentPallet);
}

void loop()
{
  timer.run();
}
