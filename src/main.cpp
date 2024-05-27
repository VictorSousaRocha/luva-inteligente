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

#define API_KEY "API KEY DO FIREBASE AQUI"
#define DATABASE_URL "URL DO FIREBASE AQUI"

// --------------------

unsigned long sendDataPrevMillis = 0;

// --------------------

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// --------------------

int readCloudInt(String var_name) 
{
  if (Firebase.RTDB.getInt(&fbdo, var_name)) 
  {  
    int returned_value = fbdo.intData();

    return returned_value;
  } 
  
  else 
  {
    debug("FAILED REASON: ");
    debug(fbdo.errorReason());
    debugln();

    return 9999999;
  }
}

void writeCloudInt(String var_name, int var_value) 
{
  if(Firebase.RTDB.setInt(&fbdo, var_name, var_value)) 
  {
    debug("SENT TO: ");
    debug(fbdo.dataPath());
    debug(" ");
    debug(fbdo.dataType());
    debug(" ");
    debug(var_value);
    debugln();
  } 
  
  else 
  {
    debug("FAILED REASON: ");
    debug(fbdo.errorReason());
    debugln();
  }
}

// --------------------

void setup() 
{
  debugln("SETUP");

  Serial.begin(115200);

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
  //debugln("LOOP");

  if (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0) {
    if (Firebase.ready() && signupOK) {
      sendDataPrevMillis = millis();
      
      writeCloudInt("horario", 1);
      
      int a = readCloudInt("horario");

      debugln(a);
    }
  }
}