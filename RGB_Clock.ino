#include "Adafruit_NeoPixel.h"
#include "WS2812_Definitions.h"
#include "Time.h"

#define PIN 4
#define LED_COUNT 60
#define HOURCOLOR YELLOW
#define MINUTECOLOR BLUE
#define SECONDCOLOR RED

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  setTime(9,25,0,1,1,11); // set time to noon Jan 1 2011
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  DrawClock();
  delay(1000);
}

void DrawClock(){
  int h = hour();
  
  if (h > 12)
    h = h - 12;
  
  h = h * 5;  
  if (h == 60)
    h=0;
    
  if (h == 0)
    strip.setPixelColor(55,0);
  else
    strip.setPixelColor(h-1, 0);
 
  if (minute() == 0)
    strip.setPixelColor(59,0);
  else
    strip.setPixelColor(minute()-1, 0);
  
  if (second() == 0)
    strip.setPixelColor(59, 0);
  else
    strip.setPixelColor(second()-1, 0);

  if (hour() == 9 && (minute() >=30 && minute() <=45))
  {
    for(int b=minute()+1;b<=45;b++)
    {
      strip.setPixelColor(b, GREEN);
    }    
  }

  if (hour() == 12 && (minute() >=15 && minute() <=45))
  {
    for(int b=minute()+1;b<=45;b++)
    {
      strip.setPixelColor(b, GREEN);
    }    
  }

  if (minute() == h)
    strip.setPixelColor(h, YELLOW);  
  else
  {
    strip.setPixelColor(minute(), BLUE);
    strip.setPixelColor(h, PURPLE);  
  }
  
  strip.setPixelColor(second(), RED);
  
  strip.show();
}


