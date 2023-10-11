
//Bibliotecas utilizadas no projeto
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOcpp.h>

#define STASSID "Rorschach" //Coloque SSID de sua rede aqui
#define STAPSK  "espectral" //Coloque a Senha de sua rede aqui

//Dados do servidor OCPP em que está se conectando
#define OCPP_HOST "192.168.0.178" //Host local não inclui porta
#define OCPP_PORT 8180            //Porta em que está configurada   
#define OCPP_URL "ws://192.168.0.178:8180/steve/websocket/CentralSystemService/SAMARA" 
/*No link websocket necessariamente tem que incluir o nome de um carregador previamente
cadastrado em seu sistema OCPP */

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

    ocpp_initialize(OCPP_HOST, OCPP_PORT, OCPP_URL, "Generic Charger", "Firmware Esp32 Ocpp");

}

void loop() {
    ocpp_loop();

    if (ocppPermitsCharge()) 
    {
        // OCPP está configurado e a transação está em andamento.
        // Você pode ligar a alimentação do plugue do veículo elétrico aqui.
    } 
    else 
    {
        // Nenhuma transação em andamento no momento.
        // Você pode desligar a alimentação do plugue do veículo elétrico aqui, se necessário.
    }

    // Leitura da ID do RFID via Serial
    if (Serial.available() > 0) 
    {
        String idTag = Serial.readStringUntil('\n');
        idTag.trim();

        if (!getTransaction()) 
        {
            // Nenhuma transação em andamento. Inicie uma nova transação.
            Serial.printf("[main] Iniciar transação com idTag %s\n", idTag.c_str());

            auto ret = beginTransaction(idTag.c_str());

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
            // Transação já iniciada. Verifique se deve encerrar a transação com o cartão RFID atual.
            if (idTag.equals(getTransactionIdTag())) {
                // O cartão corresponde - o usuário pode encerrar a transação.
                Serial.println(F("[main] Encerrar transação com cartão RFID"));

                endTransaction();
            } 
            else 
            {
                Serial.println(F("[main] Não é possível encerrar a transação com o cartão RFID (cartão diferente?)"));
            }
        }
    }
}
