#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "logs.h"

/* ============= ESTRUTURA INTERNA DO MÓDULO ================ */

static LISTA tabela[TABLE_SIZE];

/* ================= FUNÇÕES AUXILIARES ===================== */

/* Função hash para CPF */
static unsigned int hash(const char *s) {
    unsigned int h = 0;
    while (*s)
        h = h * 31 + (unsigned char)*s++;
    return h;
}

/* Calcula o índice da tabela */
static unsigned int indiceTabela(unsigned int chave) {
    return chave % TABLE_SIZE;
}

/* ============= IMPLEMENTAÇÃO DA INTERFACE PÚBLICA ================= */

void inicializarTabela(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        tabela[i].inicio = NULL;
        tabela[i].qtde = 0;
    }
}

int inserirAluno(const char *nome, const char *cpf, int idade, const char *usuario) {
    unsigned int chave = hash(cpf);
    unsigned int idx = indiceTabela(chave);

    ITEM *p = tabela[idx].inicio;

    if(idade < 18){
        printf("Menor de idade não pode se matricular!\n");
        return 0;
    }

    /*verifica a unicidade do cpf*/
    while (p) {
        if (strcmp(p->cpf, cpf) == 0) {
            registrarSaida("ERRO; CPF DUPLICADO", usuario);
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

    registrarSaida(linha, usuario);

    return 1;
}

/* Função para buscar o aluno */

ITEM* buscarAluno(const char *cpf, const char *usuario) {
    unsigned int chave = hash(cpf);
    unsigned int idx = indiceTabela(chave);
    char linha[150];

    ITEM *p = tabela[idx].inicio;

    while (p) {
        if (p->r.chave == chave && strcmp(p->cpf, cpf) == 0) {
            snprintf(linha, sizeof(linha),
                     "BUSCA; Nome: %s; CPF: %s; Idade: %d",
                     p->nome, p->cpf, p->idade);

            registrarSaida(linha, usuario);
            return p;
        }
        p = p->prox;
    }

    strcpy(linha, "Falha na busca (Item não encontrado)");

    registrarSaida(linha, usuario);
    return NULL;
}


int removerAluno(const char *cpf, const char *usuario) {
    unsigned int chave = hash(cpf);
    unsigned int idx = indiceTabela(chave);

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

            registrarSaida(linha, usuario);
            return 1;
        }

        ant = p;
        p = p->prox;
    }

    char linha[150];

    printf("Não foi possível concluir a operação! (Aluno não encontrado no banco de dados para exclusão)\n");

    strcpy(linha, "EXCLUSAO; NÃO ENCONTRADO");

    registrarSaida(linha, usuario);
    return 0;
}

/* ======================= PERSISTÊNCIA EM ARQUIVO ================================= */

void carregarDados(const char *usuario) {
    FILE *arq = fopen("../dados/base.csv", "r");
    if (!arq)
        return;

    char nome[50], cpf[12];
    int idade;

    while (fscanf(arq, "%49[^;];%11[^;];%d\n", nome, cpf, &idade) == 3) {
        inserirAluno(nome, cpf, idade, usuario);
    }

    fclose(arq);
}

void salvarDados(void) {
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
static int compararPorNome(const void *a, const void *b) {
    ITEM *x = *(ITEM **)a;
    ITEM *y = *(ITEM **)b;
    return strcmp(x->nome, y->nome);
}

/* Lista todos os alunos ordenados por nome */


void listarAlunosOrdenadosPorNome(void) {

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

    qsort(vetor, total, sizeof(ITEM *), compararPorNome);

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


int editarAluno(const char *cpf, const char *novoNome, int novaIdade, const char *usuario) {
    ITEM *p = buscarAluno(cpf, usuario);

    if (!p)
        return 0;

    strcpy(p->nome, novoNome);
    p->idade = novaIdade;

    char linha[200];

    snprintf(linha, sizeof(linha),
             "EDICAO;%s;%s;%d",
             p->nome, p->cpf, p->idade);

    salvarDados();

    registrarSaida(linha, usuario);

    return 1;
}