//TRABALHO PRATICO 1 - ED3. INTEGRANTES:
//CARLOS HENRIQUE HANNAS DE CARVALHO, NUSP: 11965988
//MATHEUS BAPTISTELLA, NUSP: 11223117

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/arvore.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 1
#define ERROR 0

#define INSERTION_END -1

#define EMPTY 0

//Funcao de criacao da estrutura de arvore
arv_dec* cria_arvore(void){
  arv_dec* raiz = (arv_dec*) malloc(sizeof(arv_dec));
  if (raiz == NULL){
    printf("Erro na criacao da Arvore de Decisao.\n");
    exit(1);
  }
  *raiz = NULL;
  return raiz;
}

//Liberacao de cada nodo da arvore
void libera_nodo(nodo* no){
  if (no == NULL) return;
  libera_nodo(no->esq);
  libera_nodo(no->dir);
  free(no);
  no = NULL;
}

//Liberacao da raiz da arvore
void libera_arvore(arv_dec* raiz){
  if (raiz == NULL) return;
  libera_nodo(*raiz);
  free(raiz);
}

int arvore_vazia(arv_dec* raiz){
  if (raiz == NULL || *raiz == NULL) return TRUE;
  return FALSE;
}

//Contabiliza quantidade total de nodos da arvore
int numero_total_nodos(arv_dec* raiz){
  if (arvore_vazia(raiz)) return EMPTY;
  int num_total_esq = numero_total_nodos(&(*raiz)->esq);
  int num_total_dir = numero_total_nodos(&(*raiz)->dir);
  int num_total = num_total_esq + num_total_dir + 1;
  return num_total;
}

int numero_nodos_folhas(arv_dec* raiz){
  if (arvore_vazia(raiz)) return EMPTY;
  nodo* aux = *raiz;
  if (aux->esq == NULL && aux->dir == NULL) return 1;
  else return (numero_nodos_folhas(&(aux->esq)) + numero_nodos_folhas(&(aux->dir)));
}

//Insercao via nomeEstacao na arvore
//Contabiliza nroEstacoes 
int insere_arvore_MODO_1(arv_dec* raiz, char* nome_lido){
    
  if (raiz == NULL) return ERROR;
  struct NO* novo;
  novo = (struct NO*) malloc(sizeof(struct NO));
  if (novo == NULL){
    printf("Erro de alocacao.\n");
    exit(1);
  }
	strcpy(novo->nome_estacao, nome_lido);
	novo->codigo_estacao = 0;
	novo->codigo_prox_estacao = 0;
  novo->esq = NULL;
  novo->dir = NULL;

  //Insercao em arvore vazia
  if(*raiz == NULL){ 
    *raiz = novo;
  } 
  else {
    struct NO* atual = *raiz;
    struct NO* ant = NULL;
    while (atual != NULL){
      ant = atual;

      //Se o nome da estacao ja foi inserida, nao ha nova insercao
      if (strcmp(novo->nome_estacao, atual->nome_estacao) == 0){ 
        free(novo);
        return ERROR;
      } 
      else {
        if (strcmp(novo->nome_estacao, atual->nome_estacao) < 0) atual = atual->esq;
        else atual = atual->dir;
      }
    }
    if (strcmp(novo->nome_estacao, ant->nome_estacao) < 0) ant->esq = novo;
    else ant->dir = novo;
  }
  return SUCCESS;
}

//Insercao via codEstacao e codProxEstacao na arvore
//Contabiliza nroParesEstacoes 
int insere_arvore_MODO_2(arv_dec* raiz, int cod_est_lido, int cod_prox_est_lido){
    
    if (raiz == NULL || cod_prox_est_lido == -1) return ERROR;
    struct NO* novo;
    novo = (struct NO*) malloc(sizeof(struct NO));
    if (novo == NULL){
        printf("Erro de alocacao.\n");
        exit(1);
    }
		strcpy(novo->nome_estacao, "X");
		novo->codigo_estacao = cod_est_lido;
		novo->codigo_prox_estacao = cod_prox_est_lido;
    novo->esq = NULL;
    novo->dir = NULL;

    //Insercao em arvore vazia
    if(*raiz == NULL){ 
        *raiz = novo;
    } else{
        struct NO* atual = *raiz;
        struct NO* ant = NULL;
        
        //Enquanto o atual nao for folha
        while (atual != NULL){
            ant = atual;

            //Se ha repeticao de insercao de CodEstacao e CodProxEstacao, nao ha uma nova insercao
            if (novo->codigo_estacao == atual->codigo_estacao && novo->codigo_prox_estacao == atual->codigo_prox_estacao){ 
                free(novo);
                return ERROR;
            } else{
                if (novo->codigo_estacao < atual->codigo_estacao || ((novo->codigo_estacao == atual->codigo_estacao) && (novo->codigo_prox_estacao < atual->codigo_prox_estacao))) atual = atual->esq;
								else if ((novo->codigo_estacao > atual->codigo_estacao) || ((novo->codigo_estacao == atual->codigo_estacao) && (novo->codigo_prox_estacao > atual->codigo_prox_estacao))) atual = atual->dir;
            }
        }

				if (novo->codigo_estacao < ant->codigo_estacao || ((novo->codigo_estacao == ant->codigo_estacao) && (novo->codigo_prox_estacao < ant->codigo_prox_estacao))){
          ant->esq = novo;
        }
        else{
          ant->dir = novo;
        } 
    }
  
    return SUCCESS;
}
