![logo](https://github.com/kevincsouto/Firmware_Esp32_Ocpp/blob/main/images/logo.png)

# Firmware Esp32 Ocpp

**Firmware Esp 32 Ocpp** é um cliente OCPP (Open Charge Point Protocol) desenvolvido para permitir a comunicação entre um carregador de veículo elétrico (EVSE) e um sistema central (Central System) em uma estação de carregamento elétrico.

Este projeto foi desenvolvido para atender às necessidades de carregamento de veículos elétricos em estações de recarga, permitindo a autenticação de usuários, monitoramento do status de carregamento, controle de carregadores e a troca de informações com sistemas centrais compatíveis com o protocolo OCPP.

## Recursos Principais

- **Comunicação OCPP**: Este cliente implementa a versão 1.6 do protocolo OCPP, permitindo a troca de mensagens padrão com sistemas centrais OCPP compatíveis.

- **Autorização RFID**: Os usuários podem autenticar seus cartões RFID para iniciar sessões de carregamento [EM FASE DE TESTES, POR ENQUANTO, APENAS VIA SERIAL].

- **Monitoramento em Tempo Real**: O cliente OCPP fornece informações em tempo real sobre o status de cada carregador, incluindo tensão, corrente, potência, energia e muito mais [MEDIÇÕES AINDA NÃO FORAM IMPLEMENTADAS].

- **Controle de Carregador**: A capacidade de iniciar, parar e reiniciar sessões de carregamento, bem como ativar e desativar carregadores, está disponível.

- **Interface Web**: Uma interface web NÃO está incluída para monitorar e controlar a estação de recarga através de um navegador. Indica-se a utilização do servidor Steve. 

## Requisitos de Configuração

- **Hardware**: Este cliente OCPP é projetado para ser executado em hardware compatível com Arduino, como o ESP8266 ou ESP32.

- **Bibliotecas**: São usadas bibliotecas adicionais, como `ArduinoJSON` e `ArduinoOcpp`, que devem ser instaladas antes de compilar o projeto.

## Uso

1. **Configuração**: Antes de usar o cliente OCPP, você deve configurar o hardware e definir as configurações específicas do seu sistema central no código-fonte.

2. **Compilação**: Compile o código-fonte usando a plataforma Arduino ou outra ferramenta compatível.

3. **Upload**: Carregue o firmware gerado em seu hardware.

4. **Operação**: Inicie o cliente OCPP em seu hardware e monitore a interface web ou use um sistema central OCPP compatível para interagir com a estação de recarga.

## Contribuições

Contribuições para este projeto são bem-vindas. Se você encontrar problemas, desejar adicionar novos recursos ou melhorias, ou quiser ajudar de alguma forma, fique à vontade para criar um problema (issue) ou enviar um pull request.

## Licença

Este projeto é licenciado sob a Licença MIT - consulte o arquivo [LICENSE](LICENSE) para obter detalhes.

---

![SteVe Logo](https://github.com/steve-community/steve/blob/master/src/main/resources/webapp/static/images/logo.png)

Para obter mais informações sobre o servidor SteVe, visite [https://github.com/steve-community/steve](https://github.com/steve-community/steve).

