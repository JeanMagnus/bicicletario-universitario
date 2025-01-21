#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <SPIFFS.h>

// Configurações de Wi-Fi e Adafruit IO
#define IO_USERNAME ""
#define IO_KEY ""
const char* ssid = "";
const char* password = "";

const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

// Pinos do servo motor, sensores, LEDs e buzzer
const int servoPin = 4;
const int trigPin = 14;    
const int echoPin = 13;
const int ledVerde = 25;
const int ledVermelho = 26;
const int buzzerPin = 27; // Novo pino para o buzzer
const int sensorInfravermelho = 5; // Novo pino para o sensor infravermelho

Servo travaServo;
bool comandoDestravarParaRetirada = false;
bool comandoDestravarParaDevolucao = false;
bool bicicletaEmUso = false;
bool estadoAtualBicicleta = false; // Para verificar mudanças de estado
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000;

// Controle de uso
unsigned long tempoInicioUso = 0;
const unsigned long tempoLimiteUso = 60000; // 1 minuto (60 segundos)
bool alertaTempoExcedido = false;

WiFiClient espClient;
PubSubClient client(espClient);
const char* ntpServer = "pool.ntp.org";
const long timeStampUTC = -10800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timeStampUTC);

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida em ");
  Serial.print(topic);
  Serial.print(": ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (String(topic).endsWith("destravar") && message == "destravar") {
    comandoDestravarParaRetirada = true;
  }
  if (String(topic).endsWith("travar") && message == "travar") {
    comandoDestravarParaDevolucao = true;
  }
}

bool reconnect() {
  if (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32-BikeUnlock";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("Conectado ao Adafruit IO!");
      client.subscribe((String(IO_USERNAME) + "/feeds/destravar").c_str());
      client.subscribe((String(IO_USERNAME) + "/feeds/travar").c_str());
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

void enviarSPIFFS(String log) {
  timeClient.update();
  String timeString = timeClient.getFormattedTime();
  time_t rawTime = timeClient.getEpochTime();
  struct tm *ptm = localtime(&rawTime);
  String dateString = String(ptm->tm_mday) + "-" + String(ptm->tm_mon + 1) + "-" + String(ptm->tm_year + 1900);

  File logFile = SPIFFS.open("/registro_bike.txt", "a");
  if (!logFile) {
    Serial.println("Falha ao abrir o arquivo para escrita!");
    return;
  }

  String logEntry = log + " às " + timeString + " horas, no dia: " + dateString + "\n----------------------------------------\n";
  logFile.print(logEntry);
  logFile.close();
  Serial.println("Log atualizado no SPIFFS: " + logEntry);
}

void lerArquivoSPIFFS(const char* caminho) {
  File arquivo = SPIFFS.open(caminho, "r");
  if (!arquivo) {
    Serial.print("Falha ao abrir o arquivo: ");
    Serial.println(caminho);
    return;
  }
  Serial.print("Conteúdo do arquivo ");
  Serial.println(caminho);

  while (arquivo.available()) {
    Serial.write(arquivo.read());
  }
  arquivo.close();
}

void atualizarSituacao(bool emUso) {
  if (emUso != estadoAtualBicicleta) {
    estadoAtualBicicleta = emUso;
    String mensagem = emUso ? "Bicicleta em uso" : "Bicicleta disponível";
    client.publish((String(IO_USERNAME) + "/feeds/situacao").c_str(), mensagem.c_str());
    Serial.println("Situação da bicicleta atualizada: " + mensagem);
  }
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distancia = (duracao * 0.034) / 2;
  return distancia;
}

bool verificarObstrucao() {
  return digitalRead(sensorInfravermelho) == LOW; // Retorna true se o sensor detectar algo
}

void emitirBip(int quantidade, int intervalo) {
  for (int i = 0; i < quantidade; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(intervalo);
    digitalWrite(buzzerPin, LOW);
    delay(intervalo);
  }
}

void destravarTrava() {
  Serial.println("Abrindo a trava...");
  emitirBip(2, 200);
  //travaServo.write(180);
  //delay(50);
  //travaServo.write(90);
  //delay(50);
  for(int pos = 0; pos < 180; pos++){
    travaServo.write(pos);
    delay(5);
  }
}

void travarTrava() {
  Serial.println("Fechando a trava...");
  emitirBip(2, 200);
  //travaServo.write(0);
  //delay(800);
  //travaServo.write(90);
  //delay(500);
   for(int pos = 180; pos >= 0; pos--){
    travaServo.write(pos);
    delay(5);
  }
}

void setup() {
  Serial.begin(115200);

  travaServo.attach(servoPin);
  travaServo.write(90);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorInfravermelho, INPUT);

  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledVermelho, LOW);

  setupWiFi();
  client.setServer(mqttserver, mqttport);
  client.setCallback(callback);

  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar SPIFFS");
    return;
  }
  Serial.println("SPIFFS montado com sucesso!");
  lerArquivoSPIFFS("/registro_bike.txt");
}

void loop() {
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > reconnectInterval) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }

  if (comandoDestravarParaRetirada) {
    destravarTrava();
    Serial.println("Esperando bicicleta ser removida...");
    while (medirDistancia() < 5.0) {
      emitirBip(1, 500);
      delay(500);
    }

    travarTrava();
    bicicletaEmUso = true;
    comandoDestravarParaRetirada = false;
    tempoInicioUso = millis();
    alertaTempoExcedido = false;
    atualizarSituacao(true);
    Serial.println("Bicicleta removida e trava fechada.");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    enviarSPIFFS("Bicicleta retirada");
  }

  if (comandoDestravarParaDevolucao) {
    destravarTrava();
    Serial.println("Esperando bicicleta ser colocada...");

    int leiturasValidas = 0;  // Contador de leituras estáveis
    const int leiturasNecessarias = 2;  // Quantidade de leituras consistentes

    unsigned long tempoInicio = millis();
    const unsigned long timeout = 30000;  // Timeout de 30 segundos

    while (millis() - tempoInicio < timeout) {
        float distancia = medirDistancia();
        bool obstrucao = verificarObstrucao();

        if (distancia < 3.0 && !obstrucao) {
            leiturasValidas++;  // Contabiliza leitura válida
        } else {
            leiturasValidas = 0;  // Reseta contador se houver erro
        }

        if (leiturasValidas >= leiturasNecessarias) {
            break;  // Sai do loop apenas se houver leituras estáveis
        }

        // Mensagens de status baseadas na leitura real
        if (distancia >= 3.0) {
            Serial.println("Bicicleta não detectada no suporte. Ajuste a posição.");
            emitirBip(1, 500);
        } else if (obstrucao) {
            Serial.println("Aro detectado na frente da trava. Aguarde a desobstrução.");
            emitirBip(1, 100);
        }

        delay(500);  // Pequena pausa para evitar sobrecarga no loop
    }

    if (leiturasValidas >= leiturasNecessarias) {
        Serial.println("Bicicleta corretamente posicionada. Fechando a trava...");
        travarTrava();
        bicicletaEmUso = false;
        comandoDestravarParaDevolucao = false;
        alertaTempoExcedido = false;
        atualizarSituacao(false);
        Serial.println("Bicicleta colocada e trava fechada.");
        client.publish((String(IO_USERNAME) + "/feeds/aviso").c_str(), "Tempo não excedido.");
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledVermelho, LOW);
        enviarSPIFFS("Bicicleta devolvida");
    } else {
        Serial.println("Tempo esgotado para a devolução da bicicleta.");
        client.publish((String(IO_USERNAME) + "/feeds/aviso").c_str(), "Erro: Tempo limite para devolução atingido.");
    }
}
  if (bicicletaEmUso) {
    unsigned long tempoAtual = millis();
    unsigned long tempoDecorrido = (tempoAtual - tempoInicioUso) / 1000;

    if (!alertaTempoExcedido && tempoDecorrido <= 60) {
      Serial.println("Tempo decorrido: " + String(tempoDecorrido) + " segundos.");
      delay(1000);
    }

    if (!alertaTempoExcedido && tempoDecorrido >= 60) {
      alertaTempoExcedido = true;
      Serial.println("ALERTA: Tempo limite de uso atingido!");
      client.publish((String(IO_USERNAME) + "/feeds/aviso").c_str(), "Tempo limite de uso atingido! Por favor, devolva a bicicleta ao suporte.");
    }

    if (alertaTempoExcedido) {
      emitirBip(1, 500);
    }
  }
}
