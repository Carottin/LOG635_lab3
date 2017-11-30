#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define DATASET_FOLDER "../Dataset.csv"
#define MAXSIZE_TITLE 50
#define NB_LIGNE_MAX 4000
#define NB_ARGUMENTS_MAX 30




int open_data(double** matrice_data, char** title_data, int* nb_ligne, int* nb_critere);
int split_char(char* txt_buf, char** txt_split, char separateur);
int split_char_todouble(char* txt_buf, double* float_split, char separateur);

