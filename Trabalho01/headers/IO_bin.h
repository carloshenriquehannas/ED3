#ifndef _IO_bin_
#define _IO_bin_

#include <stdio.h>
#include <stdlib.h>
#include "../headers/defines.h"

//Funcoes sobre escrita e leitura do binario
void write_register_bin(FILE *fp_bin, station_data *s);
void write_header_bin(FILE *fp_bin, header_bin *h);
void read_header_bin(FILE *fp_bin, header_bin *h);
void write_bin(FILE *fp_csv, FILE *fp_bin, station_data *s, header_bin *h);
void read_bin(FILE *fp_bin, station_data *s);
int read_register(FILE *fp_bin, station_data *s);

//Funcoes relacionadas aos criterios de busca
long *search_criteria(FILE *fp_bin, station_data *s,  int arg_num, int *counter);
void print_criteria(FILE *fp_bin, long *offset_storage, station_data *s, int *counter);

//Funcao relacionada a remocao logica de registros
void remove_criteria(FILE *fp_bin, long offset_storage, station_data *s, header_bin *h);

//Funcoes relacionadas a insercao de registros
int receive_full_register_info(FILE *fp_bin, station_data *s, header_bin *h);
void insert_bin(FILE* fp_bin, station_data *s, header_bin *h, int size_reg);
int available_space(FILE *fp_bin, station_data *aux_rem, header_bin *h, long *offset, int size_reg);
void fill_trash(FILE *fp_bin, int size_dif);

//Funcao de atualizacao de registros
void update_register_bin(FILE *fp_bin, long *offset_array, station_data *s, header_bin *h, int counter, int num_fields, char *field_name_array[MAX_FIELDS], char *field_value_array[MAX_FIELDS]);
void register_field_info(int num_fields, char *field_name_array[MAX_FIELDS], char *field_value_array[MAX_FIELDS]);

#endif