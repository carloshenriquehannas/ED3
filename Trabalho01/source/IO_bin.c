//TRABALHO PRATICO 1 - ED3. INTEGRANTES:
//CARLOS HENRIQUE HANNAS DE CARVALHO, NUSP: 11965988
//MATHEUS BAPTISTELLA, NUSP: 11223117

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/defines.h"
#include "../headers/funcoesFornecidas.h"
#include "../headers/IO_bin.h"
#include "../headers/functions.h"
#include "../headers/arvore.h"

//Escreve cada campo da struct para o arquivo binario
void write_register_bin(FILE *fp_bin, station_data *s) {
	s->removido = '0';
	s->proxLista = - 1;
	fwrite(&s->removido, sizeof(char), 1, fp_bin);
	fwrite(&s->tamanhoRegistro, sizeof(int), 1, fp_bin);
	fwrite(&s->proxLista, sizeof(long), 1, fp_bin);
	fwrite(&s->CodEstacao, sizeof(int), 1, fp_bin);
	fwrite(&s->CodLinha, sizeof(int), 1, fp_bin);
	fwrite(&s->CodProxEst, sizeof(int), 1, fp_bin);
	fwrite(&s->DistanciaProxEst, sizeof(int), 1, fp_bin);
	fwrite(&s->CodLinhaInteg, sizeof(int), 1, fp_bin);
	fwrite(&s->CodEstacaoInteg, sizeof(int), 1, fp_bin);
	fwrite(s->NomeEstacao, sizeof(char)*strlen(s->NomeEstacao), 1, fp_bin);
	fwrite(s->NomeLinha, sizeof(char)*strlen(s->NomeLinha), 1, fp_bin);

  return;
}

//Escrita do cabecalho no arquivo binario
void write_header_bin(FILE *fp_bin, header_bin *h){

  //Posiciona ponteiro no inicio do arquivo
  fseek(fp_bin, 0, SEEK_SET);

	//Escreve o cabecalho no binario
	fwrite(&h->status, sizeof(char), 1, fp_bin);
	fwrite(&h->topoLista, sizeof(long), 1, fp_bin);
	fwrite(&h->nroEstacoes, sizeof(int), 1, fp_bin);
	fwrite(&h->nroParesEstacao, sizeof(int), 1, fp_bin);

  return;
}

void read_header_bin(FILE *fp_bin, header_bin *h){
  
  //Coloca o ponteiro no inicio do arquivo binario
  fseek(fp_bin, 0, SEEK_SET);
  
  fread(&h->status, sizeof(char), 1, fp_bin);
  fread(&h->topoLista, sizeof(long), 1, fp_bin);
  fread(&h->nroEstacoes, sizeof(int), 1, fp_bin);
  fread(&h->nroParesEstacao, sizeof(int), 1, fp_bin);

  return;
}

//Funcao auxiliar para insercao no arquivo binario
void write_bin(FILE *fp_csv, FILE *fp_bin, station_data *s, header_bin *h) {
  int size_str, i;
  char str[STRING_SIZE];

	arv_dec *raiz_MODO_1;
	arv_dec *raiz_MODO_2;
  raiz_MODO_1 = cria_arvore();
	raiz_MODO_2 = cria_arvore();
  
  //Escrita do cabecalho do arquivo binario
  h->status = '0';
	h->topoLista = - 1;
	h->nroEstacoes = 0;
	h->nroParesEstacao = 0;
	write_header_bin(fp_bin, h);
	
  //Leitura de cada dado do csv para a struct
	while (!feof(fp_csv)) {

		read_csv(fp_csv, str);
		s->CodEstacao = atoi(str);
    if(s->CodEstacao == 0){
      s->CodEstacao = -1;
    }
	
		read_csv(fp_csv, str);
		strcpy(s->NomeEstacao, str);
    size_str = strlen(s->NomeEstacao);
    s->NomeEstacao[size_str + 1] = '\0'; 
    s->NomeEstacao[size_str] = '|';

		read_csv(fp_csv, str);
		s->CodLinha = atoi(str);
    if(s->CodLinha == 0){
      s->CodLinha = -1;
    }

		read_csv(fp_csv, str);
		strcpy(s->NomeLinha, str);
    size_str = strlen(s->NomeLinha);
    s->NomeLinha[size_str + 1] = '\0';
    s->NomeLinha[size_str] = '|';

		read_csv(fp_csv, str);
		s->CodProxEst = atoi(str);
    if(s->CodProxEst == 0){
      s->CodProxEst = -1;
    }

		read_csv(fp_csv, str);
		s->DistanciaProxEst = atoi(str);
    if(s->DistanciaProxEst == 0){
      s->DistanciaProxEst = -1;
    }

		read_csv(fp_csv, str);
		s->CodLinhaInteg = atoi(str);
    if(s->CodLinhaInteg == 0){
      s->CodLinhaInteg = -1;
    }
    
		read_csv(fp_csv, str);
		s->CodEstacaoInteg = atoi(str);
    if(s->CodEstacaoInteg == 0){
      s->CodEstacaoInteg = -1;
    }

    //Insere o registro da struct para o binario
    s->tamanhoRegistro = sizeof(char)*(strlen(s->NomeEstacao) + strlen(s->NomeLinha)) + 6*sizeof(int) + sizeof(long);
		write_register_bin(fp_bin, s);

    //Insercao de cada estacao nos modos 1 e 2 da arvore
		insere_arvore_MODO_1(raiz_MODO_1, s->NomeEstacao);
		insere_arvore_MODO_2(raiz_MODO_2, s->CodEstacao, s->CodProxEst);
	}

  read_header_bin(fp_bin, h);
	h->nroEstacoes = numero_total_nodos(raiz_MODO_1);
	h->nroParesEstacao = numero_total_nodos(raiz_MODO_2);

	h->status = '1';

  //Atualizacao do novo cabecalho no arquivo binario
	write_header_bin(fp_bin, h);

	libera_arvore(raiz_MODO_1);
	libera_arvore(raiz_MODO_2);

  return;
}

int read_register(FILE* fp_bin, station_data *s){
  int i;
  
  fread(&s->removido, sizeof(char), 1, fp_bin);
  fread(&s->tamanhoRegistro, sizeof(int), 1, fp_bin);
	
	long pos_inicial = ftell(fp_bin);
  fread(&s->proxLista, sizeof(long), 1, fp_bin);
	
	if(s->removido == '0'){ //Caso em que o campo nao foi removido
    
    fread(&s->CodEstacao, sizeof(int), 1, fp_bin);
    fread(&s->CodLinha, sizeof(int), 1, fp_bin);
    fread(&s->CodProxEst, sizeof(int), 1, fp_bin);
    fread(&s->DistanciaProxEst, sizeof(int), 1, fp_bin);
    fread(&s->CodLinhaInteg, sizeof(int), 1, fp_bin);
    fread(&s->CodEstacaoInteg, sizeof(int), 1, fp_bin);

    //Leitura de char por char do binario ate encontrar '|'
		i = -1;
    do{
      i++;
      fread(&s->NomeEstacao[i], sizeof(char), 1, fp_bin);
    } while(s->NomeEstacao[i] != '|');
		s->NomeEstacao[i] = '\0'; //Armazena '\0' ao lugar do '|'
    
    //Leitura de char por char do binario ate encontrar '|'
    i = -1;
    do{
      i++;
      fread(&s->NomeLinha[i], sizeof(char), 1, fp_bin);
    } while(s->NomeLinha[i] != '|');
		s->NomeLinha[i] = '\0'; //Armazena '\0' ao lugar do '|'

    //Tratamento para casos em que ha caracter lixo '$' no binario
		long pos_final = ftell(fp_bin);
		int qtd_bytes_lidos = pos_final - pos_inicial;
		int qtd_lixo = s->tamanhoRegistro - (qtd_bytes_lidos);
		fseek(fp_bin, qtd_lixo, SEEK_CUR);
		
  } 
  else{ //Caso em que o campo foi removido
    fseek(fp_bin, s->tamanhoRegistro - sizeof(long), SEEK_CUR); //Ajuste de aponte para o proximo registro
    return 1;
  }

	return SUCESSO;
} 

//Funcao para ler o arquivo binario
void read_bin(FILE *fp_bin, station_data *s){
	int x, empty_file;
  
  empty_file = 1;

  fseek(fp_bin, 0, SEEK_END);
  int file_end = ftell(fp_bin);
	
  //Pula o cabecalho do arquivo binario
  fseek(fp_bin, 17, SEEK_SET); 

  //Ler cada campo do binario para struct
  while(ftell(fp_bin) < file_end) {

    x = read_register(fp_bin, s);

    //Caso em que o campo nao foi removido
    if(x == SUCESSO){
      empty_file = 0;
      print_register(s);
    }
  }

  if(empty_file){
    printf("Registro inexistente.");
  }

  return;
}

long *search_criteria(FILE *fp_bin, station_data *s, int arg_num, int *counter){
  char *field_name;
  char *field_string_value;
  int field_int_value;

  //Condicao de parada dos loops
  fseek(fp_bin, 0, SEEK_END);
  long file_end = ftell(fp_bin);

  *counter = 0;
  long *offset_storage;
  long offset;

  field_name = (char*)malloc(sizeof(char) * SIZE_FIELD_NAME);
  field_string_value = (char*)malloc(sizeof(char) * STRING_SIZE);
  offset_storage = (long*)malloc(sizeof(long) * (*counter + 1));

  //Pula o cabecalho do arquivo binario
  fseek(fp_bin, 17, SEEK_SET);

  for(int i = 0; i < arg_num; i++){

    //Le o nome do campo a ser usado no criterio de busca
    scanf(" %s", field_name);
  
    //Verifica se o valor do campo vai ser uma string ou um inteiro
    if((strcmp(field_name, "nomeEstacao") == 0) || (strcmp(field_name, "nomeLinha") == 0)){

      //Se for uma string, chama a funcao que le a string com aspas
      scan_quote_string(field_string_value);

      //Reduz o alcance de busca,definindo quantos registros atendem ao primeiro criterio de busca.
      if(i == 0){

        //Neste caso e necessario varrer todo o arquivo binario
        while(ftell(fp_bin) < file_end){

          //Ja guarda o offset deste registro antes que o ponteiro mude
          offset = ftell(fp_bin);

          //Se o registro nao foi logicamente removido, desta forma os registros removidos nao sao pesquisados futuramente
          if(!read_register(fp_bin, s)){

            //Chama funcao auxiliar que retorna TRUE se o registro contem o criterio
            if(contains_string_criteria(field_name, field_string_value, s)){

              //Salva o valor do offset
              offset_storage[(*counter)] = offset;
              (*counter)++;

              offset_storage = (long*)realloc(offset_storage, sizeof(long) * (*counter + 1));
            }
          } 
        }
      }
      else{

        //Para os proximos criterios, se existirem, buscam-se apenas naqueles registros que continham o primeiro criterio
        for(int j = 0; j < (*counter); j++){
          if(offset_storage[j] != -1){
            fseek(fp_bin, offset_storage[j], SEEK_SET);
            read_register(fp_bin, s);

            //Se o registro nao atende a um outro criterio, marca-se como removido do offset_storage com -1
            if(!contains_string_criteria(field_name, field_string_value, s)){
              offset_storage[j] = -1;
            }
          }
        }
      }
    }
    else{

      //Se for um inteiro, le o inteiro
      scanf(" %d", &field_int_value);

      //Reduz o alcance de busca, definindo quantos registros atendem ao primeiro criterio de busca.
      if(i == 0){

        //Neste caso e necessario varrer todo o arquivo binario
        while(ftell(fp_bin) < file_end){
          offset = ftell(fp_bin);
          
          //Se o registro nao foi logicamente removido, desta forma os registros removidos nao sao pesquisados futuramente
          if(!read_register(fp_bin, s)){

            //Chama funcao auxiliar que retorna TRUE se o registro contem o criterio
            if(contains_int_criteria(field_name, field_int_value, s)){

              //Salva o valor do offset
              offset_storage[(*counter)] = offset;
              (*counter)++;

              offset_storage = (long*)realloc(offset_storage, sizeof(long) * (*counter + 1));
            }
          }
        }
      }
      else{

        //Para os proximos criterios, se existirem, buscam-se apenas naqueles registros que continham o primeiro criterio
        for(int j = 0; j < (*counter); j++){
          if(offset_storage[j] != -1){
            fseek(fp_bin, offset_storage[j], SEEK_SET);
            read_register(fp_bin, s);

            //Se o registro nao atende a um outro criterio, marca-se como removido do offset_sotrage com -1
            if(!contains_int_criteria(field_name, field_int_value, s)){
              offset_storage[j] = -1;
            }
          }
        }
      }
    }
  }
  //Com o fim do ciclo, os offsets que restaram em offset_storage representam os registros que atendem a todos os criterios

  free(field_name);
  free(field_string_value);
  
  return offset_storage;
}

void print_criteria(FILE *fp_bin, long *offset_storage, station_data *s, int *counter){
  int print_counter = 0;

  for(int i = 0; i < (*counter); i++){
    if(offset_storage[i] != -1){
      fseek(fp_bin, offset_storage[i], SEEK_SET);
      read_register(fp_bin, s);
      print_register(s);
      print_counter++;
    }
  }

  //Se nenhum registro foi imprimido, significa que nenhum atendeu aos criterios
  if(print_counter == 0){
    printf("Registro inexistente.");
  }

  return;
}

void remove_criteria(FILE *fp_bin, long offset_storage, station_data *s, header_bin *h){
  int aux;
  
  fseek(fp_bin, 0, SEEK_SET);

  if(offset_storage != -1){

    read_header_bin(fp_bin, h);

    //Posiciona o ponteiro do arquivo no byteoffset do inicio do registro
    fseek(fp_bin, offset_storage, SEEK_SET);

    //Marca o registro como logicamente removido
    s->removido = '1';
    fwrite(&s->removido, sizeof(char), 1, fp_bin);
    
    //Pula o campo de tamanhoRegistro
    fseek(fp_bin, sizeof(int), SEEK_CUR);

    fwrite(&h->topoLista, sizeof(long), 1, fp_bin);

    //Atualiza o cabecalho com novo valor de topoLista
    h->topoLista = offset_storage;

    //Atualizacao do cabecalho
    write_header_bin(fp_bin, h);
  }
  
  return;
}

int receive_full_register_info(FILE *fp_bin, station_data *s, header_bin *h){
  char temp[5];
  int size_reg;

  //Le todos os campos do registro a ser inserido, e armazena na struct
  scanf("%d", &s->CodEstacao);
  scan_quote_string(s->NomeEstacao);

  scanf("%d", &s->CodLinha);
  scan_quote_string(s->NomeLinha);

  scanf("%s", temp);
  if(strcmp(temp, "NULO") == 0){
    s->CodProxEst = -1;
  }
  else{
    s->CodProxEst = atoi(temp);
  }

  scanf("%s", temp);
  if(strcmp(temp, "NULO") == 0){
    s->DistanciaProxEst = -1;
  }
  else{
    s->DistanciaProxEst = atoi(temp);
  }

  scanf("%s", temp);
  if(strcmp(temp, "NULO") == 0){
    s->CodLinhaInteg = -1;
  }
  else{
    s->CodLinhaInteg = atoi(temp);
  }

  scanf("%s", temp);
  if(strcmp(temp, "NULO") == 0){
    s->CodEstacaoInteg = -1;
  }
  else{
    s->CodEstacaoInteg = atoi(temp);
  }

	//Insere o caracter '|' ao final da string
	int size_str = strlen(s->NomeEstacao);
  s->NomeEstacao[size_str + 1] = '\0';
  s->NomeEstacao[size_str] = '|';

  //Insere o caracter '|' ao final da string
  size_str = strlen(s->NomeLinha);
  s->NomeLinha[size_str + 1] = '\0';
  s->NomeLinha[size_str] = '|';
  
  //Tamanho do registro a ser inserido
  size_reg = sizeof(char)*(strlen(s->NomeEstacao) + strlen(s->NomeLinha)) + 6*sizeof(int) + sizeof(long);

  return size_reg;
}

void insert_bin(FILE* fp_bin, station_data *s, header_bin *h, int size_reg){
  int size_dif;
  station_data *aux_rem = malloc(sizeof(station_data));

 //Pega o byteoffset do ultimo registro removido em topoLista
  long offset;
  read_header_bin(fp_bin, h);

  //Coloca o byteoffset do ultimo registro removido
  offset = h->topoLista;

  //Caso em que nao ha registros removidos. Insercao no final do arquivo binario
  if(offset == -1){
    fseek(fp_bin, 0, SEEK_END);
    s->tamanhoRegistro = size_reg;
    write_register_bin(fp_bin, s);
  }
  //Insercao no espaco de um registro removido
  else{
    //Retorna TRUE se encontrou espaco em algum registro
    if(available_space(fp_bin, aux_rem, h, &offset, size_reg)){
      fseek(fp_bin, offset, SEEK_SET);
      
      size_dif = aux_rem->tamanhoRegistro - size_reg;
      
      s->tamanhoRegistro = aux_rem->tamanhoRegistro;
 
      write_register_bin(fp_bin, s);
      fill_trash(fp_bin, size_dif);
    }
    //Insercao no final do arquivo binario
    else{
      fseek(fp_bin, 0, SEEK_END);
      s->tamanhoRegistro = size_reg;
      write_register_bin(fp_bin, s);
    }
  }

  write_header_bin(fp_bin, h);

  return;
}

int available_space(FILE *fp_bin, station_data *aux_rem, header_bin *h, long *offset, int size_reg){
  int flag = 0;
  long tempOffset;

  fseek(fp_bin, *offset, SEEK_SET);
  while(*offset != -1){

    //Para um registro removido, tem-se acesso as informacoes de removido, tamanhoRegistro e proxLista apenas, o resto nao eh lido
    read_register(fp_bin, aux_rem);
    
    //Verifica qual registro removido possui tamanho que acomode as informacoes do novo registro
    if(size_reg <= aux_rem->tamanhoRegistro){
      if(flag == 0){

        //Coloca no topoLista o offset do proximo registro removido
        h->topoLista = aux_rem->proxLista;
      }
      else{

        //Coloca no registro removido anterior ao que deu certo, o proxLista do registro que deu certo
        fseek(fp_bin, tempOffset + 5, SEEK_SET);
        fwrite(&aux_rem->proxLista, sizeof(long), 1, fp_bin);
      }
   
      return 1;
    }
    flag++;

    //Armazena o offset do registro removido anterior
    tempOffset = *offset;
    
    *offset = aux_rem->proxLista;
    fseek(fp_bin, aux_rem->proxLista, SEEK_SET);
  }
  //Se chegar aqui eh porque nenhum registro foi removido ou o registro a ser inserido e maior que todos que foram removidos

  return 0;
}

void fill_trash(FILE *fp_bin, int size_dif) {
	char trash = '$';
  //Escreve o caractere de lixo no tamanho reservado no binario
	for (int i = 0; i < size_dif; i++) {
		fwrite(&trash, sizeof(char), 1, fp_bin);
	}

  return;
}

void register_field_info(int num_fields, char *field_name_array[MAX_FIELDS], char *field_value_array[MAX_FIELDS]){

  //Grava no vetor de posicoes de 16bytes os nomes dos campos e na mesma posicao do outro  vetor de posicoes de 75bytes o valor do campo.
  for(int i = 0; i < num_fields; i++){

    field_name_array[i] = (char*)malloc(sizeof(char*) * SIZE_FIELD_NAME);
    field_value_array[i] = (char*)malloc(sizeof(char*) * STRING_SIZE);

    scanf("%s ", field_name_array[i]);

    //Se for um desses dois nomes de cmapo, sabe que o valor eh uma string
    if((strcmp(field_name_array[i], "nomeEstacao") == 0) || (strcmp(field_name_array[i], "nomeLinha") == 0)){
      scan_quote_string(field_value_array[i]);
    }
    else{
      //Se nao, eh um inteiro
      scanf("%s", field_value_array[i]);
    }
  }

  return;
}

void update_register_bin(FILE *fp_bin, long *offset_array, station_data *s, header_bin *h, int counter, int num_fields, char *field_name_array[MAX_FIELDS], char *field_value_array[MAX_FIELDS]){
  int size_str;
  int size_dif;

  //Quantidade de registros que vao ser atualizados
  for(int i = 0; i < counter; i++){
    
    //Vai ate o offset do registro
    fseek(fp_bin, offset_array[i], SEEK_SET);

    if(offset_array[i] != -1){
    
      //Leitura do regitro
      read_register(fp_bin, s);

      //Para cada registro, le os campos que devem ser atualizados
      for(int j = 0; j < num_fields; j++){
        
        //Atualiza o campo "nomeEstacao"
        if(strcmp(field_name_array[j], "nomeEstacao") == 0){
          strcpy(s->NomeEstacao, field_value_array[j]);
        }
        //Atualiza o campo "nomeLinha"
        else if(strcmp(field_name_array[j], "nomeLinha") == 0){
          strcpy(s->NomeLinha, field_value_array[j]);
        }
        //Atualiza o campo "codEstacao"
        else if(strcmp(field_name_array[j], "codEstacao") == 0){
          if(strcmp(field_value_array[j], "NULO") == 0){
            s->CodEstacao = -1;
          }
          else{
            s->CodEstacao = atoi(field_value_array[j]);
          }
        }
        //Atualiza o campo "codLinha"
        else if(strcmp(field_name_array[j], "codLinha") == 0){
          if(strcmp(field_name_array[j], "NULO") == 0){
            s->CodLinha = -1;
          }
          else{
            s->CodLinha = atoi(field_value_array[j]);
          }
        }
        //Atualiza o campo "codProxEst"
        else if(strcmp(field_name_array[j], "codProxEstacao") == 0){
          if(strcmp(field_name_array[j], "NULO") == 0){
            s->CodProxEst = -1;
          }
          else{
            s->CodProxEst = atoi(field_value_array[j]);
          }
        }
        //Atualiza o campo "distProxEstacao"
        else if(strcmp(field_name_array[j], "distProxEstacao") == 0){
          if(strcmp(field_name_array[j], "NULO") == 0){
            s->DistanciaProxEst = -1;
          }
          else{
            s->DistanciaProxEst = atoi(field_value_array[j]);
          }
        }
        //Atualiza o campo "codLinhaIntegra"
        else if(strcmp(field_name_array[j], "codLinhaIntegra") == 0){
          if(strcmp(field_name_array[j], "NULO") == 0){
            s->CodLinhaInteg = -1;
          }
          else{
            s->CodLinhaInteg = atoi(field_value_array[j]);
          }
        }
        //Atualiza o campo "codEstacaoIntegrada"
        else if(strcmp(field_name_array[j], "codEstIntegra") == 0){
          if(strcmp(field_name_array[j], "NULO") == 0){
            s->CodEstacaoInteg = -1;
          }
          else{
            s->CodEstacaoInteg = atoi(field_value_array[j]);
          }
        }
      }
      //Aqui deve salvar o nomeLinha  e nomeEstacao com '|' na struct, para contar o tamnho correto de bytes
      size_str = strlen(s->NomeLinha);
      s->NomeLinha[size_str + 1] = '\0';
      s->NomeLinha[size_str] = '|';

      size_str = strlen(s->NomeEstacao);
      s->NomeEstacao[size_str + 1] = '\0';
      s->NomeEstacao[size_str] = '|';

      //A partir daqui, a struct s possui os valores antigos do registro e ainda os valores que foram atualizados

      //Calcula o tamanho do novo registro a ser atualizado
      int size_reg = sizeof(char)*(strlen(s->NomeEstacao) + strlen(s->NomeLinha)) + 6*sizeof(int) + sizeof(long);
      
      //Verifica se o registro atualizado é menor ou igual ao registro anterior
      if(size_reg <= s->tamanhoRegistro){
        
        //Volta o ponteiro para o comeco do registro que vai ser atualizado
        fseek(fp_bin, offset_array[i], SEEK_SET);

        size_dif = s->tamanhoRegistro - size_reg;

        write_register_bin(fp_bin, s);
        fill_trash(fp_bin, size_dif);
      }
      //Caso contrario, verifica se ha outros registros removidos para inseri-lo
      else{
        //Marca o registro como logicamente removido
        remove_criteria(fp_bin, offset_array[i], s, h);
        //Procura outro registro removido para inserir no lugar ou no final do arquivo
        insert_bin(fp_bin, s, h, size_reg);
      }
    }
  }

  for(int i = 0; i < num_fields; i++){
    free(field_name_array[i]);
    free(field_value_array[i]);
  }
  
  return;
}
