#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "../utils/Utils.h" 

class ApiClient {
private:
  String baseUrl;
  String username;
  String password;
  
public:
  ApiClient(const String& baseUrl, const String& username, const String& password);
  
  // Enregistre la balance sur l'API
  bool registerBalance(const String& macAddress, const String& name);
};

#endif