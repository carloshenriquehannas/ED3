//TRABALHO PRATICO 1 - ED3. INTEGRANTES:
//CARLOS HENRIQUE HANNAS DE CARVALHO, NUSP: 11965988
//MATHEUS BAPTISTELLA, NUSP: 11223117

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/functions.h"
#include "../headers/defines.h"

//Leitura dos caracteres do arquivo .csv
void read_csv(FILE *fp_csv, char *str){
  int i = 0;
  char c;

  c = fgetc(fp_csv);
  if (c == ',' || c == '\n'){
    str[0] = '|';
    str[1] = '\0';
    return;
  }

    //Le a string contida no arquivo, ate encontar um delimitador: final da linha ou final do arquivo
  while (c != ',' && c != '\n' && c != feof(fp_csv)){
    if (feof(fp_csv)) return;
    str[i] = c;
    c = fgetc(fp_csv);
    i++;
  }
  str[i] = '\0';

  return;
}

void print_register(station_data *s){

  //Print do campo CodEstacao
  if(s->CodEstacao == -1){
    printf("NULO ");
  } else{
    printf("%d ", s->CodEstacao);
  }

  //Print caracter por caracter de s->NomeEstacao
  if(strlen(s->NomeEstacao) == 0){
    printf("NULO ");
  }else{
    printf("%s ", s->NomeEstacao);
  }

  //Print do campo CodLinha
  if(s->CodLinha == -1){
    printf("NULO ");
  } else{
    printf("%d ", s->CodLinha);
  }

 //Print caracter por caracter de s->NomeLinha
  if(strlen(s->NomeLinha) == 0){
  printf("NULO ");
  } else{
    printf("%s ", s->NomeLinha);
  }

 //Print do campo CodProxEst
 if(s->CodProxEst == -1){
    printf("NULO ");
  } else{
    printf("%d ", s->CodProxEst);
  }

  //Print do campo DistanciaProxEst
  if(s->DistanciaProxEst == -1){
    printf("NULO ");
  } else{
    printf("%d ", s->DistanciaProxEst);
  }

  //Print do campo CodLinhaInteg
  if(s->CodLinhaInteg == -1){
    printf("NULO ");
  } else{
    printf("%d ", s->CodLinhaInteg);
  }
  
  //Print do campo CodEstacaoInteg
  if(s->CodEstacaoInteg == -1){
    printf("NULO\n");
  } else{
    printf("%d\n", s->CodEstacaoInteg);
  }
}

int contains_string_criteria(char *field_name, char *field_string_value, station_data *s){

  if(strcmp(field_name, "nomeEstacao") == 0){
    if(strcmp(field_string_value, s->NomeEstacao) == 0){
      return 1;
    }
  }
  else if(strcmp(field_name, "nomeLinha") == 0){
    if(strcmp(field_string_value, s->NomeLinha) == 0){
      return 1;
    }
  }

  return 0;
}

int contains_int_criteria(char *field_name, int field_int_value, station_data *s){

  if(strcmp(field_name, "codEstacao") == 0){
    if(field_int_value == s->CodEstacao){
      return 1;
    }
  }
  else if(strcmp(field_name, "codLinha") == 0){
    if(field_int_value == s->CodLinha){
      return 1;
    }
  }
  else if(strcmp(field_name, "codProxEstacao") == 0){
    if(field_int_value == s->CodProxEst){
      return 1;
    }
  }
  else if(strcmp(field_name, "distProxEstacao") == 0){
    if(field_int_value == s->DistanciaProxEst){
      return 1;
    }
  }
  else if(strcmp(field_name, "codLinhaIntegra") == 0){
    if(field_int_value == s->CodLinhaInteg){
      return 1;
    }
  }
  else if(strcmp(field_name, "codEstIntegra") == 0){
    if(field_int_value == s->CodEstacaoInteg){
      return 1;
    }
  }
  
  return 0;
}

