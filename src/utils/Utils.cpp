#include "Utils.h"

// Fonction pour obtenir l'adresse MAC formatée
String getMacAddress() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18] = { 0 };
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

// Fonction pour encoder en Base64 (nécessaire pour Basic Auth)
String base64Encode(String input) {
  const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int padCount = (3 - (input.length() % 3)) % 3;
  String encoded = "";
  unsigned char bytes[3];
  int i = 0;
  
  while (i < input.length()) {
    int j = 0;
    while (j < 3 && i < input.length()) {
      bytes[j++] = input.charAt(i++);
    }
    
    // Padding si nécessaire
    while (j < 3) {
      bytes[j++] = 0;
    }
    
    encoded += ALPHABET[(bytes[0] >> 2) & 0x3F];
    encoded += ALPHABET[((bytes[0] << 4) & 0x30) | ((bytes[1] >> 4) & 0x0F)];
    encoded += (i - 2 > input.length()) ? '=' : ALPHABET[((bytes[1] << 2) & 0x3C) | ((bytes[2] >> 6) & 0x03)];
    encoded += (i - 1 > input.length()) ? '=' : ALPHABET[bytes[2] & 0x3F];
  }
  
  return encoded;
}