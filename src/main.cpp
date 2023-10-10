/* matth-x and Kevin Souto/MicroOcpp
 * Copyright Matthias Akstaller 2019 - 2023
 * MIT License
 */

//Inicialização das bibliotecas
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOcpp.h> //A biblioteca atualizada é <MicroOcpp.h>
//Fim da inicialização das bibliotecas

//Definição dos dados da rede Wi-Fi
/*
#define STASSID "Rorschach"
#define STAPSK  "espectral"
*/

///*
#define STASSID "Ozymandias"
#define STAPSK  "emmastone"
//*/

/*
#define STASSID "Greenv"
#define STAPSK  "AL@Greenv23*"
*/
//Fim da definição dos dados da rede Wi-Fi 

//Início da configuração dos dados para conexão Web Socket 
///*
#define OCPP_HOST "189.93.47.58"
#define OCPP_PORT 8180
#define OCPP_URL  "ws://189.93.47.58:8180/steve/websocket/CentralSystemService/SAMARA"
//*/

/*
#define OCPP_HOST "centralsystem.dev.greenv.com.br"
#define OCPP_PORT 801

#define OCPP_URL "ws://centralsystem.dev.greenv.com.br/ocpp/1.6/MEDIDOR-T03"
*/
//Fim das configurações dos dados para conexão Web Socket

int selectedConnector = -1;

void setup()
{
    Serial.begin(115200);

    Serial.println(F("[main] Wait for WiFi: "));

    WiFi.begin(STASSID, STAPSK);
    while (!WiFi.isConnected())
    {
        Serial.print('.');
        delay(1000);
    }

    Serial.println(F(" connected!"));

    /*mocpp_initialize*/ocpp_initialize(OCPP_HOST, OCPP_PORT, OCPP_URL, "Generic Charger", "Greenv Mobility");
}

void loop()
{
    /*mocpp_loop*/ocpp_loop();

    if (selectedConnector == -1)
    {
        Serial.println(F("[main] Selecione o número do conector (1, 2, etc.): "));
        while (Serial.available() == 0)
        {
            // Aguarda até que o usuário insira o número do conector.
        }
        selectedConnector = Serial.parseInt();
        Serial.printf("[main] Conector selecionado: %d\n", selectedConnector);
    }
    else
    {
        if (ocppPermitsCharge(selectedConnector))
        {
            // OCPP está configurado e a transação está em andamento.
            // Você pode ligar a alimentação do plugue do veículo elétrico aqui.
        }
        else
        {
            // Nenhuma transação em andamento no momento.
            // Você pode desligar a alimentação do plugue do veículo elétrico aqui, se necessário.
        }

        if (Serial.available() > 0)
        {
            String idTag = Serial.readStringUntil('\n');
            idTag.trim();

            if (!getTransaction(selectedConnector))
            {
                Serial.printf("[main] Iniciar transação com idTag %s no conector %d\n", idTag.c_str(), selectedConnector);
                auto ret = beginTransaction(idTag.c_str(), selectedConnector);

                if (ret)
                {
                    Serial.println(F("[main] Transação iniciada. A biblioteca OCPP enviará um StartTransaction quando" \
                                     " ConnectorPlugged Input se tornar verdadeiro e se a autorização for bem-sucedida."));
                }
                else
                {
                    Serial.println(F("[main] Nenhuma transação iniciada"));
                }
            }
            else
            {
                if (idTag.equals(getTransactionIdTag(selectedConnector)))
                {
                    Serial.printf("[main] Encerrar transação com cartão RFID no conector %d\n", selectedConnector);
                    endTransaction(nullptr, nullptr, selectedConnector);
                    selectedConnector = -1; // Reseta o conector selecionado para que ele pergunte novamente
                }
                else
                {
                    Serial.println(F("[main] Não é possível encerrar a transação com o cartão RFID (cartão diferente?)"));
                }
            }
        }
    }
}
