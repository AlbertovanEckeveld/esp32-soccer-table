#include <Arduino.h>
#include "led-controller.h"
#include "ir-controller.h"

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("ESP32 Soccer Table - WiFi Setup");
  Serial.println("================================");

  initLEDs();
  
  initIRSensors();
  
  // Start with full white effect
  setLEDEffect(LED_FULL_WHITE);
  
  Serial.println("⚽ Soccer table system initialized successfully! ⚽");
  Serial.println("Ready for play! Score goals to see LED celebrations!");
}

void loop() {

  updateLEDs();
  
  // Check for goals (IR sensors)
  updateIRSensors();
  
  // Only cycle through demo effects when not celebrating a goal
  if (!isCelebrationActive()) {
    static unsigned long lastEffectChange = 0;
    static int currentEffectIndex = 1; // Start with full white
    
    if (millis() - lastEffectChange > 10000) {
      LEDEffect effects[] = {LED_FULL_WHITE, LED_COLOR_WAVE, LED_RAINBOW_WAVE, LED_BREATHING};
      
      setLEDEffect(effects[currentEffectIndex]);
      
      if (currentEffectIndex == 1) { // Color wave
        setWaveColor(CRGB::Blue); // Set wave color to blue
      }
      
      currentEffectIndex = (currentEffectIndex + 1) % 4;
      lastEffectChange = millis();
    }
  }
  
  // Your main application code goes here
  // No delay needed - the loop runs freely
}
