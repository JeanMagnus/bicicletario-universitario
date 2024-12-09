# Máquinas de Estados do Projeto

## Sumário
1. [Máquina de estado do suporte inteligente](#máquina-de-estado-do-suporte-)
2. [Máquina de estado do embarcado da bicicleta](#máquina-de-estados-do-sistema-embarcado-da-bicicleta-)

---

# Máquina de Estado do Suporte 🔒
![Máquina de Estado do Suporte](/images/ME-Suporte.png)

---

### **Descrição do Funcionamento**

A máquina de estado do **suporte inteligente** gerencia o ciclo de **destravar** e **travar** a bicicleta com base nas interações do usuário no aplicativo e nas leituras do sensor de distância. O sensor é essencial para determinar quando a bicicleta foi removida ou colocada novamente no suporte.

### **Estados:**
1. **START**: O sistema está no estado inicial, aguardando interação.
2. **DESTRAVAR**: O suporte destrava a bicicleta para que o usuário possa retirá-la.
3. **WAIT**: O suporte aguarda a remoção da bicicleta. O sensor detecta se a bicicleta foi removida ou ainda está no suporte.
4. **TRAVAR**: O suporte trava a bicicleta após o retorno dela ao suporte.

### **Eventos ou Ações:**
- **acessApp**: O evento que ocorre quando o usuário solicita o acesso à bicicleta no aplicativo. Este evento faz com que o sistema transite do estado **START** para o estado **DESTRAVAR**.
- **returnApp**: O evento que ocorre quando o usuário solicita a devolução da bicicleta ao suporte. Esse evento faz com que o sistema transite do estado **TRAVAR** de volta para o estado **DESTRAVAR**.
- **sensor**: O sensor detecta se a bicicleta foi removida do suporte ou se ela foi colocada de volta.

### **Fluxo de Estados e Eventos:**
1. **START → AcessApp**: O usuário solicita o acesso à bicicleta no aplicativo.
2. **AcessApp → DESTRAVAR**: O suporte destrava a bicicleta.
3. **DESTRAVAR → WAIT**: O sistema aguarda até que o sensor detecte se a bicicleta foi removida.  
   - **Se a bicicleta for removida (sensor')**, o sistema transita para o estado **TRAVAR**.
   - **Se a bicicleta não for removida (sensor)**, o sistema permanece em **WAIT**.
4. **WAIT → TRAVAR**: Quando o sensor detecta que a bicicleta foi removida do suporte (sensor'), o sistema vai para o estado **TRAVAR**.
5. **TRAVAR → returnApp**: O sistema aguarda o comando de devolução da bicicleta (returnApp).
6. **returnApp → DESTRAVAR**: O sistema destrava o suporte para permitir que o usuário devolva a bicicleta.
7. **DESTRAVAR → TRAVAR**:
   - **Se a bicicleta for colocada no suporte (sensor)**, o sistema transita para **TRAVAR**.
   - **Se a bicicleta não for colocada (sensor')**, o sistema permanece destravado até que a bicicleta seja inserida novamente.
8. **TRAVAR → START**: Se o sensor detecta a bicicleta corretamente no suporte e não há mais ações de devolução ou requisição, o sistema retorna ao estado inicial **START**, pronto para ser usado novamente.

---

# **Máquina de Estados do Sistema Embarcado da Bicicleta** 🚲

![Máquina de Estado da bicicleta](/images/ME-Bicicleta.png)


## **Descrição do Funcionamento**

O sistema embarcado na bicicleta é responsável por monitorar continuamente sua posição e comportamento, garantindo que ela opere dentro dos limites de segurança e funcionalidade. Ele utiliza dados de sensores (como GPS) para aferir se a bicicleta permanece dentro de um perímetro predefinido e toma ações baseadas nas condições observadas. O fluxo de estados garante que a bicicleta esteja sempre em conformidade com o uso planejado e emite alertas em caso de violações ou anomalias.

---

## **Estados**
1. **START**: O estado inicial. A bicicleta permanece nesse estado enquanto não estiver sendo usada.
2. **MONITORAR**: O sistema entra no modo de monitoramento contínuo assim que a bicicleta começa a ser utilizada.
3. **ANALISE**: Os dados monitorados são processados e analisados para determinar a necessidade de ações.
4. **ALERTA**: Um estado de alerta é ativado quando dados críticos ou violações (como ultrapassar os limites de perímetro) são detectados.

---

## **Eventos ou Ações**
- **bikeOn**: Evento que ocorre quando a bicicleta é ativada para uso, transicionando do estado **START** para **MONITORAR**.
- **newData**: Dados novos, como informações de GPS, são recebidos no sistema durante o monitoramento.
- **okData**: Dados que foram analisados e estão dentro dos parâmetros esperados, indicando operação normal.
- **solved**: Indica que uma anomalia ou problema detectado foi resolvido.
- **(bikeOn)’**: A bicicleta permanece no estado inicial se não for ativada.

---

## **Fluxo de Estados e Eventos**
1. **START → MONITORAR (bikeOn)**: Quando a bicicleta é ativada para uso, o sistema transita do estado inicial para o estado de monitoramento.
2. **MONITORAR → MONITORAR (newData)**: Durante o uso, o sistema recebe dados continuamente enquanto a bicicleta está em operação.
3. **MONITORAR → ANALISE (newData)**: Quando novos dados chegam, o sistema os envia para análise no estado **ANALISE**.
4. **ANALISE → MONITORAR (okData)**: Caso os dados analisados indiquem que a bicicleta está operando dentro do perímetro permitido e sem anomalias, o sistema retorna ao estado de monitoramento.
5. **ANALISE → ALERTA (dados críticos)**: Se os dados indicarem que a bicicleta ultrapassou os limites do perímetro ou detectaram outra anomalia, o sistema transita para o estado **ALERTA**.
6. **ALERTA → MONITORAR (solved)**: Quando a situação de alerta é resolvida (por intervenção manual ou automática), o sistema retorna ao estado de monitoramento.
7. **ALERTA → ALERTA (não resolvido)**: Caso o problema persista, o sistema permanece no estado de alerta, emitindo notificações para a central.
8. **START → START (bikeOn')**: A bicicleta permanece no estado inicial enquanto não for ativada.

---

## **Exemplo do Fluxo Operacional**
1. A bicicleta está estacionada no estado **START**.  
2. O usuário ativa a bicicleta para uso (**bikeOn**), e o sistema transita para **MONITORAR**.
3. Durante o uso, o GPS envia continuamente dados (**newData**) ao sistema, que são analisados em **ANALISE**.  
   - Se os dados indicarem que a bicicleta está dentro do perímetro permitido, o sistema retorna a **MONITORAR**.
   - Se os dados indicarem que a bicicleta ultrapassou o perímetro, o sistema transita para **ALERTA**.  
4. Em **ALERTA**, um aviso é enviado à central. O estado só retorna a **MONITORAR** quando a anomalia for resolvida (**solved**).  
5. Ao final do uso, a bicicleta pode ser desativada e retorna ao estado **START**.

---


