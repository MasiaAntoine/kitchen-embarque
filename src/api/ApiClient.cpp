#include "ApiClient.h"
#include <WiFiClientSecure.h>

ApiClient::ApiClient(const String& baseUrl, const String& username, const String& password) {
  this->baseUrl = baseUrl;
  this->username = username;
  this->password = password;
}

bool ApiClient::registerBalance(const String& macAddress, const String& name) {
  WiFiClientSecure *client = new WiFiClientSecure;
  client->setInsecure();

  HTTPClient http;
  
  // URL pour l'enregistrement de la balance
  String url = baseUrl + "/balances/reserved-machine";
  
  Serial.print("Enregistrement de la balance à l'URL: ");
  Serial.println(url);
  
  // Pour HTTPS, désactiver la vérification du certificat
  http.begin(*client, url);
  
  // En-tête pour l'authentification Basic
  String authHeader = "Basic " + base64Encode(username + ":" + password);
  http.addHeader("Authorization", authHeader);
  http.addHeader("Content-Type", "application/json");
  
  // Préparation du corps de la requête avec Arduino_JSON
  JSONVar requestData;
  requestData["mac_address"] = macAddress;
  requestData["name"] = name;
  
  String requestBody = JSON.stringify(requestData);
  Serial.println("Corps de la requête: " + requestBody);  // Log pour débogage
  
  // Envoi de la requête POST
  int httpResponseCode = http.POST(requestBody);
  bool success = false;
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Code de réponse HTTP: " + String(httpResponseCode));
    Serial.println("Réponse: " + response);
    
    // Analyse de la réponse avec Arduino_JSON si nécessaire
    if (httpResponseCode == 200 || httpResponseCode == 201) {  // Ajout de 201 Created
      JSONVar responseObj = JSON.parse(response);
      if (JSON.typeof(responseObj) != "undefined") {
        // Traitement de la réponse JSON si nécessaire
        Serial.println("Réponse JSON valide reçue");
        success = true;
      }
    }
  } else {
    Serial.print("Erreur lors de l'envoi de la requête HTTP: ");
    Serial.println(httpResponseCode);
    Serial.println("Tentative de diagnostic...");
    
    // Messages d'erreur plus détaillés pour le débogage
    switch(httpResponseCode) {
      case -1: Serial.println("HTTPC_ERROR_CONNECTION_REFUSED"); break;
      case -2: Serial.println("HTTPC_ERROR_SEND_HEADER_FAILED"); break;
      case -3: Serial.println("HTTPC_ERROR_SEND_PAYLOAD_FAILED"); break;
      case -4: Serial.println("HTTPC_ERROR_NOT_CONNECTED"); break;
      case -5: Serial.println("HTTPC_ERROR_CONNECTION_LOST"); break;
      case -6: Serial.println("HTTPC_ERROR_NO_STREAM"); break;
      case -7: Serial.println("HTTPC_ERROR_NO_HTTP_SERVER"); break;
      case -8: Serial.println("HTTPC_ERROR_TOO_LESS_RAM"); break;
      case -9: Serial.println("HTTPC_ERROR_ENCODING"); break;
      case -10: Serial.println("HTTPC_ERROR_STREAM_WRITE"); break;
      case -11: Serial.println("HTTPC_ERROR_READ_TIMEOUT"); break;
      default: Serial.println("HTTPC_ERROR_UNKNOWN: " + String(httpResponseCode));
    }
  }
  
  http.end();
  return success;
}