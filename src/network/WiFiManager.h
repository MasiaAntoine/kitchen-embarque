#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// Connecte l'ESP au réseau WiFi
bool connectToWiFi(const String& ssid, const String& password, unsigned long timeout = 30000);

// Vérifie si le WiFi est connecté
bool isWiFiConnected();

// Récupère l'adresse IP
String getLocalIP();

#endif