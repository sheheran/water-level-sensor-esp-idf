#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <AsyncDelay.h>

#define PIN 2
#define NUMPIXELS 1
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_BGR + NEO_KHZ800);

void setup() {
    Serial.begin(115200); // Initialize Serial
    //pinMode(2, OUTPUT);   // Set GPIO 2 as output
    pixels.begin();
}

void loop() {
    // digitalWrite(2, HIGH); // Turn LED on
    // delay(1000);           // Wait for 1 second
    // digitalWrite(2, LOW);  // Turn LED off
    // delay(1000);           // Wait for 1 second

    pixels.clear();

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 20, 10));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

}

// Enabling C++ compile
// extern "C" { void app_main(); }

// void app_main(void)
// {

// }

extern "C" void app_main() {
    initArduino();
    setup();
    while (true) {
        loop();
    }
}
