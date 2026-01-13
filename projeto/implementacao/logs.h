#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdin.h>

 typedef struct cad_alu{
     char nome_p;
     char cpf;
     char data_nasc;
     char numero_tel;
 }cadastro;

typedef struct login{
     char email;
     char senha;
}login_alu;
 
 typedef struct aluno{
     cadastro_alu r;
     struct aluno* prox;
 }aluno_c;

 typedef struct login_alu{
     login r;
     struct aluno* prox;
 }aluno_l;
 
 typedef struct{
  ITEM *inicio;
  ITEM  *fim;
 }FILA; 
