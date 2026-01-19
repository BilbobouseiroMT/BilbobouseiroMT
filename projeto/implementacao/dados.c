#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

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

int inserirAluno(const char *nome, const char *cpf, int idade) {
    unsigned int chave = hash(cpf);
    unsigned int idx = indiceTabela(chave);

    ITEM *p = tabela[idx].inicio;

    /* Verifica unicidade do CPF */
    while (p) {
        if (strcmp(p->cpf, cpf) == 0)
            return 0;
        p = p->prox;
    }

    ITEM *novo = (ITEM *) malloc(sizeof(ITEM));
    if (!novo)
        return 0;

    novo->r.chave = chave;
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->idade = idade;

    novo->prox = tabela[idx].inicio;
    tabela[idx].inicio = novo;
    tabela[idx].qtde++;

    return 1;
}

ITEM* buscarAluno(const char *cpf) {
    unsigned int chave = hash(cpf);
    unsigned int idx = indiceTabela(chave);

    ITEM *p = tabela[idx].inicio;

    while (p) {
        if (strcmp(p->cpf, cpf) == 0)
            return p;
        p = p->prox;
    }

    return NULL;
}

int removerAluno(const char *cpf) {
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
            return 1;
        }
        ant = p;
        p = p->prox;
    }

    return 0;
}

/* ======================= PERSISTÊNCIA EM ARQUIVO ================================= */

void carregarDados(void) {
    FILE *arq = fopen("data/alunos.csv", "r");
    if (!arq)
        return;

    char nome[50], cpf[12];
    int idade;

    while (fscanf(arq, "%49[^;];%11[^;];%d\n", nome, cpf, &idade) == 3) {
        inserirAluno(nome, cpf, idade);
    }

    fclose(arq);
}

void salvarDados(void) {
    FILE *arq = fopen("data/alunos.csv", "w");
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

    /* 1. Conta quantos alunos existem */
    for (int i = 0; i < TABLE_SIZE; i++) {
        total += tabela[i].qtde;
    }

    if (total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    /* 2. Cria vetor auxiliar de ponteiros */
    ITEM **vetor = malloc(total * sizeof(ITEM *));
    if (!vetor) {
        printf("Erro de memória.\n");
        return;
    }

    /* 3. Copia os ponteiros da hash para o vetor */
    int k = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ITEM *p = tabela[i].inicio;
        while (p) {
            vetor[k++] = p;
            p = p->prox;
        }
    }

    /* 4. Ordena o vetor */
    qsort(vetor, total, sizeof(ITEM *), compararPorNome);

    /* 5. Exibe os dados ordenados */
    printf("\n--- LISTA DE ALUNOS (ORDENADA POR NOME) ---\n");
    for (int i = 0; i < total; i++) {
        printf("Nome: %s | CPF: %s | Idade: %d\n",
               vetor[i]->nome,
               vetor[i]->cpf,
               vetor[i]->idade);
    }

    /* 6. Libera memória auxiliar */
    free(vetor);
}