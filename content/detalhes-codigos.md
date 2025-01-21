# Detalhamento do código dos sitemas embarcados

Nesta seção estarão disponíveis os códigos utilizados para o desenvolvimento dos sitemas embarcados tanto para o [suporte](#sistema-embarcado-do-suporte), como para o da [bicicleta](#sistema-embarcado-da-bicicleta).

## Link para o código em extenso
- [Sistema embarcado do suporte](/codes/suporte.c)
- [Sistema embarcado da bicicleta](/codes/bike.c)

# Sistema embarcado do suporte:

O suporte está equipado com sensores para a verificação de distância e de obstrução da passagem da trava, e um servo motor para movimentar a trava em posição de trancamento e destrancamento, LEDs para sinalização e um buzzer para alertas sonoros. Além disso, os eventos são registrados no SPIFFS com timestamps obtidos de um servidor NTP.

## Passo a passo e explicação do código desenvolvido


## Bibliotecas Utilizadas

O código utiliza as seguintes bibliotecas:

- `WiFi.h`: Para conexão à rede Wi-Fi.
- `PubSubClient.h`: Para comunicação MQTT com o Adafruit IO.
- `ESP32Servo.h`: Para controle do servo motor da trava.
- `WiFiUdp.h` e `TimeLib.h`: Para sincronização de tempo via NTP.
- `NTPClient.h`: Cliente NTP para obtenção de timestamps.
- `SPIFFS.h`: Para armazenamento de logs no ESP32.

## Configuração de Rede e MQTT

```cpp
#define IO_USERNAME ""
#define IO_KEY ""
const char* ssid = "";
const char* password = "";
const char* mqttserver = "";
const int mqttport = 1883;
```

Aqui são definidas as credenciais do Wi-Fi e do Adafruit IO.

## Configuração dos Pinos

```cpp
const int servoPin = 4;
const int trigPin = 14;
const int echoPin = 13;
const int ledVerde = 25;
const int ledVermelho = 26;
const int buzzerPin = 27;
const int sensorInfravermelho = 5;
```

Os pinos do ESP32 são atribuídos para os diferentes componentes.

## Funções Detalhadas

### `setupWiFi()`

```cpp
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
```

Esta função estabelece a conexão Wi-Fi do ESP32 e exibe o endereço IP obtido.

### `callback()`

```cpp
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
```

Esta função recebe mensagens MQTT e atualiza as variáveis de controle do sistema.

### `reconnect()`

```cpp
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
```

Tenta reconectar ao MQTT caso a conexão seja perdida.

### `enviarSPIFFS()`

```cpp
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
```

Registra eventos importantes no sistema de arquivos do ESP32.

### `medirDistancia()`

```cpp
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
```

Usa um sensor ultrassônico para medir a distância e detectar a presença da bicicleta.

### `verificarObstrucao()`

```cpp
bool verificarObstrucao() {
  return digitalRead(sensorInfravermelho) == LOW;
}
```

Utiliza um sensor infravermelho para verificar obstruções na trava.

### `destravarTrava()` e `travarTrava()`

```cpp
void destravarTrava() {
  Serial.println("Abrindo a trava...");
  emitirBip(2, 200);

  for(int pos = 0; pos < 180; pos++){
    travaServo.write(pos);
    delay(5);
  }
}

void travarTrava() {
  Serial.println("Fechando a trava...");
  emitirBip(2, 200);

   for(int pos = 180; pos >= 0; pos--){
    travaServo.write(pos);
    delay(5);
  }
}
```

Funções responsáveis pelo controle da trava através do servo motor.

# Sistema embarcado da bicicleta


Este código implementa um sistema de monitoramento de perímetro para uma bicicleta utilizando um ESP32. O dispositivo se conecta ao Wi-Fi e utiliza o protocolo MQTT para receber dados de latitude e longitude via Adafruit IO. Caso a bicicleta saia de um perímetro pré-definido, um alerta é publicado em um feed MQTT e um buzzer é ativado para notificar o usuário.

O código inclui funcionalidades como:

- Conexão com Wi-Fi.
- Comunicação via MQTT.
- Cálculo de distância utilizando a fórmula de Haversine.
- Monitoramento contínuo da posição da bicicleta.
- Publicação de alertas quando a bicicleta sai do perímetro.

---

## Explicação das Funções

### setupWiFi()

Esta função estabelece a conexão do ESP32 com a rede Wi-Fi especificada. O código entra em um loop até que a conexão seja estabelecida e exibe o endereço IP do dispositivo.

### callback()

Esta função é chamada sempre que o ESP32 recebe uma mensagem via MQTT. Dependendo do tópico recebido (latitude ou longitude), os valores são armazenados para posterior verificação do perímetro.

### reconnect()

Tenta reconectar o ESP32 ao broker MQTT caso a conexão tenha sido perdida. Se a conexão for bem-sucedida, o ESP32 volta a se inscrever nos feeds de latitude e longitude.

### getDistance(flat1, flon1, flat2, flon2)

```cpp
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
```

Utiliza a fórmula de Haversine para calcular a distância entre dois pontos geográficos com base em suas coordenadas de latitude e longitude.

### checkPerimeter()

```cpp
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
```

Verifica a posição atual do ESP32 em relação ao ponto de referência. Caso a distância ultrapasse o raio permitido, o buzzer é ativado e um alerta é enviado via MQTT.

### setup()

Configura o ESP32 inicializando a comunicação serial, o pino do buzzer e estabelecendo a conexão Wi-Fi e MQTT.

### loop()

```cpp
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
```
Executa continuamente a lógica do código. Garante que a conexão MQTT esteja ativa e verifica periodicamente a posição da bicicleta para monitorar o perímetro.

---

Este código fornece um sistema eficiente de monitoramento para garantir que a bicicleta permaneça dentro de uma área segura, alertando quando sair do perímetro estabelecido.






