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
  
  // Start with LEDs off (will turn white automatically when game becomes active)
  setLEDEffect(LED_OFF);
  
  Serial.println("⚽ Soccer table system initialized successfully! ⚽");
  Serial.println("Ready for play! Score goals to see LED celebrations!");
}

void loop() {
  // Non-blocking LED animations
  updateLEDs();
  
  // Check for goals (IR sensors) - only during active games
  updateIRSensors();
  
  // LED control based on game state
  if (!isCelebrationActive()) {
    if (isGameActive()) {
      // Game is active - show static white light
      static bool gameActiveEffectSet = false;
      if (!gameActiveEffectSet) {
        setLEDEffect(LED_FULL_WHITE);
        gameActiveEffectSet = true;
      }
    } else {
      // No game active - turn LEDs off
      static bool gameInactiveEffectSet = false;
      if (!gameInactiveEffectSet) {
        setLEDEffect(LED_OFF);
        gameInactiveEffectSet = true;
      }
    }
    
    // Reset flags when game state changes
    static bool lastGameState = isGameActive();
    if (lastGameState != isGameActive()) {
      static bool gameActiveEffectSet = false;
      static bool gameInactiveEffectSet = false;
      lastGameState = isGameActive();
    }
  }
  
  // Your main application code goes here
  // No delay needed - the loop runs freely
}
