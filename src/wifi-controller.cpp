#include "wifi-controller.h"

const char* ssid = "Koos Draadloos";
const char* password = "Hofweg#een01";

const unsigned long WIFI_TIMEOUT = 20000;
const unsigned long WIFI_CHECK_INTERVAL = 100;

WiFiConnectionStatus currentStatus = WIFI_DISCONNECTED;
unsigned long lastWiFiCheck = 0;
unsigned long wifiConnectionStartTime = 0;
unsigned long lastStatusPrint = 0;

void initWiFi() {
  Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void connectToWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi already connected");
    return;
  }
  
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  currentStatus = WIFI_CONNECTING;
  WiFi.begin(ssid, password);
  wifiConnectionStartTime = millis();
  lastStatusPrint = millis();
}

void updateWiFi() {
  unsigned long currentTime = millis();
  
  // Only check WiFi status at intervals to avoid overwhelming the system
  if (currentTime - lastWiFiCheck < WIFI_CHECK_INTERVAL) {
    return;
  }
  lastWiFiCheck = currentTime;
  
  if (currentStatus == WIFI_CONNECTING) {
    // Print dots every 500ms while connecting
    if (currentTime - lastStatusPrint >= 500) {
      Serial.print(".");
      lastStatusPrint = currentTime;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      currentStatus = WIFI_CONNECTED;
      Serial.println();
      Serial.println("WiFi connected successfully!");
      printWiFiInfo();
      onWiFiConnected();
    } else if (currentTime - wifiConnectionStartTime >= WIFI_TIMEOUT) {
      currentStatus = WIFI_CONNECTION_FAILED;
      Serial.println();
      Serial.println("Failed to connect to WiFi");
      onWiFiDisconnected();
    }
  }
}

void checkWiFiStatus() {
  if (WiFi.status() != WL_CONNECTED && currentStatus == WIFI_CONNECTED) {
    currentStatus = WIFI_CONNECTION_LOST;
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    onWiFiDisconnected();
    connectToWiFi();
  } else if (WiFi.status() == WL_CONNECTED && currentStatus != WIFI_CONNECTED && currentStatus != WIFI_CONNECTING) {
    currentStatus = WIFI_CONNECTED;
    Serial.println("WiFi reconnected!");
    printWiFiInfo();
    onWiFiConnected();
  }
}

void disconnectWiFi() {
  WiFi.disconnect();
  currentStatus = WIFI_DISCONNECTED;
  Serial.println("WiFi disconnected");
}

WiFiConnectionStatus getWiFiStatus() {
  return currentStatus;
}

void printWiFiInfo() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("--- WiFi Information ---");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.println("----------------------");
  }
}

String getLocalIP() {
  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.localIP().toString();
  }
  return "Not connected";
}

int getSignalStrength() {
  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.RSSI();
  }
  return 0;
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void onWiFiConnected() {
  Serial.println("WiFi connected event triggered");
}

void onWiFiDisconnected() {
  Serial.println("WiFi disconnected event triggered");
}
