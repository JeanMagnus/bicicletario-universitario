# Diagrama de Blocos do Sistema 🚲

## **Descrição**

O **diagrama de blocos** é uma representação visual dos componentes principais do sistema e suas interações. Ele fornece uma visão geral da arquitetura do projeto, destacando como os módulos de hardware, software e comunicação se integram para realizar as funcionalidades do bicicletário.

---
## **Diagrama de Blocos do Suporte**

Abaixo está o diagrama de blocos representando a interação entre os componentes do suporte:

![Diagrama de Blocos do Sistema](/images/blocos-suporte.png)

---
## **Fluxograma do Suporte**

Abaixo está o fluxograma do suporte:

![Diagrama de Blocos do Sistema](/images/fluxo-suporte.png)

---

## **Diagrama de Blocos da bicicleta**

Abaixo está o diagrama de blocos representando a interação entre os componentes da bicicleta:

![Diagrama de Blocos do Sistema](/images/blocos-bike.png)

---
## **Fluxograma da Bicicleta**

Abaixo está o fluxograma da bicicleta:

![Diagrama de Blocos do Sistema](/images/fluxo-bicicleta.png)

---



## **Componentes Principais**

### **1. Módulo de Hardware**
O módulo de hardware inclui todos os dispositivos físicos que compõem o sistema embarcado. Estes componentes são responsáveis pela detecção, monitoramento e controle das bicicletas.

- **Microcontrolador (ESP32):** Responsável pelo processamento local, controle das travas e comunicação via Wi-Fi.
- **Sensor Ultrassônico (HC-SR04):** Detecta a presença ou ausência da bicicleta no suporte.
- **Atuador Linear:** Controla a trava do suporte, permitindo o destravamento ou travamento da bicicleta.
- **GPS (NEO-6M):** Realiza o monitoramento da localização em tempo real.
- **Acelerômetro (MPU6050):** Monitora movimentações e vibrações anômalas da bicicleta.

### **2. Módulo de Comunicação**
Este módulo gerencia a troca de informações entre os componentes do sistema e o aplicativo móvel.

- **Protocolo MQTT:** Usado para comunicação eficiente entre o microcontrolador e o servidor.
- **Wi-Fi (Integrado no ESP32):** Estabelece a conexão de rede para transmissão de dados.

### **3. Aplicativo Móvel**
O aplicativo é a interface do usuário com o sistema, permitindo a interação e o controle das bicicletas.

- **Funcionalidades:**
  - Reservar e desbloquear bicicletas.
  - Monitorar o status e a localização da bicicleta em tempo real.
  - Solicitar devolução da bicicleta ao suporte.

### **4. Servidor Central**
O servidor processa e armazena os dados enviados pelo hardware e gerencia as solicitações do aplicativo.

- **Funções do Servidor:**
  - Registro e análise de dados das bicicletas (localização, velocidade, etc.).
  - Gerenciamento de usuários e autenticação.
  - Emissão de alertas em caso de violação do perímetro.

---

## **Fluxo de Dados**

1. **Comando do Aplicativo:**
   - O usuário interage com o aplicativo para reservar, desbloquear ou devolver uma bicicleta.
   - O comando é enviado ao servidor, que o repassa ao microcontrolador via MQTT.

2. **Sensores e Atuação:**
   - O sensor ultrassônico detecta a presença da bicicleta no suporte.
   - O microcontrolador processa os dados e aciona o atuador linear para travar ou destravar a bicicleta.

3. **Monitoramento:**
   - O GPS e o acelerômetro enviam dados em tempo real para o servidor, garantindo a localização e a segurança da bicicleta.

4. **Retorno ao Usuário:**
   - O servidor processa os dados recebidos e atualiza o aplicativo em tempo real, exibindo o status da bicicleta.

---

