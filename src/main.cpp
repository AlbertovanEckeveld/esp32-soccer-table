#include <Arduino.h>
#include "wifi-controller.h"

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("ESP32 Soccer Table - WiFi Setup");
  Serial.println("================================");
  
  initWiFi();
  connectToWiFi();
}

void loop() {
  updateWiFi();
  
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > 10000) {
    checkWiFiStatus();
    lastWiFiCheck = millis();
  }
  
  // Your main application code goes here
  // No delay needed - the loop runs freely
}
