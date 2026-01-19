#ifndef DADOS_H
#define DADOS_H

/* =========================================================
   MÓDULO: DADOS
   DESCRIÇÃO:
   Responsável pelo gerenciamento dos dados da autoescola,
   incluindo cadastro, busca, listagem e remoção de alunos.

   Este arquivo define APENAS:
   - Tipos de dados
   - Constantes
   - Protótipos de funções

   Nenhuma lógica ou acesso a arquivos é feito aqui.
   ========================================================= */

/* ===== CONSTANTES DO MÓDULO ====== */

#define TABLE_SIZE 1024      /* Tamanho da tabela hash */

/* ====== ESTRUTURAS DE DADOS =========== */

/* Registro base utilizado na indexação */
typedef struct {
    unsigned int chave;      /* Chave hash (derivada do CPF) */
} REGISTRO;

/* Estrutura que representa um aluno da autoescola */
typedef struct item {
    REGISTRO r;              /* Registro de controle */
    char nome[50];           /* Nome completo do aluno */
    char cpf[12];            /* CPF (11 dígitos + '\0') */
    int idade;               /* Idade do aluno */
    struct item *prox;       /* Próximo item (encadeamento) */
} ITEM;

/* Estrutura de lista encadeada usada nas colisões da hash */
typedef struct {
    ITEM *inicio;            /* Ponteiro para o primeiro item */
    int qtde;                /* Quantidade de itens na lista */
} LISTA;

/* ======= INTERFACE PÚBLICA ========== */

/*
 Inicializa a estrutura de dados.
 Deve ser chamada uma única vez no início do programa.
*/
void inicializarTabela(void);

/*
 Insere um novo aluno no sistema.
 Retorna:
  1 -> inserção realizada com sucesso
  0 -> CPF já existente ou erro de memória
*/
int inserirAluno(const char *nome, const char *cpf, int idade);

/*
 Busca um aluno pelo CPF.
 Retorna:
  Ponteiro para o ITEM encontrado
  NULL caso não exista
*/
ITEM* buscarAluno(const char *cpf);

/*
 Remove um aluno pelo CPF.
 Retorna:
  1 -> remoção realizada com sucesso
  0 -> aluno não encontrado
*/
int removerAluno(const char *cpf);

/*
 Carrega os dados persistidos do arquivo para a memória.
 Deve ser chamada no início do programa após a inicialização.
*/
void carregarDados(void);

/*
 Salva os dados da memória para o arquivo.
 Deve ser chamada antes do encerramento do programa.
*/
void salvarDados(void);

/*
Exibe o nome dos aluno ordenados por nome.
*/

void listarAlunosOrdenadosPorNome(void);

#endif /* DADOS_H */
