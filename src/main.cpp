#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;
double** matrice_data;
char** title_data;
int n_k;


int main(int argc, char* argv[])
{
	int nb_lignes_bdd = 3000;
	n_k = 5;
	if (argc >= 2) {//nb de ligne de test 
		nb_lignes_bdd = atoi(argv[1]);
	}
	if (argc >= 3) {//nb de points proche à chercher
		n_k = atoi(argv[2]);
	}

	str_resultat resultat;
	resultat.nb_reponses_correct = 0;
	resultat.nb_reponse_fausses = 0;

	matrice_data = (double**)malloc(sizeof(double*) * NB_LIGNE_MAX); // Données des classes	
	title_data = (char**)malloc(sizeof(char*)*NB_ATTRIBUTS_MAX); // nom de la donnée

	int nb_lignes = open_data();

	//print_ligne(14);

	/////////////////////////////////////////////////////////////////////////////////////////test1

	//initialisation de tableau qui va stocker les points les plus proches.
	str_point* points_proches = (str_point*)malloc(sizeof(str_point)*n_k);
	double tmp_distance = 0;
	for (int i = 0; i < n_k; i++)
	{
		points_proches[i].distance = DIST_INF;
		points_proches[i].ligue = LEAGUE_NULL;
	}


	////algo en lui même
	//je definis un certain nombre de ligne qui vont servir de reference appellées lignes base de données (bdd)
	//le X lignes bdd sont comparées une a une au nouveau point
	//et l'on rmplis le tableau des point les plus proches de n_k specifié en parametre.

	for (int indice_ligne_test = nb_lignes_bdd; indice_ligne_test < nb_lignes; indice_ligne_test++) { // on parcours toutes les lignes inconnues		
		for (int indice_ligne_bdd = 0; indice_ligne_bdd < nb_lignes_bdd; indice_ligne_bdd++) { //par rapport au lignes connus
			
			tmp_distance = get_distance(matrice_data[indice_ligne_test], matrice_data[indice_ligne_bdd]);

			if (tmp_distance < points_proches[n_k - 1].distance) {
				points_proches[n_k - 1].distance = tmp_distance;
				points_proches[n_k - 1].ligue = matrice_data[indice_ligne_bdd][0];

				//trie
				str_point tmp_point;
				for (int i = n_k - 1; i > 0; i--)
				{
					if (points_proches[i].distance < points_proches[i - 1].distance) {
						tmp_point = points_proches[i];
						points_proches[i] = points_proches[i - 1];
						points_proches[i - 1] = tmp_point;
					}
				}
			}
		}

		// a partir d'ici on a les n_k points les plus proches soluce 1 on prend le plus proche
		int moyenne_ligue_trouve = 0;
		for (int i = 0; i < n_k; i++)
		{
			moyenne_ligue_trouve += points_proches[i].ligue;
		}
		int ligue_trouve = roundf((float)moyenne_ligue_trouve / (float)n_k);
		//int ligue_trouve = points_proches[0].ligue;
		if (ligue_trouve == matrice_data[indice_ligne_test][0]) {
			resultat.nb_reponses_correct++;
		}
		else {
			resultat.nb_reponse_fausses++;
		}
	}

	double taux = (double)resultat.nb_reponses_correct / (double)(resultat.nb_reponses_correct + resultat.nb_reponse_fausses) * 100;
	printf("%d lignes de base de conaissances, %d lignes teste, n_k = %d, taux de bonne reponse : %.2f\n", nb_lignes_bdd, nb_lignes- nb_lignes_bdd, n_k, taux);


	return 0;
}


// Open the csv file
// Fill matrice data
int open_data() {

	int nb_ligne = 0;
	FILE* file = NULL; // create a new file
	file = fopen(DATASET_FOLDER, "r");  // open csv file

	if (file != NULL) {
		char txt_buf[500];
		fgets(txt_buf, 500, file);
		int nb_critere = split_char(txt_buf, title_data, ',');

		int i = 0;
		while (fgets(txt_buf, 500, file) != NULL)
		{
			matrice_data[i] = (double*)malloc(sizeof(double)*nb_critere);
			split_char_todouble(txt_buf, matrice_data[i], ',');
			i++;
		}
		fclose(file);
		nb_ligne = i;
	}
	else
	{
		printf("erreur d'ouverture de fichier");
	}
	//CLEAN MATRICE
	int tab_indice[] = ATTRIBUTS_PERTINENTS;

	for (int i = 0; i < NB_ATTRIBUTS_PERTINENTS; i++) {
		title_data[i] = title_data[tab_indice[i]];
	}


	for (int i = 0; i < nb_ligne; i++) {
		for (int j = 0; j < NB_ATTRIBUTS_PERTINENTS; j++) {
			(double)matrice_data[i][j] = (double)matrice_data[i][tab_indice[j]];
		}
		//realloc((double*)matrice_data[i], NB_ATTRIBUTS_PERTINENTS + 1);
	}
	//realloc((void*)matrice_data, *nb_ligne);
	//realloc((void*)title_data,NB_ATTRIBUTS_PERTINENTS);
	return nb_ligne;
}

void print_ligne(int nb) {
	for (int i = 0; i < NB_ATTRIBUTS_PERTINENTS; i++) {
		cout << title_data[i] << "->" << matrice_data[nb][i] << endl;
	}

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
	while (txt_buf[j] != '\n') {
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
		istringstream os(s);
		double d;
		os >> d;
		double_split[i] = d;
	}
	else {
		double_split[i] = atof(s.c_str());
	}
	return i;

}


double get_distance(double* point_A, double* point_B) {
	double tmp = 0;
	for (int i = 1; i < NB_ATTRIBUTS_PERTINENTS; i++) {
		tmp += (point_A[i] + point_B[i])*(point_A[i] + point_B[i]);
	}
	return sqrt(tmp);
}





