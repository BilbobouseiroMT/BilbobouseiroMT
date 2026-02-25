# Projeto Final - Sistema Autoescola - Algoritmos e Estrutura de Dados II

## Introdução
O presente trabalho tem como objetivo desenvolver um sistema que integra conceitos fundamentais de **Algoritmos e Estrutura de Dados II**, aplicados em um contexto prático.  
A proposta consiste em estruturar o projeto de uma autoescola, a partir de quatro pilares principais: **autenticação, dados, logs e apresentação**.  

---

## Autenticação
A autenticação representa o mecanismo de segurança do sistema, garantindo que apenas usuários autorizados possam acessar suas funcionalidades.  
Foram implementados métodos de validação de credenciais e controle de acesso, assegurando confiabilidade e proteção das informações.  

---

## Dados
Os dados constituem o núcleo do projeto.  
A ênfase foi dada à organização, armazenamento e manipulação eficiente das informações, utilizando estruturas adequadas para otimizar consultas e operações.  
A integridade e consistência dos dados foram tratadas como requisitos essenciais.  

---

## Logs
Os logs desempenham papel fundamental no monitoramento e na rastreabilidade do sistema.  
Cada operação realizada é registrada, permitindo auditoria, análise de desempenho e identificação de falhas.  
Esse componente assegura transparência e suporte à manutenção futura.  

---

## Apresentação
A apresentação corresponde à interface de comunicação entre o sistema e seus usuários.  
Ela foi elaborada de forma clara e objetiva, visando transmitir os resultados obtidos e demonstrar a arquitetura desenvolvida.  
Além disso, a apresentação sintetiza os principais aprendizados adquiridos durante o processo de implementação.  

---

## Conclusão
O projeto final de **Algoritmos e Estrutura de Dados II** evidencia a aplicação prática dos conceitos estudados ao longo da disciplina.  
A integração entre autenticação, dados, logs e apresentação demonstra não apenas o domínio técnico, mas também a capacidade de organização, persistência e trabalho em equipe.  
Assim, o sistema desenvolvido cumpre seu papel acadêmico e serve como base para futuras evoluções.  
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Detalhe Técnico

---

### Estrutura Utilizada
O sistema foi implementado utilizando uma **Tabela Hash com Encadeamento Externo**.  
Para o gerenciamento dos dados em cada posição da tabela, foi empregada uma **Lista Encadeada Simples**, garantindo flexibilidade e eficiência no tratamento das colisões.  
É importante destacar que a estrutura utilizada não corresponde a filas ou pilhas, mas sim a listas lineares dinâmicas.

---

### Acesso Rápido
O acesso às informações é realizado por meio do **CPF**, que serve como chave para a função de hash.  
Esse mecanismo permite localizar registros de forma quase instantânea, sem a necessidade de percorrer todo o arquivo de dados.

---

### Tratamento de Colisões
Como diferentes CPFs podem gerar o mesmo índice na tabela, o sistema utiliza a **Lista Encadeada Simples** para armazenar múltiplos registros em uma mesma posição.  
Dessa forma, evita-se a sobrescrita de dados e garante-se a integridade das informações.

---

### Lógica de Re-hashing
Na operação de edição, caso o CPF seja alterado, o sistema realiza um **re-hashing**:  
- Remove o registro da posição antiga.  
- Insere o registro na nova posição correspondente ao novo CPF.  

Esse procedimento previne inconsistências e mantém a tabela organizada, evitando desperdício de memória.

---

### Persistência e Auditoria
O sistema está integrado com mecanismos de **persistência em arquivos CSV**, permitindo leitura e gravação dos dados de forma estruturada.  
Além disso, foram implementados **logs de auditoria**, que registram todas as operações realizadas (quem executou, o que foi feito e quando).  
Esse recurso assegura transparência e conformidade com os requisitos do projeto.

---

## Conclusão
A implementação demonstra a aplicação prática dos conceitos de **estruturas de dados** e **algoritmos de acesso eficiente**, conciliando desempenho, organização e confiabilidade.  
O uso da Tabela Hash com Encadeamento Externo, aliado à persistência e auditoria, evidencia a robustez da solução e sua adequação ao contexto acadêmico proposto.


