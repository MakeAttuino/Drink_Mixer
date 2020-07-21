/****************************************

  Progetto  :  Base bicchiere
  Autore    : L.Mariani
  Data      : 21/07/2020
  Versione  : V 1.0
  Contatto  :  makeattuin@gmail.com
  Web site  :  www.lab121.it

****************************************

  Vengono definite 4 sequenze distinte
  di gioco di luce selezionabili tramite
  2 ingressi dell'ATtiny85 in funzione
  dello stato in cui si trova il
  Drink_Mixer durante le varie fasi
  di preparazione del cocktail

****************************************/

#include <Adafruit_NeoPixel.h>
#define pin_strip PB1
#define num_pin 24+12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_pin, pin_strip, NEO_GRB + NEO_KHZ800);

// Variabili e/o costanti di Sistema
#ifdef __AVR__
  #include <avr/power.h>
#endif
uint8_t app1;
uint8_t app2;

// mappa I/O
#define tipo_1 PB3
#define tipo_2 PB4

void setup() {
  // inizializzo pin I/O
  pinMode(tipo_1,INPUT_PULLUP);
  pinMode(tipo_2,INPUT_PULLUP);
  // inizializzo anelli Neopixel
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // 'off'
}

void loop() {
  app1 = digitalRead(tipo_1);app2 = digitalRead(tipo_2);
  if(app1 == HIGH && app2 == HIGH){
  singolo_colore(strip.Color(0, 127, 0), 100); // White
  singolo_colore(strip.Color(127, 0, 0), 100); // Red
  singolo_colore(strip.Color(0, 0, 127), 100); // Blue
  }
  if(app1 == LOW && app2 == HIGH){
    rainbow(20);
  }
  if(app1 == HIGH && app2 == LOW){
    rainbowCycle(20);
  }
  if(app1 == LOW && app2 == LOW){
    theaterChaseRainbow(50);
  }
}

// abilita singolo pixel in sequenza, con singolo colore
void singolo_colore(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<num_pin; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<num_pin; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< num_pin; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / num_pin) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < num_pin; i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < num_pin; i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
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
