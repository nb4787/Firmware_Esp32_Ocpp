/*
 * matth-x and Kevin Souto/MicroOcpp
 * Copyright Matthias Akstaller 2019 - 2023
 * MIT License
 */

//Library definitions
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOcpp.h>
//End library definitions

//Wi-Fi connection data
#define STASSID "GreenV-IoT"
#define STAPSK  "Greenv@135"
//End Wi-Fi connection data

//  Settings which worked for my SteVe instance:
///*
#define OCPP_HOST "192.168.0.176"
#define OCPP_PORT 8180
#define OCPP_URL "ws://192.168.0.176:8180/steve/websocket/CentralSystemService/EMMA"
//*/

/*
#define OCPP_HOST "steve.greenv.com.br"
#define OCPP_PORT 8080
#define OCPP_URL "wss://steve.greenv.com.br/steve/websocket/CentralSystemService/CHARGEBOXNAME"
*/
//End of settings 


// Pino de controle do carregador
const int chargerControlPin = 22;

// Estado atual do carregador
bool isCharging = false;

void onAuthorizeResponse(JsonObject response);

void setup() 
{
    //Initialize Serial and Wi-Fi
    Serial.begin(115200);
    Serial.print(F("[main] Wait for WiFi: "));
    //End of initialie Serial and Wi-Fi

    //WiFi definitions and connection 
    WiFi.begin(STASSID, STAPSK);
    
    while (!WiFi.isConnected()) 
    {
        Serial.print('.');
        delay(1000);
    }

    Serial.println(F(" connected!"));
    //End WiFi connection

    //Initialize the OCPP library
    ocpp_initialize(OCPP_HOST, OCPP_PORT, OCPP_URL, "Generic Model", "Greenv Mobility");
    //End os initialize OCPP library 
    
    // Defina o pino do carregador como uma saída digital
    pinMode(chargerControlPin, OUTPUT);

    // Inicialmente, o carregador está desligado
    isCharging = false;

    // Desligue o carregador (opcional)
    digitalWrite(chargerControlPin, LOW);

}

void loop() 
{
  // Verifique a entrada serial para ID Tags
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Imprima a ID Tag para depuração
    Serial.print("ID Tag lida: ");
    Serial.println(input);

    // Verifique se a ID Tag é válida
    authorize(input.c_str(), onAuthorizeResponse); 
  }

  // Mantenha a comunicação OCPP ativa
  ocpp_loop();
}


void onAuthorizeResponse(JsonObject response) {
  const char *status = response["idTagInfo"]["status"];
  if (strcmp(status, "Accepted") == 0) {
    if (!isCharging) {
      // Inicie o carregamento
      startTransaction(getTransactionIdTag());
      isCharging = true;
      
      // Ligue o carregador (defina o pino como alto)
      digitalWrite(chargerControlPin, HIGH);
      Serial.println("Carregador ligado");
    } else {
      // Encerre o carregamento
      endTransaction(getTransactionIdTag(), "Fim do Carregamento");
      isCharging = false;
      
      // Desligue o carregador (defina o pino como baixo)
      digitalWrite(chargerControlPin, LOW);
      Serial.println("Carregador desligado");
    }
  } else {
    Serial.println("ID Tag não autorizada pelo servidor");
  }
}
