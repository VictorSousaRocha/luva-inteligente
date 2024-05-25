#include <Arduino.h>

// --------------------

#include<WiFi.h>
#include<Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// --------------------

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

// --------------------

#define WIFI_SSID "NOME DO WIFI AQUI"
#define WIFI_PASSWORD "SENHA DO WIFI AQUI"

// --------------------

#define API_KEY "AIzaSyDNDQz9SrA6qy9GR9FG6q2qVBok8LnjV-Y"
#define DATABASE_URL "https://luva-inteligente-default-rtdb.firebaseio.com/"

// --------------------

unsigned long sendDataPrevMillis = 0;

// --------------------

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// --------------------

void setup() {
  debugln("SETUP");

  Serial.begin(9600);

  // --------------------

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  debug("conetando ao wifi");

  while (WiFi.status() != WL_CONNECTED)
  {
    debugln("ainda conectando"); 
    delay(300);
  }

  debugln();
  debug("Conectado ao ip: ");
  debugln(WiFi.localIP());
  debugln();

  // --------------------

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if(Firebase.signUp(&config, &auth, "", ""))
  {
    debugln("signUp OK");
    signupOK = true;
  }

  else 
  { 
    debugln(config.signer.signupError.message.c_str());
  }
  
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  debugln("LOOP");

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    
    if(Firebase.RTDB.setInt(&fbdo, "horario", 1))
    {
      Serial.println(); 
      Serial.println("salvo em ");
      Serial.println(fbdo.dataPath());
      Serial.println(fbdo.dataType());
    }
    
    if (Firebase.RTDB.getInt(&fbdo, "horario")) 
    {  
      if(fbdo.intData() == 1)
      {
        Serial.println("A");
      }
    }
    
    else 
    {
      Serial.println("Falhou ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(1000);
}