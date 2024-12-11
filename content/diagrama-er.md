## **Diagrama Entidade Relacionamento** 

Essa sessão tem como objetivo descrever o diagrama ER que será usado para gerenciar o uso do **Bicicletário Universitário**, que será implantando na Universidade Federal do Rio Grande do Norte.

Para armanezar os dados utilizados no aplicativo será necessário fazer o armazenamento em banco de dados, por isso foi contruído o banco de ados da aplicaçaõ.

### **1. Definindo Entidades e Atributos**
- **Aluno:** 
  1. matricula (chave primária)
  2. nome (texto)
  3. email (texto)
  4. vinculo (texto, ex: graduação, pós-graduação, professor)
- **Bicicleta:** 
  1. id_bicicleta (chave primária)
  2. status (texto, ex: disponível, em uso, em manutenção)
  3. localizacao_atual (coordenadas GPS)
  4. qr_code (texto ou imagem, para identificar a bicicleta)
- **Aluguel (Relacionamento entre Aluno e Bicicleta:** 
  1. id_aluguel (chave primária)
  2. matricula_aluno (chave estrangeira para Aluno.matricula)
  3. id_bicicleta (chave estrangeira para Bicicleta.id_bicicleta)
  4. data_hora_inicio (data/hora)
  5. data_hora_fim (data/hora)
  6. status (texto, ex: ativo, concluído)
 
 ---
 
### **2. Definindo Relacionamento**
- **Aluno <-> Aluguel**
  1. Um **aluno** pode realizar vários alugueis ao longo do tempo.
  2. Um **aluguel** pertence a um único aluno.  (Relacionamento 1:N)
- **Bicicleta <-> Aluguel**
  1. Uma bicicleta pode ser usada em vários alugueis ao longo do tempo.
  2. Um aluguel refere-se a uma única bicicleta.(Relacionamento 1:N).
 
  ---

 
Após fazer estas definições, foi realizada a contrução do diagrama Entidade-Relacionamento de fato.

Segue abaixo diagrama mencionado:

![Diagrama Entidade-Relacionamento](/images/diagrama-entidade-relacionamento.png)

Após a montagem do diagrama, é possível pensar em **Possíveis Funcionalidades e Telas no Banco de Dados** como: Login, Localização da Bicicleta, Desbloqueio via QR Code e Perfil do Aluno. Com isso podemos realizar os próximos passo para ver se o modelo ER atende todas as funções que devem ser implementadas no sistema, afim de verificar se o mesmo atende todos os requisitos definidos.
