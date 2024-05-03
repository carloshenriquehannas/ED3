//TRABALHO PRATICO 1 - ED3. INTEGRANTES:
//CARLOS HENRIQUE HANNAS DE CARVALHO, NUSP: 11965988
//MATHEUS BAPTISTELLA, NUSP: 11223117

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/defines.h"
#include "./headers/funcoesFornecidas.h"
#include "./headers/IO_bin.h"
#include "./headers/arvore.h"

int main(){
  int command;
  int arg_num;
  int num_crit;
  int file_end;
  int size_reg;
  int counter;
  int num_fields;

  long *offset_array;
  char *file_csv, *file_bin;
  char *field_name_array[MAX_FIELDS];
  char *field_value_array[MAX_FIELDS];
  
  FILE *fp_csv, *fp_bin;
  station_data *s = NULL;
  header_bin *h = NULL;

  arv_dec *raiz_MODO_1;
  arv_dec *raiz_MODO_2;

  s = malloc(sizeof(station_data));
  h = malloc(sizeof(header_bin));

  if (s == NULL || h == NULL) return 1;

  //Le qual comando vai executar
  scanf("%d", &command);
  if(command == 1){
    scanf(" %ms", &file_csv);
  }
  //Le o nome do arquivo
  scanf(" %ms", &file_bin);

  //Menu das 6 opcoes para o executar o programa
  switch(command){

    //Insercao de dados do csv para binario
    case 1:

      fp_csv = fopen(file_csv, "r");
      fp_bin = fopen(file_bin, "wb");

      if(fp_csv == NULL || fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }

      //Pula o cabecalho do arquivo .csv
      char c = fgetc(fp_csv);
      while (c != '\n'){
        c = fgetc(fp_csv);
      }

      //Funcao auxiliar para escrever no binario
      write_bin(fp_csv, fp_bin, s, h);

      fclose(fp_csv);
      fclose(fp_bin);

      binarioNaTela(file_bin);

      break;

    //Impressao de todos os registros do arquivo binario
    case 2:

      fp_bin = fopen(file_bin, "rb");

      if(fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }

      //Funcao auxiliar para ler o arquivo binario
      read_bin(fp_bin, s);

      fclose(fp_bin);

      break;

    //Impressao dos registros que atendem a determinados criterios de busca
    case 3:

      //Recebe do input do usuario a quantidade de campos a serem usados como criterio de busca
      scanf(" %d", &arg_num);

      fp_bin = fopen(file_bin, "rb");
      if(fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }

      //Funcao auxiliar que busca byteoffset do registro buscado
      offset_array = search_criteria(fp_bin, s, arg_num, &counter);

      //Funcao auxiliar que printa os registros que tem seus byteoffsets em offset_array
      print_criteria(fp_bin, offset_array, s, &counter);

      free(offset_array);

      fclose(fp_bin);

      break;

    //Remocao logica de registros a partir de criterios de busca
    case 4:
      
      //Recebe do usuario a quantidade de remocoes
      scanf(" %d", &num_crit);

      fp_bin = fopen(file_bin, "rb+");
      if(fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }

      //Atauliza status para '0'
      read_header_bin(fp_bin, h);
      h->status = '0';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);
      
      for(int i = 0; i < num_crit; i++){
        //Recebe do input do usuario a quantidade de campos a serem usados como criterio de busca
        scanf("%d", &arg_num);

        //Armazena os byteoffsets dos registros buscados que atendem aos criterios, retorna os offsetts dos registros que devem ser atualizados
        offset_array = search_criteria(fp_bin, s, arg_num, &counter);
        
        for(int i = 0; i < counter; i++){
          //Funcao auxiliar que remove os registros cujos byteoffsets estao em offset_array
          remove_criteria(fp_bin, offset_array[i], s, h);
        }
        
        free(offset_array);
      }
       
      //Posiciona o ponteiro no fim de arquivo e armazena-se essa posicao
      fseek(fp_bin, 0, SEEK_END);
      file_end = ftell(fp_bin);

      //Retorna ponteiro para o inicio do arquivo logo apos o cabecalho
      fseek(fp_bin, 17, SEEK_SET);

      //Chamada da arvore para  recontagem de estacao e nroParEstacao, ja que algumas foram, ou nao, removidas
      raiz_MODO_1 = cria_arvore();
      raiz_MODO_2 = cria_arvore();
      while(ftell(fp_bin) < file_end){
        read_register(fp_bin, s);
        if(s->removido == '0'){
          insere_arvore_MODO_1(raiz_MODO_1, s->NomeEstacao);
          insere_arvore_MODO_2(raiz_MODO_2, s->CodEstacao, s->CodProxEst);
        }
      }
      read_header_bin(fp_bin, h);
      h->nroEstacoes = numero_total_nodos(raiz_MODO_1);
      h->nroParesEstacao = numero_total_nodos(raiz_MODO_2);

      //Atualiza status do cabecalho de fechamento do arquivo
      h->status = '1';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);

      binarioNaTela(file_bin);

      libera_arvore(raiz_MODO_1);
      libera_arvore(raiz_MODO_2);

      fclose(fp_bin);

      break;

    //Insercao de registros com aproveitamento de memoria
    case 5:
      
      //Recebe do input do ususario quantos registros serao inseridos
      scanf(" %d", &num_crit);

      fp_bin = fopen(file_bin, "rb+");
      if(fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }
      
      //Atualiza status para '0'
      read_header_bin(fp_bin, h);
      h->status = '0';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);
  
      for(int i = 0; i < num_crit; i++){
        size_reg = receive_full_register_info(fp_bin, s, h);

        insert_bin(fp_bin, s, h, size_reg);
      }
      
      //Posiciona o ponteiro no fim de arquivo e armazena-se essa posicao
      fseek(fp_bin, 0, SEEK_END);
      file_end = ftell(fp_bin);

      //Retorna ponteiro para o inicio do arquivo logo apos o cabecalho
      fseek(fp_bin, 17, SEEK_SET);

      //Chamada da arvore para  recontagem de estacao e nroParEstacao, ja que algumas foram, ou nao, removidas
      raiz_MODO_1 = cria_arvore();
      raiz_MODO_2 = cria_arvore();
      while(ftell(fp_bin) < file_end){
        read_register(fp_bin, s);
        if(s->removido == '0'){
          insere_arvore_MODO_1(raiz_MODO_1, s->NomeEstacao);
          insere_arvore_MODO_2(raiz_MODO_2, s->CodEstacao, s->CodProxEst);
        }
      }
      h->nroEstacoes = numero_total_nodos(raiz_MODO_1);
      h->nroParesEstacao = numero_total_nodos(raiz_MODO_2);

      //Atualiza status do cabecalho de fechamento do arquivo
      h->status = '1';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);

      binarioNaTela(file_bin);

      libera_arvore(raiz_MODO_1);
      libera_arvore(raiz_MODO_2);

      fclose(fp_bin);
      
      break;

    //Atualiza os determinados campos de registros que atendem a determinados criterios
    case 6:
    
      //Recebe do input do ususario quantidade de atualizacoes
      scanf(" %d", &num_crit);

      fp_bin = fopen(file_bin, "rb+");
      if(fp_bin == NULL){
        printf("Falha no processamento do arquivo.");
        return 0;
      }
      
      //Atualiza status para '0'
      read_header_bin(fp_bin, h);
      h->status = '0';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);

      for(int i = 0; i < num_crit; i++){
        //Recebe do input do usuario a quantidade de campos a serem usados como criterio de busca
        scanf("%d", &arg_num);

        //Armazena os byteoffsets dos registros buscados que atendem aos criterios, retorna os offsetts dos registros que devem ser atualizados
        offset_array = search_criteria(fp_bin, s, arg_num, &counter);

        int num_fields;
        scanf("%d ", &num_fields);
        //Armazena em fiel_name_array[i] o nome do campo e em field_value_array[i] o seu valor correspondente
        register_field_info(num_fields, field_name_array, field_value_array);

        //Atualiza os campos, contidos em field_array, dos registros cujos byteoffsets estao contidos em offset_array
        update_register_bin(fp_bin, offset_array, s, h, counter, num_fields, field_name_array, field_value_array);

        free(offset_array);
      }

      //Posiciona o ponteiro no fim de arquivo e armazena-se essa posicao
      fseek(fp_bin, 0, SEEK_END);
      file_end = ftell(fp_bin);

      //Retorna ponteiro para o inicio do arquivo logo apos o cabecalho
      fseek(fp_bin, 17, SEEK_SET);

      //Chamada da arvore para  recontagem de estacao e nroParEstacao, ja que algumas foram, ou nao, removidas
      raiz_MODO_1 = cria_arvore();
      raiz_MODO_2 = cria_arvore();
      while(ftell(fp_bin) < file_end){
        read_register(fp_bin, s);
        if(s->removido == '0'){
          insere_arvore_MODO_1(raiz_MODO_1, s->NomeEstacao);
          insere_arvore_MODO_2(raiz_MODO_2, s->CodEstacao, s->CodProxEst);
        }
      }
      h->nroEstacoes = numero_total_nodos(raiz_MODO_1);
      h->nroParesEstacao = numero_total_nodos(raiz_MODO_2);

      //Atualiza status do cabecalho de fechamento do arquivo
      h->status = '1';
      write_header_bin(fp_bin, h);
      read_header_bin(fp_bin, h);

      binarioNaTela(file_bin);

      libera_arvore(raiz_MODO_1);
      libera_arvore(raiz_MODO_2);

      fclose(fp_bin);
      
      break;

    default:
      break;
  }

  free(s);
  free(h);

  return 0;
}
