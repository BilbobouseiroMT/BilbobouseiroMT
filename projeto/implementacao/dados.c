#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "logs.h"

/* ============= ESTRUTURA INTERNA DO MÓDULO ================ */

static LISTA tabela[TABLE_SIZE];

/* ================= FUNÇÕES AUXILIARES ===================== */

/* Função hash para CPF */
static unsigned int gerar_hash(const char *s) {
    unsigned int h = 0;
    while (*s)
        h = h * 31 + (unsigned char)*s++;
    return h;
}

/* Calcula o índice da tabela */
static unsigned int calcular_indice(unsigned int chave) {
    return chave % TABLE_SIZE;
}

/* ============= IMPLEMENTAÇÃO DA INTERFACE PÚBLICA ================= */

void inicializar_tabela(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        tabela[i].inicio = NULL;
        tabela[i].qtde = 0;
    }
}

int inserir_aluno(const char *nome, const char *cpf, int idade, const char *usuario) {
    unsigned int chave = gerar_hash(cpf);
    unsigned int idx = calcular_indice(chave);

    ITEM *p = tabela[idx].inicio;

    if(idade < 18){
        printf("Menor de idade não pode se matricular!\n");
        return 0;
    }

    /*verifica a unicidade do cpf*/
    while (p) {
        if (strcmp(p->cpf, cpf) == 0) {
            registrar_saida("ERRO; CPF DUPLICADO", usuario);
            printf("Não foi possível concluir a operação! (CPF já existente no banco de dados)\n");
            return 0;
        }
        p = p->prox;
    }

    ITEM *novo = malloc(sizeof(ITEM));
    if (!novo)
        return 0;

    novo->r.chave = chave;
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->idade = idade;

    novo->prox = tabela[idx].inicio;
    tabela[idx].inicio = novo;
    tabela[idx].qtde++;

    char linha[150];
    sprintf(linha, "INSERCAO;%s;%s;%d", nome, cpf, idade);

    return 1;
}

/* Função para buscar o aluno */

ITEM* buscar_aluno(const char *cpf, const char *usuario) {
    unsigned int chave = gerar_hash(cpf);
    unsigned int idx = calcular_indice(chave);
    char linha[150];

    ITEM *p = tabela[idx].inicio;

    while (p) {
        if (p->r.chave == chave && strcmp(p->cpf, cpf) == 0) {
            snprintf(linha, sizeof(linha),
                     "BUSCA; Nome: %s; CPF: %s; Idade: %d",
                     p->nome, p->cpf, p->idade);

            return p;
        }
        p = p->prox;
    }

    return NULL;
}


int remover_aluno(const char *cpf, const char *usuario) {
    unsigned int chave = gerar_hash(cpf);
    unsigned int idx = calcular_indice(chave);

    ITEM *p = tabela[idx].inicio;
    ITEM *ant = NULL;

    while (p) {
        if (strcmp(p->cpf, cpf) == 0) {

            if (ant)
                ant->prox = p->prox;
            else
                tabela[idx].inicio = p->prox;

            free(p);
            tabela[idx].qtde--;

            char linha[150];

            printf("\nOperação conlcuída com sucesso!\n");

            strcpy(linha, "EXCLUSÃO; SUCESSO");

            registrar_saida(linha, usuario);
            return 1;
        }

        ant = p;
        p = p->prox;
    }

    char linha[150];

    printf("Não foi possível concluir a operação! (Aluno não encontrado no banco de dados para exclusão)\n");

    strcpy(linha, "EXCLUSAO; NÃO ENCONTRADO");

    registrar_saida(linha, usuario);
    return 0;
}

/* ======================= PERSISTÊNCIA EM ARQUIVO ================================= */

void carregar_dados(const char *usuario) {
    FILE *arq = fopen("../dados/base.csv", "r");
    if (!arq)
        return;

    char nome[50], cpf[12];
    int idade;

    while (fscanf(arq, "%49[^;];%11[^;];%d\n", nome, cpf, &idade) == 3) {
        inserir_aluno(nome, cpf, idade, usuario);
    }

    fclose(arq);
}

void salvar_dados(void) {
    FILE *arq = fopen("../dados/base.csv", "w");
    if (!arq)
        return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        ITEM *p = tabela[i].inicio;
        while (p) {
            fprintf(arq, "%s;%s;%d\n", p->nome, p->cpf, p->idade);
            p = p->prox;
        }
    }

    fclose(arq);
}

/* =========================================================
   ORDENAÇÃO E LISTAGEM
   ========================================================= */

/* Função auxiliar de comparação */
static int comparar_por_nome(const void *a, const void *b) {
    ITEM *x = *(ITEM **)a;
    ITEM *y = *(ITEM **)b;
    return strcmp(x->nome, y->nome);
}

/* Lista todos os alunos ordenados por nome */


void listar_alunos_ordenados_por_nome(void) {

    int total = 0;

    for (int i = 0; i < TABLE_SIZE; i++)
        total += tabela[i].qtde;

    if (total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    ITEM **vetor = malloc(total * sizeof(ITEM *));
    if (!vetor) return;

    int k = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        ITEM *p = tabela[i].inicio;
        while (p) {
            vetor[k++] = p;
            p = p->prox;
        }
    }

    qsort(vetor, total, sizeof(ITEM *), comparar_por_nome);

    FILE *saida = fopen("../saida/saida.csv", "w");

    for (int i = 0; i < total; i++) {

        printf("%d - %s | %s | %d \n",
               i+1,
               vetor[i]->nome,
               vetor[i]->cpf,
               vetor[i]->idade);

        if (saida)
            fprintf(saida, "%d;%s;%s;%d\n",
                    i+1,
                    vetor[i]->nome,
                    vetor[i]->cpf,
                    vetor[i]->idade);
    }

    if (saida) fclose(saida);
    free(vetor);
}


/* * Funcao: editar_aluno
 * --------------------
 * Atualiza os dados de um aluno. Se o CPF for alterado, a funcao realiza
 * o re-hashing do registro para manter a integridade da tabela hash.
 *
 * cpf_antigo: CPF atual do aluno para busca
 * novo_nome: Novo nome a ser atribuído
 * nova_idade: Nova idade a ser atribuída
 * novo_cpf: Novo CPF (pode ser igual ao antigo se nao desejar alterar)
 * usuario: Usuario que realiza a operacao (para auditoria)
 *
 * retorna: 1 em caso de sucesso, 0 se o aluno nao existir ou o novo CPF for duplicado.
 */
int editar_aluno(const char *cpf_antigo, const char *novo_nome, int nova_idade, const char *novo_cpf, const char *usuario) {
    
    // 1. Busca o aluno original
    ITEM *aluno_original = buscar_aluno(cpf_antigo, usuario);
    if (!aluno_original) {
        printf("Erro: Aluno com CPF %s não encontrado.\n", cpf_antigo);
        return 0;
    }

    // 2. Caso o CPF esteja sendo alterado, precisamos validar a unicidade do novo
    if (strcmp(cpf_antigo, novo_cpf) != 0) {
        // Verifica se o novo_cpf ja pertence a outro aluno
        if (buscar_aluno(novo_cpf, usuario) != NULL) {
            printf("Erro: O novo CPF %s ja esta em uso.\n", novo_cpf);
            registrar_saida("EDICAO; FALHA; CPF DUPLICADO", usuario);
            return 0;
        }

        /* * Re-hashing
         * Se o CPF mudou, o indice na tabela provavelmente mudara. 
         * A forma mais segura e remover o antigo e inserir o novo.
         */
        
        // Remove o registro antigo (isso trata ponteiros e tabela[idx].qtde)
        remover_aluno(cpf_antigo, usuario);

        // Insere como um novo registro (isso gera novo hash e novo log de insercao)
        if (inserir_aluno(novo_nome, novo_cpf, nova_idade, usuario)) {
            registrar_saida("EDICAO; SUCESSO; CPF ALTERADO", usuario);
            return 1;
        }
        return 0;
    }

    // 3. Caso o CPF seja o mesmo, apenas atualizamos os outros campos in-place
    strcpy(aluno_original->nome, novo_nome);
    aluno_original->idade = nova_idade;

    // Atualiza o arquivo fisico
    salvar_dados();

    char mensagem_log[200];
    snprintf(mensagem_log, sizeof(mensagem_log), "EDICAO; NOME:%s; IDADE:%d", novo_nome, nova_idade);
    registrar_saida(mensagem_log, usuario);

    printf("Dados atualizados com sucesso!\n");
    return 1;
}