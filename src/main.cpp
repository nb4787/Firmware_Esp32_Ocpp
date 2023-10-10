/*
 * matth-x and Kevin Souto/MicroOcpp
 * Copyright Matthias Akstaller 2019 - 2023
 * MIT License
*/

//Library definitions
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;
#elif defined(ESP32)
#include <WiFi.h>
#else
#error only ESP32 or ESP8266 supported at the moment
#endif

#include <ArduinoOcpp.h>   //Actually is used MicroOcpp
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

void setup() 
{
    //Initialize Serial and Wi-Fi
    Serial.begin(115200);
    Serial.print(F("[main] Wait for WiFi: "));
    //End of initialie Serial and Wi-Fi

    #if defined(ESP8266)
        WiFiMulti.addAP(STASSID, STAPSK);
        
        while (WiFiMulti.run() != WL_CONNECTED) 
        {
            Serial.print('.');
            delay(1000);
        }
    
    #elif defined(ESP32)
        WiFi.begin(STASSID, STAPSK);
        
        while (!WiFi.isConnected()) 
        {
            Serial.print('.');
            delay(1000);
        }
    
    #else
    #error only ESP32 or ESP8266 supported at the moment
    #endif

    Serial.println(F(" connected!"));

    //Initialize the OCPP library
    /*mocpp_initialize*/ocpp_initialize(OCPP_HOST, OCPP_PORT, OCPP_URL, "My Charging Station", "Greenv Mobility"); 
    //End os initialize OCPP library 
    
    //Integrate OCPP functionality. You can leave out the following part if your EVSE doesn't need it.
    setEnergyMeterInput([]() 
    {
        //Take the energy register of the main electricity meter and return the value in watt-hours
        return 0.f;
    });

    setSmartChargingCurrentOutput([](float limit) 
    {
        //Set the SAE J1772 Control Pilot value here
        Serial.printf("[main] Smart Charging allows maximum charge rate: %.0f\n", limit);
    });

    setConnectorPluggedInput([]() 
    {
        //Return true if an EV is plugged to this EVSE
        return false;
    });
    //... see MicroOcpp.h for more settings
}

void loop() 
{
    //Do all OCPP stuff (process WebSocket input, send recorded meter values to Central System, etc.)
    /*mocpp_loop*/ocpp_loop();
    
    //Energize EV plug if OCPP transaction is up and running
    if (ocppPermitsCharge()) 
    {
      //OCPP set up and transaction running. Energize the EV plug here
      //Serial.println("RELE ON");
    } 
    else 
    {
      //No transaction running at the moment. De-energize EV plug
      //Serial.println("RELE OFF");
    }

    //Use NFC reader to start and stop transactions
    if (/* RFID chip detected? */ false) 
    {
        String idTag = "0123456789ABCD"; //e.g. idTag = RFID.readIdTag();

        if (!getTransaction()) 
        {
            //No transaction running or preparing. Begin a new transaction
            Serial.printf("[main] Begin Transaction with idTag %s\n", idTag.c_str());

            /*
             * Begin Transaction. The OCPP lib will prepare transaction by checking the Authorization
             * and listen to the ConnectorPlugged Input. When the Authorization succeeds and an EV
             * is plugged, the OCPP lib will send the StartTransaction
            */
            
            auto ret = beginTransaction(idTag.c_str());

            if (ret) 
            {
                Serial.println(F("[main] Transaction initiated. OCPP lib will send a StartTransaction when" \
                                 "ConnectorPlugged Input becomes true and if the Authorization succeeds"));
            } 
            else 
            {
                Serial.println(F("[main] No transaction initiated"));
            }
        } 
        else 
        {
            //Transaction already initiated. Check if to stop current Tx by RFID card
            if (idTag.equals(getTransactionIdTag())) {
                //Card matches -> user can stop Tx
                Serial.println(F("[main] End transaction by RFID card"));

                endTransaction();
            } 
            else 
            {
                Serial.println(F("[main] Cannot end transaction by RFID card (different card?)"));
            }
        }
    }
    //... see MicroOcpp.h for more possibilities
}