#include <Arduino.h>
#include "wifi-controller.h"
#include "led-controller.h"

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("ESP32 Soccer Table - WiFi Setup");
  Serial.println("================================");
  
  initWiFi();
  connectToWiFi();
  
  initLEDs();
  
  setLEDEffect(LED_FULL_WHITE);
  
  Serial.println("System initialized successfully!");
}

void loop() {
  updateWiFi();
  
  updateLEDs();
  
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > 10000) {
    checkWiFiStatus();
    lastWiFiCheck = millis();
  }
  
  static unsigned long lastEffectChange = 0;
  static int currentEffectIndex = 1; 
  
  if (millis() - lastEffectChange > 10000) {
    LEDEffect effects[] = {LED_FULL_WHITE, LED_COLOR_WAVE, LED_RAINBOW_WAVE, LED_BREATHING};
    
    setLEDEffect(effects[currentEffectIndex]);
    
    if (currentEffectIndex == 1) { 
      setWaveColor(CRGB::Blue); // Set wave color to blue
    }
    
    currentEffectIndex = (currentEffectIndex + 1) % 4;
    lastEffectChange = millis();
  }
  
  // Your main application code goes here
  // No delay needed - the loop runs freely
}
