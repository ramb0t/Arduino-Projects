// Based on http://www.instructables.com/id/Rainbow-LED-Headband-WS2812-ATTiny85/ https://github.com/smartynov/iotfun/blob/instructables/arduino/deco_lights/deco_lights.ino
#include <Adafruit_NeoPixel.h>

// set to pin connected to data input of WS8212 (NeoPixel) strip
#define PIN         0

// any pin with analog input (used to initialize random number generator)
#define RNDPIN      2

// number of LEDs (NeoPixels) in your strip
// (please note that you need 3 bytes of RAM available for each pixel)
#define NUMPIXELS   1

// max LED brightness (1 to 255) – start with low values!
// (please note that high brightness requires a LOT of power)
#define BRIGHTNESS  255

// increase to get narrow spots, decrease to get wider spots
#define FOCUS       65

// decrease to speed up, increase to slow down (it's not a delay actually)
#define DELAY       4000

// set to 1 to display FPS rate
#define DEBUG       0


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int sensorValue = 0; 

void setup() {
  //http://digistump.com/wiki/digispark/tutorials/basics#analog_read
  pinMode(5, INPUT);

  // initialize LED strip
  strip.begin();
  strip.show();
}

void loop() {

  sensorValue = analogRead(0); //Read P5
  //To set to input: pinMode(5, INPUT);
  //THIS IS P5, P5 is analog input 0, so when you are using analog read, you refer to it as 0.

  //1023 = 10V
  //921 = 9V
  //818 = 8V
  //716 = 7V
  //665 = 6.5V

  if(sensorValue >= 921){ // >9V
    strip.setPixelColor(0,0,0,255); //Blue
  }else if(sensorValue >= 818){ // >8V
    strip.setPixelColor(0,0,255,255); //Blue Green
  }else if(sensorValue >= 716){ // >7V
    strip.setPixelColor(0,0,255,0); //Green
  }else if(sensorValue >= 665){ // >6.5V
    strip.setPixelColor(0,255,0,0); //Red
  }else{
    strip.setPixelColor(0,255,128,0); //Orange
  }
  

  // send data to LED strip
  strip.show();
}
