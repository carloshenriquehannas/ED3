#ifndef _defines_
#define _defines_

#define STRING_SIZE 75
#define SIZE_FIELD_NAME 16
#define MAX_FIELDS 8 

#define FIM_ARQUIVO -1
#define SUCESSO 0

typedef struct{
  char removido;
  int tamanhoRegistro;
  long proxLista;
  int CodEstacao;
  char NomeEstacao[STRING_SIZE];
  int CodLinha;
  char NomeLinha[STRING_SIZE];
  int CodProxEst;
  int DistanciaProxEst;
  int CodLinhaInteg;
  int CodEstacaoInteg;
} station_data;

typedef struct{
  char status;
  long topoLista;
  int nroEstacoes;
  int nroParesEstacao;
} header_bin;

#endif
