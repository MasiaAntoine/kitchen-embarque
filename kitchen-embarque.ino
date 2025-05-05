#include <Arduino.h>
#include "src/config/env.h"
#include "src/network/WiFiManager.h"
#include "src/api/ApiClient.h"
#include "src/utils/Utils.h"

// Variables globales
String macAddress = "";
ApiClient* apiClient = nullptr;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Démarrage...");

  // Connexion au WiFi
  if (connectToWiFi(WIFI_SSID, WIFI_PASSWORD)) {
    // Obtention de l'adresse MAC
    macAddress = getMacAddress();
    Serial.print("Adresse MAC: ");
    Serial.println(macAddress);

    // IMPORTANT : Ajouter un délai pour stabiliser la connexion
    Serial.println("Attente de stabilisation de la connexion WiFi...");
    delay(5000);

    // Initialisation du client API
    apiClient = new ApiClient(API_BASE_URL, API_USERNAME, API_PASSWORD);

    // Enregistrement de la balance
    apiClient->registerBalance(macAddress, macAddress);
  }
}

void loop() {
  delay(10000);
}