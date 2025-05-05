#include "WiFiManager.h"

bool connectToWiFi(const String& ssid, const String& password, unsigned long timeout) {
  Serial.println("Connexion au WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid.c_str(), password.c_str());
  
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Vérifier si le timeout est atteint
    if (millis() - startTime > timeout) {
      Serial.println("\nÉchec de connexion WiFi - timeout");
      return false;
    }
  }
  
  Serial.println("");
  Serial.println("WiFi connecté!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
  
  return true;
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

String getLocalIP() {
  return WiFi.localIP().toString();
}