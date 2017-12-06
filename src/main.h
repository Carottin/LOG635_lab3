#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define DATASET_FOLDER "../Dataset.csv"
#define MAXSIZE_TITLE 50
#define NB_LIGNE_MAX 4000
#define NB_ATTRIBUTS_MAX 30

#define INDEX_LEAGUE 1
#define NB_ATTRIBUTS_PERTINENTS 5
#define ATTRIBUTS_PERTINENTS {INDEX_LEAGUE, 5, 10, 11, 12, 13}
//#define COEFFICIENT_ATTRIBUTS {1, 0.1, 100, 100, 1, 1, 1, 100, 1, 1, 1, 1, 0.001}

#define DIST_INF (float)2e6
#define LEAGUE_NULL -1
typedef struct point
{
	float distance;
	int ligue;
} str_point;

typedef struct resultat
{
	int nb_reponses_correctes, nb_reponse_fausses;
	//double* lignes_fausses;
} str_resultat;



int open_data(void);
int split_char(char* txt_buf, char** txt_split, char separateur);
int split_char_todouble(char* txt_buf, double* float_split, char separateur);
void print_ligne(int);
void sort_points(str_point*);
double get_distance(double*, double*);



