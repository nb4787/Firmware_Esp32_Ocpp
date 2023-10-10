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

const int connectorId = 1;

void freeUseFlow();
void authorizedUseFlow();

bool isEVConnected()
{
  return true;
}

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
    
    setConnectorPluggedInput(isEVConnected);

}

void loop() 
{
    //Do all OCPP stuff (process WebSocket input, send recorded meter values to Central System, etc.)
    ocpp_loop();

/*    
    if (true) 
    {
      freeUseFlow();
    }
    else
    {
      authorizedUseFlow();
    }
*/
  if (isEVConnected() && !isTransactionActive()) {
    // Inicie uma transação sem verificar credenciais
    beginTransaction_authorized("WARNER");

    // Aguarde algum tempo (simulado)
    delay(5000); // Espere por 5 segundos

    // Termine a transação (simulado)
    endTransaction("WARNER", "Fim da Transação");

    // Aguarde algum tempo (simulado)
    delay(5000); // Espere por mais 5 segundos
  }



}

void freeUseFlow()
{
/*
  if (!isOperative(connectorId) && !isTransactionRunning(connectorId))
  {

    stopEnergizingEV(connectorId);
    continue;
  }
  
  startEnergizingEV(connectorId); //Passagem de corrente sempre permitida quando freeVendEnabled

  if (isTransactionRunning(connectorId))
  {
    changeLedMode(CHARGING);
  }
  else
  {
    changeLedMode(UNLOCKED);
  }
  //Define o comportamento de quando o carregador começa a passar corrente para o carro
  if (isCharging(connectorId) && !isTransactionRunning(connectorId))
  {
    resetPzems(numConnectors);
    Serial.print("Inicio da transacao");
    beginTransaction(ID_TAG.c_str() ,connectorId);
  }
  if (!isCharging(connectorId) && isTransactionRunning(connectorId))
  {
    endTransaction("EVDisconnected", connectorId);
  }
*/
}