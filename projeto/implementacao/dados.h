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
void inicializar_tabela(void);

/*
 Insere um novo aluno no sistema.
 Retorna:
  1 -> inserção realizada com sucesso
  0 -> CPF já existente ou erro de memória
*/
int inserir_aluno(const char *nome, const char *cpf, int idade, const char *usuario);

/*
 Busca um aluno pelo CPF.
 Retorna:
  Ponteiro para o ITEM encontrado
  NULL caso não exista
*/
ITEM* buscar_aluno(const char *cpf, const char *usuario);

/*
 Remove um aluno pelo CPF.
 Retorna:
  1 -> remoção realizada com sucesso
  0 -> aluno não encontrado
*/
int remover_aluno(const char *cpf, const char *usuario);

/*
 Carrega os dados persistidos do arquivo para a memória.
 Deve ser chamada no início do programa após a inicialização.
*/
void carregar_dados(const char *usuario);

/*
 Salva os dados da memória para o arquivo.
 Deve ser chamada antes do encerramento do programa.
*/
void salvar_dados(void);

/*
Exibe o nome dos aluno ordenados por nome.
*/

void listar_alunos_ordenados_por_nome(void);

editar_aluno(const char *cpf_antigo, const char *novo_nome, int nova_idade, const char *novo_cpf, const char *usuario);


#endif 
/* DADOS_H */
