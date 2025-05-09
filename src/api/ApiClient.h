#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "../utils/Utils.h"
#include "../utils/Credentials.h"

class ApiClient {
private:
  String baseUrl;
  Credentials credentials;
  
public:
  ApiClient(const String& baseUrl, const Credentials& credentials);
  ApiClient(const String& baseUrl, const String& username, const String& password);
  
  // Enregistre la balance sur l'API
  bool registerBalance(const String& macAddress, const String& name);
  
  // Envoie les données de poids à l'API
  bool updateQuantity(const String& macAddress, int quantity);
};

#endif