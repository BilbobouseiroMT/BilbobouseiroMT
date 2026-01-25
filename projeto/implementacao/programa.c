#include "autenticacao.h"
#include "dados.h"
#include "logs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Função Auxiliar para limpar o buffer ---
   Serve para consumir caracteres extras caso o usuário digite
   mais do que o permitido, evitando pular o próximo input */
void limpar_restos_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* --- Função Auxiliar para ler string e tratar o Enter --- */
void ler_texto(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        // Verifica se o \n foi lido
        char *quebra_linha = strchr(buffer, '\n');
        
        if (quebra_linha) {
            *quebra_linha = '\0'; // Remove o \n
        } else {
            // Se não achou \n, significa que o usuário digitou demais
            // Precisamos limpar o buffer do teclado
            limpar_restos_buffer();
        }
    }
}

int main(void){

    char usuario_logado[50];
    int escolha, loop = 1;
    char buffer[50];   // Aumentei um pouco para segurança
    
    // Aumentei o CPF para 16 para evitar o buffer overflow (11 digitos + enter + segurança)
    char cpf[16]; 
    char nome[50];

    if (autenticar(usuario_logado)) {

        inicializarTabela();
        carregarDados(usuario_logado);

        do {
            printf("\n--- MENU PRINCIPAL ---\n");
            printf("1. Cadastrar novo item\n");
            printf("2. Listar registros\n");
            printf("3. Pesquisar registro\n");
            printf("4. Editar registro\n");
            printf("5. Excluir registro\n");
            printf("6. Encerrar o programa\n");
            printf("Opcao: ");

            /* ===== Leitura segura do menu ===== */
            ler_texto(buffer, sizeof(buffer));
            
            if (sscanf(buffer, "%d", &escolha) != 1) {
                escolha = -1;
            }

            switch(escolha){

            /* ================================================= */
            case 1: {
                int idade;

                printf("Nome: ");
                ler_texto(nome, sizeof(nome));

                printf("CPF: ");
                ler_texto(cpf, sizeof(cpf));

                printf("Idade: ");
                ler_texto(buffer, sizeof(buffer));

                if (sscanf(buffer, "%d", &idade) != 1) {
                    printf("Idade invalida.\n");
                    break;
                }

                if (inserirAluno(nome, cpf, idade, usuario_logado)) {
                    salvarDados();
                    registrarLog(usuario_logado, LOG_INSERCAO, nome);
                }
                break;
            }

            /* ================================================= */
            case 2:
                listarAlunosOrdenadosPorNome();
                registrarLog(usuario_logado, LOG_LISTAGEM, "Exibicao da lista de alunos");
                break;

            /* ================================================= */
            case 3: {
                char linha[150];

                printf("CPF para busca: ");
                ler_texto(cpf, sizeof(cpf));

                ITEM *p = buscarAluno(cpf, usuario_logado);

                if (p) {
                    printf("\n----------------------------\n");
                    printf("CPF: %s\nNOME: %s\nIDADE: %d", p->cpf, p->nome, p->idade);
                    printf("\n----------------------------\n");
                } else {
                    printf("\nAluno nao encontrado.\n");
                }

                snprintf(linha, sizeof(linha), "BUSCA; Parametro: %s", cpf);
                
                // Nota: registrarSaida parece duplicado com log, mas mantive seu original
                registrarSaida(linha, usuario_logado); 
                registrarLog(usuario_logado, LOG_BUSCA, linha);
                break;
            }

            /* ================================================= */
            case 4: {
                char novo_nome[50], linha[150];
                int nova_idade;

                printf("CPF do aluno que deseja alterar: ");
                ler_texto(cpf, sizeof(cpf));

                ITEM *i = buscarAluno(cpf, usuario_logado);

                if(!i){
                    printf("Usuário não existente na base de dados!\n");
                    break;
                }

                printf("Novo nome: ");
                ler_texto(novo_nome, sizeof(novo_nome));

                printf("Nova idade: ");
                ler_texto(buffer, sizeof(buffer));
                
                if (sscanf(buffer, "%d", &nova_idade) == 1) {
                    editarAluno(cpf, novo_nome, nova_idade, usuario_logado);
                    
                    snprintf(linha, sizeof(linha), "EDICAO; CPF do registro: %s", cpf);
                    registrarLog(usuario_logado, LOG_EDICAO, linha);
                } else {
                    printf("Idade invalida. Edicao cancelada.\n");
                }
                break;
            }

            /* ================================================= */
            case 5: {
                char linha[150];

                printf("CPF para excluir: ");
                ler_texto(cpf, sizeof(cpf));

                if (removerAluno(cpf, usuario_logado)) {
                    salvarDados();

                    snprintf(linha, sizeof(linha), "EXCLUSAO; CPF do registro: %s", cpf);
                    registrarLog(usuario_logado, LOG_EXCLUSAO, linha);
                }
                break;
            }

            /* ================================================= */
            case 6:
                printf("\nAte a proxima!\n");
                registrarSaida("Encerramento da aplicacao", usuario_logado);
                registrarLog(usuario_logado, LOG_SAIDA, "Usuario saiu da aplicacao");
                loop = 0;
                break;

            default:
                printf("Opcao invalida.\n");
            }

        } while(loop);
    }

    return 0;
}