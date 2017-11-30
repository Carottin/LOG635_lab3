#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;



int main(int argc, int* argv[])
{
	double** matrice_data = (double**)malloc(sizeof(double*) * NB_LIGNE_MAX); // Données des classes	
	char** title_data = (char**)malloc(sizeof(char*)*NB_ARGUMENTS_MAX); // nom de la donnée
	int nb_ligne, nb_critere;

	open_data(matrice_data, title_data, &nb_ligne, &nb_critere);

	for (int i = 0; i < nb_critere; i++)
	{

		cout << title_data[i] << "->" <<  matrice_data[14][i] << endl;
	}
	

	return 0;
}


// Open the csv file
// Fill matrice data
int open_data(double** matrice_data, char** title_data, int* nb_ligne, int* nb_critere) {
	FILE* file = NULL; // create a new file
	file = fopen(DATASET_FOLDER, "r");  // open csv file
	if (file != NULL) {
		char txt_buf[500];
		fgets(txt_buf, 500, file);
		*nb_critere = split_char(txt_buf, title_data, ',');

		
		int i = 0;
		while (fgets(txt_buf, 500, file) != NULL)
		{
			matrice_data[i] = (double*)malloc(sizeof(double)*(*nb_critere));
			split_char_todouble(txt_buf, matrice_data[i], ',');
			i++;
		}
		*nb_ligne = i;
		fclose(file);
	}
	else
	{
		printf("erreur d'ouverture de fichier");
	}
	
	return 0;
	
}

// Split txt_buf with by separateur in txt_split
int split_char(char* txt_buf, char** txt_split, char separateur) {
	int sizeof_split = 0;
	for (int i = 0; txt_buf[i] != '\n'; i++) if (txt_buf[i] == separateur) sizeof_split++;
		
	for (int i = 0; i < sizeof_split + 1; i++) {
		txt_split[i] = (char*)malloc(sizeof(char)*MAXSIZE_TITLE);
	}
	int i = 0, j = 0, k = 0;
	while (txt_buf[j] != '\n') {
		if (txt_buf[j] == separateur) {
			txt_split[i][k] = '\0';
			i++;
			k = 0;
		}
		else {
			txt_split[i][k] = txt_buf[j];
			k++;
		}
		j++;
	}
	txt_split[i][k] = '\0';
	return sizeof_split + 1;
}

// same as split_char but convert in double
int split_char_todouble(char* txt_buf, double* double_split, char separateur) {
	
	char tmp[30];

	int i = 0, j = 0, k = 0;
	while (txt_buf[j] != '\n' ) {
		if (txt_buf[j] == separateur) {
			tmp[k] = EOF;
			double_split[i] = atof(tmp);
			i++;
			k = 0;
		}
		else {
			tmp[k] = txt_buf[j];
			k++;
		}
		j++;
	}
	tmp[k] = '\0';
	string s = string(tmp);
	if (s.find('"', 0) != std::string::npos) {
		s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
		cout << s << endl;
		istringstream os(s);
		double d;
		os >> d;
		cout << d << endl;
		double_split[i] = d;
	}
	else {
		double_split[i] = atof(s.c_str());
	}
	return i;
	
}
