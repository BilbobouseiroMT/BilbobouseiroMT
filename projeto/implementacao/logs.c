/* =========================================================
   MÓDULO: LOGS
   DESCRIÇÃO:
   Implementa o registro de eventos do sistema da autoescola.
   ========================================================= */

#include <stdio.h>
#include <time.h>
#include "logs.h"

/* =========================================================
   IMPLEMENTAÇÃO DA INTERFACE PÚBLICA
   ========================================================= */

void registrarLog(const char *usuario, TipoLog tipo, const char *detalhe) {
    FILE *arquivo = fopen("data/logs.txt", "a");
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



