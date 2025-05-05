🔌 Kitchen Embarqué

## 📝 Description

Ce projet est le composant embarqué qui gère les balances connectées pour mon système de cuisine intelligent. Il permet de :

- Connecter la balance via WiFi à l'API centrale
- Enregistrer la balance automatiquement sur le serveur
- Transmettre les données de pesée en temps réel
- Gérer l'étalonnage et la mise à zéro de la balance

Ce firmware s'exécute sur des modules ESP32 et communique avec l'API backend pour la gestion automatique des stocks d'ingrédients dans ma cuisine.

## 🛠️ Matériel nécessaire

- Module ESP32 (WiFi + Bluetooth)
- Module HX711 (amplificateur pour capteur de poids)
- Capteur de poids (cellule de charge)
- Fils de connexion
- Alimentation 5V (batterie ou adaptateur secteur)
- Boîtier pour l'installation (optionnel)

### 🔌 Schéma de connexion

- **ESP32 ↔ HX711**

  - ESP32 GPIO 13 → HX711 CLK
  - ESP32 GPIO 12 → HX711 DOUT
  - ESP32 3.3V → HX711 VCC
  - ESP32 GND → HX711 GND

- **HX711 ↔ Cellule de charge**
  - HX711 E+ → Cellule de charge fil rouge
  - HX711 E- → Cellule de charge fil noir
  - HX711 A+ → Cellule de charge fil blanc
  - HX711 A- → Cellule de charge fil vert

## 🛠️ Installation et configuration

### ⚙️ Configuration de l'environnement

Créez un fichier env.h dans le dossier config en copiant le fichier env-exemple.h :

```bash
cp src/config/env-exemple.h src/config/env.h
```

Configurez les paramètres WiFi et API dans le fichier env.h :

```c
// Configuration WiFi
#define WIFI_SSID "votre_reseau_wifi"
#define WIFI_PASSWORD "votre_mot_de_passe_wifi"

// Configuration API
#define API_BASE_URL "https://votre-api.com"
#define API_USERNAME "votre_nom_utilisateur"
#define API_PASSWORD "votre_mot_de_passe"
```

### 📚 Librairies requises

Pour compiler ce projet, vous aurez besoin d'installer les bibliothèques Arduino suivantes :

- Arduino_JSON
- HTTPClient
- WiFi
- HX711 (pour le capteur de poids)

Installez-les via le gestionnaire de bibliothèques de l'IDE Arduino.

## 📋 Architecture du projet

Le projet est organisé de façon modulaire :

- `kitchen-embarque.ino` : Point d'entrée principal
- `api` : Communication avec l'API backend
- `config` : Fichiers de configuration
- `network` : Gestion des connexions WiFi
- `scale` : Gestion du capteur de poids HX711
- `utils` : Fonctions utilitaires (encodage Base64, etc.)

## 🔄 Fonctionnement automatique

### 📲 Enregistrement automatique

Au démarrage, la balance :

1. Se connecte au réseau WiFi configuré
2. Récupère son adresse MAC unique
3. S'enregistre automatiquement dans la base de données via l'API
4. Aucune configuration manuelle n'est nécessaire côté matériel

### 🔗 Association dans l'interface

Pour utiliser la balance :

1. Accédez à l'interface web de gestion
2. Dans la section "Balances", vous verrez la nouvelle balance listée avec son adresse MAC
3. Associez simplement un ingrédient à cette balance depuis l'interface
4. La balance est immédiatement opérationnelle
