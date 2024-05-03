#ifndef _functions_
#define _functions_

#include <stdio.h>
#include <stdlib.h>
#include "../headers/defines.h"

//Leitura do arquivo csv
void read_csv(FILE *fp_csv, char *str);

//Impressao dos dados de um registro
void print_register(station_data *s);

//Comparacao para verificar o conteudo de um campo especifico
int contains_string_criteria(char *field_name, char *field_string_value, station_data *s);
int contains_int_criteria(char *field_name, int field_int_value, station_data *s);

#endif