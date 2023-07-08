  #include <Adafruit_NeoPixel.h>
  
  #define LED_PIN 0
  #define LED_COUNT 300
  
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int effect = 5;
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Choose a random color
//  effect = effect%6 + 1;
  Serial.println(effect);
  if(effect==1){
  uint32_t color = strip.Color(random(255), random(255), random(255));
  
  // Set a random LED to the chosen color
  int ledIndex = random(strip.numPixels());
  strip.setPixelColor(ledIndex, color);
  
  // Fade out the previous LED color
  for(int i=0; i<strip.numPixels(); i++) {
    uint32_t oldColor = strip.getPixelColor(i);
    uint8_t r = (oldColor >> 16) & 0xFF;
    uint8_t g = (oldColor >> 8) & 0xFF;
    uint8_t b = oldColor & 0xFF;
    if(r > 0) r--;
    if(g > 0) g--;
    if(b > 0) b--;
    strip.setPixelColor(i, r, g, b);
  }
  
  strip.show();
  
  delay(10);
  }
  else if(effect==2){
    for(int i=strip.numPixels()-1; i>=1; i--) {
    uint32_t color = strip.getPixelColor(i-1);
    strip.setPixelColor(i, color);
  }
  
  // Choose a random color for the first LED
  uint32_t color = strip.Color(random(255), random(255), random(255));
  strip.setPixelColor(0, color);
  
  strip.show();
  
  delay(50);
  }
  else if(effect==3){
    for(int i=0; i<strip.numPixels(); i+=2) {
    strip.setPixelColor(i, 255, 0, 0); // Red
  }
  for(int i=1; i<strip.numPixels(); i+=2) {
    strip.setPixelColor(i, 0, 0, 255); // Blue
  }
  
  strip.show();
  
  delay(500);
  
  // Swap the colors of the even-numbered and odd-numbered LEDs
  for(int i=0; i<strip.numPixels()-1; i+=2) {
    uint32_t color1 = strip.getPixelColor(i);
    uint32_t color2 = strip.getPixelColor(i+1);
    strip.setPixelColor(i, color2);
    strip.setPixelColor(i+1, color1);
  }
  
  strip.show();
  
  delay(500);
  }
  else if(effect==4){
    uint8_t intensity[LED_COUNT];
  for(int i=0; i<strip.numPixels(); i++) {
    intensity[i] = random(200, 255);
  }
  
  // Set the color of each LED based on its intensity value
  for(int i=0; i<strip.numPixels(); i++) {
    uint8_t red = map(intensity[i], 0, 255, 0, 255);
    uint8_t green = map(intensity[i], 0, 255, 0, 80);
    uint8_t blue = 0;
    strip.setPixelColor(i, red, green, blue);
  }
  
  strip.show();
  
  // Slowly decrease the intensity of each LED
  for(int i=0; i<strip.numPixels(); i++) {
    if(intensity[i] > 5) {
      intensity[i] -= 5;
    }
  }
  
  delay(30);
  }
  else if(effect==5){
    static uint16_t pos = 0;
  
  // Set the color of each LED based on its position in the rainbow cycle
  for(int i=0; i<strip.numPixels(); i++) {
    uint8_t r, g, b;
    Wheel((i+pos) & 255, &r, &g, &b);
    strip.setPixelColor(i, r, g, b);
  }
  
  strip.show();
  
  // Increment the position in the rainbow cycle
  pos++;
  
  delay(10);
  }
  else if(effect==6){
    for (int i = 0; i < strip.numPixels(); i++) {
    if (i % 10 == 0) {
      strip.setPixelColor(i, 255, 0, 0); // red
    } else if (i % 10 == 1) {
      strip.setPixelColor(i, 0, 255, 0); // green
    } else if (i % 10 == 2) {
      strip.setPixelColor(i, 0, 0, 255); // blue
    } else if (i % 10 == 3) {
      strip.setPixelColor(i, 255, 255, 0); // yellow
    } else if (i % 10 == 4) {
      strip.setPixelColor(i, 255, 0, 255); // magenta
    } else if (i % 10 == 5) {
      strip.setPixelColor(i, 0, 255, 255); // cyan
    } else if (i % 10 == 6) {
      strip.setPixelColor(i, 255, 128, 0); // orange
    } else if (i % 10 == 7) {
      strip.setPixelColor(i, 128, 255, 0); // lime
    } else if (i % 10 == 8) {
      strip.setPixelColor(i, 0, 128, 255); // sky blue
    } else {
      strip.setPixelColor(i, 128, 0, 255); // purple
    }
    strip.show();
  }
  for (int i = 299; i >=0; i--) {
    if (i % 10 == 0) {
      strip.setPixelColor(i, 255, 0, 0); // red
    } else if (i % 10 == 1) {
      strip.setPixelColor(i, 0, 255, 0); // green
    } else if (i % 10 == 2) {
      strip.setPixelColor(i, 0, 0, 255); // blue
    } else if (i % 10 == 3) {
      strip.setPixelColor(i, 255, 255, 0); // yellow
    } else if (i % 10 == 4) {
      strip.setPixelColor(i, 255, 0, 255); // magenta
    } else if (i % 10 == 5) {
      strip.setPixelColor(i, 0, 255, 255); // cyan
    } else if (i % 10 == 6) {
      strip.setPixelColor(i, 255, 128, 0); // orange
    } else if (i % 10 == 7) {
      strip.setPixelColor(i, 128, 255, 0); // lime
    } else if (i % 10 == 8) {
      strip.setPixelColor(i, 0, 128, 255); // sky blue
    } else {
      strip.setPixelColor(i, 128, 0, 255); // purple
    }
    strip.setPixelColor(i,0,0,0);
    strip.show();
  }
  
  
  delay(10);
  }
//  effect++;
  
}

void Wheel(byte WheelPos, byte *r, byte *g, byte *b) {
  if(WheelPos < 85) {
    *r = WheelPos * 3;
    *g = 255 - WheelPos * 3;
    *b = 0;
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    *r = 255 - WheelPos * 3;
    *g = 0;
    *b = WheelPos * 3;
  }
  else {
    WheelPos -= 170;
    *r = 0;
    *g = WheelPos * 3;
    *b = 255 - WheelPos * 3;
  }
}
