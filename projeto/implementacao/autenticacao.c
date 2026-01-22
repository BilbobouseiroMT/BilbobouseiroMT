#include <stdio.h>      
#include <string.h>     
#include "autenticacao.h"

// Protótipo da função de log (implementada em logs.c ou aqui mesmo)
void registrar_log(const char *acao, const char *usuario);

/*
 Função: autenticar
 Objetivo:
   - Pedir login e senha ao usuário
   - Ler o arquivo dados/usuarios.csv
   - Verificar se existe uma linha com login;senha iguais
   - Repetir até acertar
   - Registrar sucesso ou falha em logs.csv

 Parâmetro:
   usuario_logado -> vetor onde será armazenado o login válido

 Retorno:
   1 -> autenticação realizada com sucesso
*/
int autenticar(char usuario_logado[]) {
    
    FILE *f;

    f = fopen("dados/usuarios.csv", "r");

    // Se não conseguiu abrir o arquivo, erro fatal
    if (f == NULL) {
        printf("Erro: nao foi possivel abrir dados/usuarios.csv\n");
        return 0;
    }

    // Vetores para armazenar dados lidos do arquivo
    char linha[100];      
    char login[50];      
    char senha[50];      

    // Vetores para armazenar o que o usuário digitar
    char login_digitado[50];
    char senha_digitada[50];

    int autenticado = 0; // flag: 0 = ainda não autenticou, 1 = autenticado

    // Loop principal: só sai quando autenticado == 1
    while (!autenticado) {

        printf("Login: ");
        scanf("%s", login_digitado);

        printf("Senha: ");
        scanf("%s", senha_digitada);

        rewind(f);

        autenticado = 0;

        // Lê o arquivo linha por linha
        while (fgets(linha, sizeof(linha), f)) {

            /* Quebra a linha no formato: login;senha
            Exemplo de linha: "admin;1234\n"
            Após isso:
            login = "admin"
            senha = "1234"
            */
            sscanf(linha, "%[^;];%s", login, senha);

            if (strcmp(login, login_digitado) == 0 &&
                strcmp(senha, senha_digitada) == 0) {

                // Encontrou usuário válido
                autenticado = 1;

                // Copia o login válido para o vetor de saída
                strcpy(usuario_logado, login_digitado);

                break;
            }
        }

        
        if (!autenticado) {
            printf("Login ou senha incorretos.\n");

            // Registra tentativa de login com erro no log
            registrar_log("login_falhou", login_digitado);
        }
    }

    // Se saiu do while, é porque autenticou com sucesso
    printf("Login realizado com sucesso.\n");

    // Registra login bem-sucedido no log
    registrar_log("login_sucesso", usuario_logado);

    // Fecha o arquivo de usuários
    fclose(f);

    return 1;
}
