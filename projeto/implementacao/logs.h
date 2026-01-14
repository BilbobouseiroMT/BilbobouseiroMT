#ifndef LOGS_H
#define LOGS_H

/* =========================================================
   MÓDULO: LOGS
   DESCRIÇÃO:
   Responsável pelo registro das operações realizadas
   no sistema da autoescola, incluindo login, inserção,
   edição, exclusão, listagem, busca e saída.

   Este arquivo define APENAS:
   - Tipos de dados (enumeração de eventos)
   - Protótipos de funções de log

   Nenhuma lógica de gravação em arquivo é feita aqui.
   ========================================================= */

/* ===== TIPOS DE LOGS DISPONÍVEIS ===== */

typedef enum {
    LOG_LOGIN_SUCESSO,  /* Login realizado com sucesso */
    LOG_LOGIN_FALHA,     /* Tentativa de login sem sucesso */
    LOG_INSERCAO,        /* Inserção de dados */
    LOG_EDICAO,          /* Edição de dados */
    LOG_EXCLUSAO,        /* Exclusão de dados */
    LOG_LISTAGEM,        /* Listagem de registros */
    LOG_BUSCA,           /* Busca de informações */
    LOG_SAIDA            /* Saída do sistema */
} TipoLog;

/* ======= INTERFACE PÚBLICA ========== */

/*
 Registra um evento de log no sistema.
 Parâmetros:
   usuario -> nome do usuário que realizou a ação
   tipo    -> tipo de log (um dos valores de TipoLog)
   detalhe -> informações adicionais sobre a ação
*/
void registrarLog(const char *usuario, TipoLog tipo, const char *detalhe);

#endif /* LOGS_H */


