## **Casos de Uso do Sistema** 📚

A seguir, detalhamos os principais casos de uso do sistema do **Bicicletário Universitário**, cobrindo interações entre os usuários, o aplicativo, e o hardware embarcado.

Segue abaixo o diagrama de casos de uso:

![Diagrama de casos de uso](/images/casos-uso.png)

---



### **1. Desbloquear Bicicleta**
- **Ator principal:** Usuário do aplicativo  
- **Pré-condição:** O usuário deve estar cadastrado no aplicativo e autorizado a usar o serviço.  
- **Fluxo principal:**
  1. O usuário abre o aplicativo e seleciona uma bicicleta disponível.
  2. O sistema embarcado verifica se a bicicleta está presente no suporte.
  3. O sistema destrava a bicicleta após a confirmação do usuário.
  4. O aplicativo atualiza o status para "em uso" e exibe as informações da bicicleta.  
- **Pós-condição:** A bicicleta está desbloqueada e pronta para uso.

---

### **2. Retornar Bicicleta ao Suporte**
- **Ator principal:** Usuário do aplicativo  
- **Pré-condição:** A bicicleta está sendo usada.  
- **Fluxo principal:**
  1. O usuário retorna ao bicicletário e posiciona a bicicleta no suporte.
  2. O sensor ultrassônico detecta a presença da bicicleta.
  3. O sistema aciona a trava eletrônica para fixar a bicicleta ao suporte.
  4. O status da bicicleta é atualizado para "disponível" no sistema.  
- **Pós-condição:** A bicicleta está segura no suporte e disponível para o próximo uso.

---

### **3. Reservar Bicicleta**
- **Ator principal:** Usuário do aplicativo  
- **Pré-condição:** Bicicletas disponíveis no sistema.  
- **Fluxo principal:**
  1. O usuário seleciona uma bicicleta no aplicativo e solicita a reserva.
  2. O sistema verifica a disponibilidade e confirma a reserva.
  3. A bicicleta é marcada como "reservada" no sistema, com um limite de tempo para retirada.  
- **Pós-condição:** A bicicleta está reservada para o usuário.

---

### **4. Gerar Relatórios de Uso**
- **Ator principal:** Administrador do sistema  
- **Pré-condição:** Dados de uso acumulados no sistema.  
- **Fluxo principal:**
  1. O administrador acessa a interface de gerenciamento e solicita relatórios.
  2. O sistema compila dados sobre uso das bicicletas, trajetos mais frequentes, e métricas de impacto ambiental.
  3. Relatórios são gerados em formatos visuais ou exportáveis.  
- **Pós-condição:** Relatórios detalhados são gerados e disponíveis para consulta.

---

### **5. Visualizar Alertas de Segurança**
- **Ator Principal:** Administrador do Sistema  
- **Pré-condição:** O sistema deve ter emitido alertas de segurança (ex.: bicicleta fora do perímetro).  
- **Fluxo Principal:**
  1. O administrador acessa a interface de gerenciamento.  
  2. O sistema exibe uma lista de alertas recentes, incluindo detalhes como horário, localização da bicicleta e tipo de alerta (ex.: perímetro excedido, movimentação suspeita).  
  3. O administrador seleciona um alerta específico para obter mais informações.  
  . O sistema fornece informações detalhadas, incluindo o histórico de localização e os dados dos sensores relacionados ao evento.  
  5. O administrador toma ações corretivas, como enviar uma notificação ao usuário ou marcar o alerta como resolvido.  

- **Pós-condição:** O administrador visualiza os alertas de segurança e pode gerenciar ou resolver as ocorrências.

---

### **6. Monitorar Localização em Tempo Real**
- **Ator Principal:** Administrador do Sistema  
- **Pré-condição:** As bicicletas devem estar conectadas à rede e transmitindo dados de GPS.  
- **Fluxo Principal:**
  1. O administrador acessa a interface de gerenciamento e seleciona a opção de monitoramento.  
  2. O sistema exibe um mapa interativo mostrando a localização em tempo real de todas as bicicletas.  
  3. O administrador clica em uma bicicleta específica para visualizar informações adicionais, como status (em uso, disponível), trajetória recente e velocidade.  
  4. Caso uma bicicleta esteja fora do perímetro permitido, o sistema destaca a localização em vermelho e exibe uma notificação.  
  5. O administrador pode filtrar as bicicletas por status, localização ou horário de atividade para análises específicas.  

- **Pós-condição:** O administrador monitora a localização em tempo real de todas as bicicletas e toma ações com base nos dados apresentados.

---
### **7. Monitorar Localização e Dados da Bicicleta**
- **Ator principal:** Central do sistema  
- **Pré-condição:** A bicicleta deve estar em uso e conectada à rede.  
- **Fluxo principal:**
  1. A bicicleta transmite dados de GPS e acelerômetro para o servidor central.
  2. O sistema analisa os dados em tempo real para rastrear a localização e verificar padrões de uso.
  3. Caso detecte uma anomalia (como movimentação fora do perímetro), o sistema envia um alerta.  
- **Pós-condição:** A localização e o status da bicicleta estão atualizados.

---

### **8. Alerta de Perímetro Excedido**
- **Ator principal:** Sistema anti-furto  
- **Pré-condição:** A bicicleta está sendo monitorada em tempo real.  
- **Fluxo principal:**
  1. O sistema detecta que a bicicleta ultrapassou os limites definidos no mapa (dependências da UFRN).
  2. Uma notificação é enviada ao aplicativo do usuário e à central do sistema. 
- **Pós-condição:** A segurança da bicicleta é garantida, e o usuário é notificado.

---
