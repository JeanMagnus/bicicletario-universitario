# Bicicletário Universitário 🚴‍♂️

## Sumário de Desenvolvimento 📚

1. [Códigos desenvolvidos](content/detalhes-codigos.md)
2. [Requisitos funcionais e não funcionais](/content/requisitos.md)
3. [Casos de uso do sistema](/content/casos-uso.md)
4. [Máquinas de estado](/content/maquinas-de-estado.md)
5. [Diagrama de blocos](/content/diagrama-blocos.md)
6. [Diagrama Entidade-Relacionamento](/content/diagrama-er.md)
7. [Protótipo do aplicativo](/content/telas-app.md)
8. [Modelo do suporte](/content/modelo-suporte.md)
9. [Descrição do projeto](#descrição-do-projeto)
10. [Principais funcionalidades](#principais-funcionalidades)
11. [Arquitetura do sistema](#arquitetura-do-sistema)
12. [Impacto esperado](#impacto-esperado)
13. [Equipe](#equipe)

## **Documentos do projeto**
- [Apresentação de Sistemas Embarcados](/docs/Bicicletario-Universitario-Projeto-de-Sistemas-Embarcados.pdf)
- [Apresentação final do sistema (IoT)](https://www.canva.com/design/DAGdl63WHCU/qx2l47pAY6Rwcjr3kKUz1g/edit?utm_content=DAGdl63WHCU&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)
- [Artigo escrito](/docs/Bicicletrio_Universitrio__Uma_Soluo_Inovadora_para_os_Desafios_de_Mobilidade%20(1).pdf) [EM ANDAMENTO]

## **Descrição do Projeto**
O **Bicicletário Universitário** é uma solução inovadora para os desafios de mobilidade no campus da Universidade Federal do Rio Grande do Norte (UFRN). O projeto combina tecnologia, sustentabilidade e bem-estar, oferecendo uma alternativa eficiente e acessível de transporte para a comunidade acadêmica.

A iniciativa busca mitigar problemas como superlotação dos circulares, atrasos frequentes e emissões de CO₂, ao mesmo tempo que promove a saúde física e mental dos usuários. Alinhado aos Objetivos de Desenvolvimento Sustentável (ODS), o projeto integra bicicletas equipadas com sistemas embarcados, um suporte físico inteligente e um aplicativo para gerenciar todas as funcionalidades.




## **Principais Funcionalidades**
- **Monitoramento em Tempo Real**:  
  Cada bicicleta está equipada com GPS e acelerômetro, permitindo rastreamento contínuo de localização e dados de uso, além de garantir segurança com um sistema anti-furto.

- **Acesso Inteligente**:  
  Usuários podem desbloquear bicicletas diretamente pelo aplicativo, que se comunica com o suporte físico inteligente via internet.

- **Sustentabilidade**:  
  Os trajetos mais utilizados pelos usuários são mapeados para identificar áreas prioritárias para melhorias ambientais, como o plantio de árvores.

- **Inclusão e Acessibilidade**:  
  Bicicletas são disponibilizadas gratuitamente, promovendo igualdade de oportunidades e integração social.


## **Arquitetura do Sistema**
1. **Sistema Embarcado no Suporte**:
   - Travas eletrônicas acionadas via aplicativo.
   - Sensores ultrassônicos para detectar a presença da bicicleta.
   - Microcontrolador ESP32 para processamento local e comunicação via internet.

2. **Sistema Embarcado na Bicicleta**:
   - GPS para rastreamento.
   - Acelerômetro para monitoramento de movimento.
   - Comunicação integrada com o aplicativo.

3. **Aplicativo Mobile**:
   - Interface intuitiva para gerenciamento das bicicletas.
   - Informações em tempo real sobre disponibilidade e localização.
   - Configurações personalizáveis de notificações e rotas.


## **Impacto Esperado**
O **Bicicletário Universitário** visa transformar a mobilidade no campus, oferecendo:
- **Redução do Impacto Ambiental**: Menos emissões de CO₂ com a substituição de transporte motorizado.
- **Qualidade de Vida Melhorada**: Incentivo à atividade física e redução do estresse.
- **Sustentabilidade a Longo Prazo**: Planejamento de infraestrutura verde e expansão do modelo para outros locais.


## **Equipe**
Desenvolvido pelos estudantes da UFRN:
- Jean Magnus  
- Laura de Fátima  
- Larissa Souza  
- Jhonatan Batista  

---

**Junte-se a nós nessa pedalada rumo a um campus mais sustentável e conectado! 🚲🌿**
