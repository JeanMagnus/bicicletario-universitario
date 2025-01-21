#include <WiFi.h>
#include <PubSubClient.h>

// Configurações de Wi-Fi e Adafruit IO
#define IO_USERNAME ""
#define IO_KEY ""
const char* ssid = "";
const char* password = "";

const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

WiFiClient espClient;
PubSubClient client(espClient);

// Coordenadas iniciais para verificar o perímetro
float latInicial = -5.82318; // Latitude do ESP32
float lonInicial = -35.20514; // Longitude do ESP32
float raioPerimetro = 8; // Raio permitido (em metros)

// Pino do buzzer
const int buzzerPin = 27;

// Variáveis de controle
unsigned long lastReconnectAttempt = 0;
unsigned long lastLatitudeLongitudeCheck = 0;
float latitude, longitude; // Variáveis para armazenar coordenadas

// Função para conectar ao Wi-Fi
void setupWiFi() {
  delay(10);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Callback para processar mensagens MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic).endsWith("longitude")) {
    longitude = message.toFloat();
    Serial.print("Longitude recebida: ");
    Serial.println(longitude);
  }

  if (String(topic).endsWith("latitude")) {
    latitude = message.toFloat();
    Serial.print("Latitude recebida: ");
    Serial.println(latitude);
  }
}

// Função para reconectar ao broker MQTT
bool reconnect() {
  if (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32-PerimeterMonitor";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("Conectado ao Adafruit IO!");
      client.subscribe((String(IO_USERNAME) + "/feeds/longitude").c_str());
      client.subscribe((String(IO_USERNAME) + "/feeds/latitude").c_str());
      return true;
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s...");
      return false;
    }
  }
  return true;
}

// Função para calcular a distância entre dois pontos geográficos usando a fórmula de Haversine
float getDistance(float flat1, float flon1, float flat2, float flon2) {
  float dist_calc = 0;
  float dist_calc2 = 0;
  float diflat = 0;
  float diflon = 0;

  diflat = radians(flat2 - flat1);
  flat1 = radians(flat1);
  flat2 = radians(flat2);
  diflon = radians((flon2) - (flon1));

  dist_calc = (sin(diflat / 2.0) * sin(diflat / 2.0));
  dist_calc2 = cos(flat1);
  dist_calc2 *= cos(flat2);
  dist_calc2 *= sin(diflon / 2.0);
  dist_calc2 *= sin(diflon / 2.0);
  dist_calc += dist_calc2;

  dist_calc = (2 * atan2(sqrt(dist_calc), sqrt(1.0 - dist_calc)));
  dist_calc *= 6371000.0; // Converter para metros

  return dist_calc;
}

// Função para verificar se o ESP32 saiu do perímetro
void checkPerimeter() {
  float distance = getDistance(latitude, longitude, latInicial, lonInicial);
  Serial.print("Distância atual: ");
  Serial.println(distance);

  if (distance > raioPerimetro) {
    digitalWrite(buzzerPin, HIGH);  // Ativa o buzzer
    client.publish((String(IO_USERNAME) + "/feeds/perimetro").c_str(), "Fora do perímetro. Por favor retorne às dependências do campus.");
    Serial.println("Fora do perímetro! Buzzer ativado.");
  } else {
    digitalWrite(buzzerPin, LOW);   // Desativa o buzzer
    client.publish((String(IO_USERNAME) + "/feeds/perimetro").c_str(), "Dentro do perímetro.");
    Serial.println("Dentro do perímetro.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT); // Definir o pino do buzzer como saída

  setupWiFi(); // Conectar ao Wi-Fi

  client.setServer(mqttserver, mqttport);
  client.setCallback(callback);

  lastReconnectAttempt = millis();
}

void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }

  // Verifica o perímetro a cada 5 segundos
  if (millis() - lastLatitudeLongitudeCheck >= 5000) {
    checkPerimeter();
    lastLatitudeLongitudeCheck = millis();
  }
}