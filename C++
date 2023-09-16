class MqttWiFiConnection {
public:
    MqttWiFiConnection(const char* ssid, const char* password, const char* mqttBroker, int mqttPort, const char* clientId)
        : ssid(ssid), password(password), mqttBroker(mqttBroker), mqttPort(mqttPort), clientId(clientId) {
    }

    bool connect() {
        // Inicialize a conexão Wi-Fi
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Conectando ao WiFi...");
        }

        // Inicialize o cliente MQTT
        mqttClient.setClient(wifiClient);
        mqttClient.setServer(mqttBroker, mqttPort);
        mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
            this->mqttCallback(topic, payload, length);
        });

        if (mqttClient.connect(clientId)) {
            Serial.println("Conexão MQTT bem-sucedida");
            return true;
        } else {
            Serial.println("Falha na conexão MQTT");
            return false;
        }
    }

    void disconnect() {
        if (mqttClient.connected()) {
            mqttClient.disconnect();
        }
        WiFi.disconnect();
    }

    bool isConnected() const {
        return WiFi.status() == WL_CONNECTED && mqttClient.connected();
    }

    void publish(const char* topic, const char* message) {
        if (mqttClient.connected()) {
            mqttClient.publish(topic, message);
        }
    }

    void subscribe(const char* topic) {
        if (mqttClient.connected()) {
            mqttClient.subscribe(topic);
        }
    }

    void loop() {
        if (mqttClient.connected()) {
            mqttClient.loop();
        }
    }

private:
    const char* ssid;
    const char* password;
    const char* mqttBroker;
    int mqttPort;
    const char* clientId;
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    void mqttCallback(char* topic, byte* payload, unsigned int length) {
        // Lide com mensagens MQTT recebidas aqui
    }
};

int main() {
    // Exemplo de uso da classe MqttWiFiConnection
    MqttWiFiConnection mqttWiFi("NomeDaRede", "SenhaDaRede", "broker.mqtt.com", 1883, "MeuClienteMQTT");

    if (mqttWiFi.connect()) {
        mqttWiFi.subscribe("topico/subscribe");
        mqttWiFi.publish("topico/publish", "Mensagem de teste MQTT");
        Serial.println("Conexão estabelecida e mensagens MQTT publicadas/assinadas.");
    } else {
        Serial.println("Falha na conexão Wi-Fi/MQTT.");
    }

    while (mqttWiFi.isConnected()) {
        mqttWiFi.loop();
        // Realize outras operações, se necessário
    }

    mqttWiFi.disconnect();
    Serial.println("Desconectado da rede Wi-Fi e do broker MQTT.");

    return 0;
}

