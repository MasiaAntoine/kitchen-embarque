#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <WiFi.h>

// Fonction pour obtenir l'adresse MAC formatée
String getMacAddress();

// Fonction pour encoder en Base64 (nécessaire pour Basic Auth)
String base64Encode(String input);

#endif