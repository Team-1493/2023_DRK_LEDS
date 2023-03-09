// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        10 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

#define INP1 2 // robot enabled/disabled
#define INP2 3 // has cube or cone
#define INP3 4 // arm stowed

#define TURNON 0
#define TURNOFF 1
#define CUBE 2
#define STOWED 3
#define CONED 4

#define FALCON_BLUE 0x3237CF
#define STANLEY_ORANGE 0xF2B13F

#define BTOP 0xD60270
#define BMID 0x9B4F96
#define BBOT 0x0038A8
int enabled = 0;
int lastintent = -1;

void setup() {
  pinMode(INP1,INPUT);
  pinMode(INP2,INPUT);
  pinMode(INP3,INPUT);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  int intent = lastintent;
  if (lastintent == TURNOFF || lastintent == TURNON){
    intent = -1;
  }

  if (digitalRead(INP2)){
    intent = HASITEM;    
  }

  int enabledcur = digitalRead(INP1);

  if (digitalRead(INP3)){
    intent = STOWED;

    if (enabledcur) {
      // INTENT = 
      enabledcur = 0;
    }    
  }
    
  if (enabledcur != enabled){
    enabled = enabledcur;
    if (enabled == 1){
      intent = TURNON;
    }else{
      intent = TURNOFF;
    }
  }

  intent = CONED;
  switch (intent){
    case TURNON:
        pixels.clear();
        sweep_in_ends(pixels.Color(200,200,200),50);    
      break;
    case TURNOFF:
        pixels.clear();
        sweep_full(pixels.Color(255,255,255),2);
        delay(500);        
        sweep_ends_in(pixels.Color(0,0,0),35);

        sweep_in_ends(FALCON_BLUE,10);   
      break;
    case CUBE:
        sweep_from_to(0,NUMPIXELS,BTOP,10);
        sweep_from_to(0,NUMPIXELS,BMID,10);
        sweep_from_to(0,NUMPIXELS,BBOT,10);
        sweep_in_ends(0x000000,5);
        sweep_from_to(NUMPIXELS,0,BBOT,10);
        sweep_from_to(NUMPIXELS,0,BMID,10);
        sweep_from_to(NUMPIXELS,0,BTOP,10);
      break;
    case CONED:
      
      break;
  }  
  pixels.clear();
  pixels.fill(pixels.Color(0, 0, 0));
  delay(500);

  lastintent = intent;
}


void sweep_full(uint32_t color,unsigned long delay_dt){
  sweep_from_to(0, NUMPIXELS, color, delay_dt);
}
void sweep_in_ends(uint32_t color,unsigned long delay_dt){  
  int mid = NUMPIXELS/2;
  for(int i=0; i<mid; i++) {
    pixels.setPixelColor(mid+i, color);
    pixels.setPixelColor(mid-i, color);
    pixels.show(); 
    delay(delay_dt);
  }
  pixels.setPixelColor(mid, color);
  pixels.show(); 
}

void sweep_nth(int offset,int mod,uint32_t color){
  for(int i=0; i <= NUMPIXELS; i++ ) {
    if ((i + offset) % mod != 0){
      pixels.setPixelColor(i, color); 
    }
  }
}

void sweep_ends_in(uint32_t color,unsigned long delay_dt){  
  int mid = NUMPIXELS/2;
  for(int i=0; i<mid; i++) {
    pixels.setPixelColor(i, color);
    pixels.setPixelColor(NUMPIXELS-i, color);
    pixels.show(); 
    delay(delay_dt);
  }
  
  pixels.setPixelColor(mid, color);
  pixels.show(); 
}
void sweep_from_to(int start, int end,uint32_t color,unsigned long delay_dt){
  int inc = 1;
  if (start > end) inc = -1;
  
  for(int i=start; i != end; i+= inc) {
    pixels.setPixelColor(i, color);
    pixels.show(); 
    delay(delay_dt);
  }
}
