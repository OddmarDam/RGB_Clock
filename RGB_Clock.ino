#include "Adafruit_NeoPixel.h"
#include "WS2812_Definitions.h"
#include "DS1307RTC.h"
#include "Time.h"
#include <Wire.h>  


#define PIN 4
#define LED_COUNT 60

#define HOURCOLOR    MAGENTA
#define MINUTECOLOR  CYAN
#define SECONDCOLOR  NAVY            
#define BREAKCOLOR   YELLOW

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
//  setTime(9,26,0,1,1,11); // set time to noon Jan 1 2011
  Serial.begin(9600);
  Serial.println("RGB Clock");
  
  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      
     
  digitalClockDisplay();  
     
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  if(Serial.available())
  {
     time_t t = processSyncMessage();
     if(t >0)
     {
        RTC.set(t);   // set the RTC and the system time to the received value
        setTime(t);          
     }
  } 
//  digitalClockDisplay();
  DrawClock();
  delay(500);
}

/*  code to process time sync messages from the serial port   */
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message

time_t processSyncMessage() {
  // return the time if a valid sync message is received on the serial port.
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      return pctime; 
    }  
  }
  return 0;
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void DrawClock(){
  int h = hour();
  int m = minute();
  int s = second();
  
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
    strip.setPixelColor(m-1, 0);
  
  if (second() == 0)
    strip.setPixelColor(59, 0);
  else
    strip.setPixelColor(s-1, 0);

//  rainbow(10);

  // Draw coffee break
  if (hour() == 9 && (m >=30 && m <=45))
  {
    for(int b=m+1;b<=45;b++)
    {
      strip.setPixelColor(b, BREAKCOLOR);
    }    
  }

  // Draw lunch break
  if (hour() == 12 && (m >=15 && m <=45))
  {
    for(int b=m+1;b<=45;b++)
    {
      strip.setPixelColor(b, BREAKCOLOR);
    }    
  }


  // Draw Hour, minute, second
  if (m == h)
    strip.setPixelColor(h, WHITE);  
  else
  {
    strip.setPixelColor(m, MINUTECOLOR);
    strip.setPixelColor(h, HOURCOLOR);  
  }
  
  strip.setPixelColor(s, SECONDCOLOR);
  
  strip.show();
 }

void rainbow(uint8_t wait) {
  uint16_t i;
  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, 2,2,2);
  }
//  strip.show();
}

