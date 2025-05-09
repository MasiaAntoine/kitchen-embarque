#include "ApiClient.h"
#include <WiFiClientSecure.h>

ApiClient::ApiClient(const String& baseUrl, const Credentials& credentials) {
  this->baseUrl = baseUrl;
  this->credentials = credentials;
}

ApiClient::ApiClient(const String& baseUrl, const String& username, const String& password) {
  this->baseUrl = baseUrl;
  this->credentials = Credentials(username, password);
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
  String authHeader = "Basic " + base64Encode(credentials.getBasicAuthString());
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
    if (httpResponseCode == 200 || httpResponseCode == 201) {
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

bool ApiClient::updateQuantity(const String& macAddress, int quantity) {
  WiFiClientSecure *client = new WiFiClientSecure;
  client->setInsecure();

  HTTPClient http;
  
  // Augmenter le délai d'attente
  http.setTimeout(15000);
  
  String url = baseUrl + "/balances/reserved-machine/update-quantity";
  Serial.print("Envoi du poids à l'URL: ");
  Serial.println(url);
  
  if (!http.begin(*client, url)) {
    Serial.println("Échec de l'initialisation de la connexion HTTP");
    delete client;
    return false;
  }
  
  String authHeader = "Basic " + base64Encode(credentials.getBasicAuthString());
  http.addHeader("Authorization", authHeader);
  http.addHeader("Content-Type", "application/json");
  
  JSONVar requestData;
  requestData["mac_address"] = macAddress;
  requestData["quantity"] = String(quantity);
  
  String requestBody = JSON.stringify(requestData);
  Serial.println("Corps de la requête: " + requestBody);
  
  // Ajout de diagnostics supplémentaires
  Serial.print("État de la connexion WiFi: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connecté" : "Déconnecté");
  Serial.print("Force du signal WiFi: ");
  Serial.println(WiFi.RSSI());
  
  int httpResponseCode = http.POST(requestBody);
  bool success = false;
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Code de réponse HTTP: " + String(httpResponseCode));
    Serial.println("Réponse: " + response);
    
    if (httpResponseCode == 200 || httpResponseCode == 201) {
      success = true;
      Serial.println("Mise à jour de la quantité réussie");
    }
  } else {
    Serial.print("Erreur lors de l'envoi de la requête HTTP: ");
    Serial.println(httpResponseCode);
    
    // Diagnostics supplémentaires
    Serial.println("Tentative de diagnostic réseau...");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Passerelle: ");
    Serial.println(WiFi.gatewayIP());
  }
  
  // Pause avant de fermer la connexion
  delay(1000);
  http.end();
  delete client;
  return success;
}