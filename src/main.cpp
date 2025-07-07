#include <Arduino.h>
#include "wifi-controller.h"
#include "led-controller.h"
#include "ir-controller.h"

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("ESP32 Soccer Table - WiFi Setup");
  Serial.println("================================");
  
  // Initialize WiFi
  initWiFi();
  connectToWiFi();
  
  // Initialize LED strip
  initLEDs();
  
  // Initialize IR sensors for goal detection
  initIRSensors();
  
  // Start with full white effect
  setLEDEffect(LED_FULL_WHITE);
  
  Serial.println("⚽ Soccer table system initialized successfully! ⚽");
  Serial.println("Ready for play! Score goals to see LED celebrations!");
}

void loop() {
  // Non-blocking WiFi management
  updateWiFi();
  
  // Non-blocking LED animations
  updateLEDs();
  
  // Check for goals (IR sensors)
  updateIRSensors();
  
  // Check WiFi status every 10 seconds
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > 10000) {
    checkWiFiStatus();
    lastWiFiCheck = millis();
  }
  
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
