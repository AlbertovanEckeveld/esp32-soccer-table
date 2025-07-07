#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>

extern const char* ssid;
extern const char* password;
extern const unsigned long WIFI_TIMEOUT;

enum WiFiConnectionStatus {
  WIFI_DISCONNECTED,
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_CONNECTION_FAILED,
  WIFI_CONNECTION_LOST
};

void initWiFi();
void connectToWiFi();
void updateWiFi(); 
void checkWiFiStatus();
void disconnectWiFi();
WiFiConnectionStatus getWiFiStatus();
void printWiFiInfo();
String getLocalIP();
int getSignalStrength();
bool isWiFiConnected();

void onWiFiConnected();
void onWiFiDisconnected();

#endif // WIFI_CONTROLLER_H
