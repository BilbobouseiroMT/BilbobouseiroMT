/* =========================================================
   MÓDULO: LOGS
   DESCRIÇÃO:
   Implementa o registro de eventos do sistema da autoescola.
   Os eventos são gravados em um arquivo de texto para fins
   de auditoria e acompanhamento das operações realizadas.

   Este arquivo contém:
   - Implementação da função registrarLog
   ========================================================= */

#include <stdio.h>
#include <time.h>
#include "logs.h"

/*
 Função: registrarLog
 --------------------
 Registra um evento de log no arquivo "logs.txt".

 Parâmetros:
   usuario -> nome do usuário que realizou a ação
   tipo    -> tipo de log (um dos valores de TipoLog)
   detalhe -> informações adicionais sobre a ação

 Comportamento:
   - Abre o arquivo "logs.txt" em modo append.
   - Captura a data e hora atuais.
   - Converte o tipo de log em texto legível.
   - Grava uma linha formatada com todas as informações.
   - Fecha o arquivo após a escrita.

 Observações:
   - Caso o arquivo não possa ser aberto, a função apenas
     imprime uma mensagem de erro no console.
*/
void registrarLog(const char *usuario, TipoLog tipo, const char *detalhe) {
    FILE *arquivo = fopen("logs.txt", "a");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de logs");
        return;
    }

    time_t agora = time(NULL);
    struct tm *infoTempo = localtime(&agora);

    const char *tipoTexto;
    switch (tipo) {
        case LOG_LOGIN_SUCESSO: tipoTexto = "LOGIN_SUCESSO"; break;
        case LOG_LOGIN_FALHA:   tipoTexto = "LOGIN_FALHA";   break;
        case LOG_INSERCAO:      tipoTexto = "INSERCAO";      break;
        case LOG_EDICAO:        tipoTexto = "EDICAO";        break;
        case LOG_EXCLUSAO:      tipoTexto = "EXCLUSAO";      break;
        case LOG_LISTAGEM:      tipoTexto = "LISTAGEM";      break;
        case LOG_BUSCA:         tipoTexto = "BUSCA";         break;
        case LOG_SAIDA:         tipoTexto = "SAIDA";         break;
        default:                tipoTexto = "DESCONHECIDO";  break;
    }

    fprintf(arquivo,
            "[%02d/%02d/%04d %02d:%02d:%02d] Usuario: %s | Acao: %s | Detalhe: %s\n",
            infoTempo->tm_mday,
            infoTempo->tm_mon + 1,
            infoTempo->tm_year + 1900,
            infoTempo->tm_hour,
            infoTempo->tm_min,
            infoTempo->tm_sec,
            usuario,
            tipoTexto,
            detalhe ? detalhe : "-");

    fclose(arquivo);
}
