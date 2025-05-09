#include <Arduino.h>
#include "src/config/env.h"
#include "src/network/WiFiManager.h"
#include "src/api/ApiClient.h"
#include "src/utils/Utils.h"
#include "src/utils/Credentials.h"
#include "HX711.h"

// Configuration de la balance HX711
#define WEIGHT_THRESHOLD 2 // Seuil de différence en grammes
#define STABILIZATION_TIME 10000 // Temps de stabilisation en millisecondes

// Variables globales
String macAddress = "";
ApiClient* apiClient = nullptr;
HX711 scale(5, 4);
float calibration_factor = -106;
int lastWeight = 0;
int currentWeight = 0;
unsigned long lastWeightChangeTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Démarrage...");

  // Initialisation de la balance HX711
  scale.set_scale(calibration_factor);
  scale.tare();  // Mise à zéro de la balance
  Serial.println("Balance initialisée");

  // Connexion au WiFi
  if (connectToWiFi(WIFI_SSID, WIFI_PASSWORD)) {
    // Obtention de l'adresse MAC
    macAddress = getMacAddress();
    Serial.print("Adresse MAC: ");
    Serial.println(macAddress);

    // IMPORTANT : Ajouter un délai pour stabiliser la connexion
    Serial.println("Attente de stabilisation de la connexion WiFi...");
    delay(10000);

    // Création des credentials
    Credentials apiCredentials(API_USERNAME, API_PASSWORD);

    // Initialisation du client API
    apiClient = new ApiClient(API_BASE_URL, apiCredentials);

    // Enregistrement de la balance
    apiClient->registerBalance(macAddress, macAddress);
  }
}

void loop() {
  // Lecture du poids actuel
  scale.set_scale(calibration_factor);
  float units = scale.get_units(10);
  
  // Si négatif, on considère comme zéro
  if (units < 0) {
    units = 0.00;
  }
  
  // Conversion en entier pour faciliter la comparaison
  currentWeight = round(units);
  
  // Affichage du poids actuel
  Serial.print("Poids actuel: ");
  Serial.print(currentWeight);
  Serial.println(" grammes");
  
  // Vérification si le temps de stabilisation est écoulé
  if (millis() - lastWeightChangeTime >= STABILIZATION_TIME) {
    // Vérification si le poids a changé significativement
    if (abs(currentWeight - lastWeight) > WEIGHT_THRESHOLD) {
      Serial.print("Changement de poids détecté! Ancien: ");
      Serial.print(lastWeight);
      Serial.print("g, Nouveau: ");
      Serial.print(currentWeight);
      Serial.println("g");
      
      // Envoi des données à l'API si elle est disponible
      if (apiClient != nullptr) {
        bool success = apiClient->updateQuantity(macAddress, currentWeight);
        if (success) {
          Serial.println("Données de poids envoyées à l'API avec succès");
        } else {
          Serial.println("Échec de l'envoi des données à l'API");
        }
      } else {
        Serial.println("Client API non initialisé, impossible d'envoyer les données");
      }
      
      // Mise à jour du dernier poids connu
      lastWeight = currentWeight;
      // Réinitialisation du timer de stabilisation
      lastWeightChangeTime = millis();
    }
  }
  
  // Pause entre les lectures
  delay(1000);
}