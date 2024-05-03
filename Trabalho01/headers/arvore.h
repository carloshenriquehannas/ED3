#ifndef _arvore_
#define _arvore_

#define TRUE 1
#define FALSE 0

#define SUCCESS 1
#define ERROR 0

#define INSERTION_END -1

#define EMPTY 0

#include "../headers/defines.h"

typedef struct NO nodo;
typedef struct NO* arv_dec;

struct NO{
  struct NO* esq;
  struct NO* dir;
  char nome_estacao[50];
  int codigo_estacao;
  int codigo_prox_estacao;

};

arv_dec* cria_arvore(void);
void libera_nodo(nodo* no);
void libera_arvore(arv_dec* raiz);

int arvore_vazia(arv_dec* raiz);
int numero_total_nodos(arv_dec* raiz);
int numero_nodos_folhas(arv_dec* raiz);

//MODO 1: conta o numero de estacoes diferentes ha no arquivo binario
//MODO 2: conta o numero de pares distintos codEstacao e codProximaEst
int insere_arvore_MODO_1(arv_dec* raiz, char* nome_lido);
int insere_arvore_MODO_2(arv_dec* raiz, int cod_est_lido, int cod_prox_est_lido);

#endif
