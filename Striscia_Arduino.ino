#include <Adafruit_NeoPixel.h>
#define pin_out 4
#define num_led 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_led, pin_out, NEO_GRB + NEO_KHZ800);


void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  // Some example procedures showing how to display to the pixels:
  for(byte y = 1; y < 5; y++)
  {
    colorWipe(strip.Color(255, 0, 0), 100); // Red
    colorWipe(strip.Color(0, 255, 0), 100); // Green
    colorWipe(strip.Color(0, 0, 255), 100); // Blue
  }
  rainbow(20);
  rainbowCycle(20);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for(uint16_t i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for(j=0; j<256*5; j++)
  { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
