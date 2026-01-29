#ifndef DADOS_H
#define DADOS_H

/* ============================================================================
 * MÓDULO: DADOS
 * DESCRIÇÃO: Responsável pelo gerenciamento de alunos (entidade principal).
 * ESTRUTURA: Utiliza uma Tabela Hash com Encadeamento Externo para garantir
 * buscas e inserções eficientes.
 * REQUISITOS: Atende RF0003 até RF0007 e RNF0003 do enunciado.
 * ============================================================================
 */

/* ===== CONSTANTES DO MÓDULO ====== */

/**
 * TABLE_SIZE: Define a capacidade da tabela hash.
 * Por que 1024? É uma potência de 2, o que otimiza a operação de módulo (%)
 * e oferece um bom equilíbrio entre uso de memória e redução de colisões.
 */
#define TABLE_SIZE 1024

/* ====== ESTRUTURAS DE DADOS =========== */

/**
 * REGISTRO: Encapsula a chave de indexação.
 * Por que? Facilita a manutenção caso a lógica de busca mude de CPF (string)
 * para um ID numérico no futuro, sem alterar a estrutura do ITEM.
 */
typedef struct {
    unsigned int chave; /* Valor resultante da função de espalhamento (hash) */
} REGISTRO;

/**
 * ITEM: Representa o nó da lista encadeada (aluno).
 * Por que? Permite o "Tratamento de Colisões por Encadeamento". Se dois CPFs
 * gerarem o mesmo índice, eles serão armazenados nesta lista.
 */
typedef struct item {
    REGISTRO r;           /* Dados de controle para busca rápida */
    char nome[50];        /* Nome do aluno (RNF0002 - texto) */
    char cpf[12];         /* Chave única (11 dígitos + terminador) */
    int idade;            /* Validado para garantir maioridade no cadastro */
    struct item *prox;    /* Ponteiro para tratar colisões no mesmo índice */
} ITEM;

/**
 * LISTA: Representa um "balde" (bucket) da tabela hash.
 * Como? Armazena o início da lista e um contador para monitorar a densidade
 * de ocupação da tabela, auxiliando na depuração.
 */
typedef struct {
    ITEM *inicio;         /* Cabeça da lista encadeada no índice */
    int qtde;             /* Total de alunos colididos neste índice */
} LISTA;

/* ======= INTERFACE PÚBLICA (PROTÓTIPOS) ========== */

/**
 * Funcao: inicializar_tabela
 * Por que: O C não limpa a memória automaticamente. É vital garantir que todos
 * os ponteiros da tabela comecem como NULL.
 * Como: Percorre o array 'tabela' definindo inicio = NULL e qtde = 0.
 */
void inicializar_tabela(void);

/**
 * Funcao: inserir_aluno
 * Por que: Adiciona novos registros garantindo a regra de unicidade do CPF.
 * Como: Calcula o hash do CPF, verifica duplicatas na lista encadeada do índice
 * e insere o novo nó no início da lista.
 */
int inserir_aluno(const char *nome, const char *cpf, int idade, const char *usuario);

/**
 * Funcao: buscar_aluno
 * Por que: Necessário para as operações de consulta, edição e exclusão.
 * Como: Localiza o índice via hash e realiza uma busca linear apenas na pequena
 * lista de colisões daquele índice.
 */
ITEM* buscar_aluno(const char *cpf, const char *usuario);

/**
 * Funcao: remover_aluno
 * Por que: Atende ao requisito RF0007 (Excluir registro).
 * Como: Localiza o item, ajusta os ponteiros 'prox' do elemento anterior para
 * "pular" o excluído e libera a memória com free().
 */
int remover_aluno(const char *cpf, const char *usuario);

/**
 * Funcao: carregar_dados
 * Por que: Garante a persistência exigida no RNF0002.
 * Como: Abre o arquivo base.csv em modo leitura e reconstrói a tabela hash na RAM.
 */
void carregar_dados(const char *usuario);

/**
 * Funcao: salvar_dados
 * Por que: Salva o estado atual da memória antes de fechar o programa.
 * Como: Percorre toda a tabela hash e escreve os dados no formato CSV.
 */
void salvar_dados(void);

/**
 * Funcao: listar_alunos_ordenados_por_nome
 * Por que: Atende ao requisito RF0004 de listagem.
 * Como: Copia os ponteiros para um vetor auxiliar, ordena usando qsort() e
 * exibe os dados com um contador, conforme o caso de teste CT0005.
 */
void listar_alunos_ordenados_por_nome(void);

/**
 * Funcao: editar_aluno
 * Por que: Atende ao requisito RF0006 (Editar registro).
 * Como: Se o CPF mudar, realiza o re-hashing (remove e reinsere o nó) para
 * não corromper o índice da tabela. Caso contrário, atualiza in-place.
 */
int editar_aluno(const char *cpf_antigo, const char *novo_nome, int nova_idade, 
                 const char *novo_cpf, const char *usuario);

#endif /* DADOS_H */