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
int nb_lignes;
int nb_critere;

int main(int argc, char* argv[])
{
	int nb_lignes_bdd = 3000;
	n_k = 4;
	if (argc >= 2) {//nb de ligne de test 
		nb_lignes_bdd = atoi(argv[1]);
	}
	if (argc >= 3) {//nb de points proche à chercher
		n_k = atoi(argv[2]);
	}

	str_resultat resultat;
	resultat.nb_reponses_correctes = 0;
	resultat.nb_reponse_fausses = 0;

	matrice_data = (double**)malloc(sizeof(double*) * NB_LIGNE_MAX); // Données des classes	
	title_data = (char**)malloc(sizeof(char*)*NB_ATTRIBUTS_MAX); // nom de la donnée

	nb_lignes = open_data();
	nb_lignes_bdd = 0.66*nb_lignes;
	normalize_data();

	// testing metrics
	int league_count_dataset[8];
	int league_count_test[8];
	float precisions[8];
	float rappels[8];
	float fmesures[8];
	float errmoys[8];
	int predictions_correctes[8];
	float errmoy = 0.0f;

	for (int i = 0; i < 8; i++) {
		league_count_dataset[i] = 0.0f;
		league_count_test[i] = 0.0f;
		precisions[i] = 0.0f;
		rappels[i] = 0.0f;
		fmesures[i] = 0.0f;
		errmoys[i] = 0.0f;
		predictions_correctes[i] = 0;
	}
	//print_ligne(14);

	/////////////////////////////////////////////////////////////////////////////////////////test1

	//initialisation de tableau qui va stocker les points les plus proches.
	str_point* points_proches = (str_point*)malloc(sizeof(str_point)*n_k);
	double tmp_distance = 0;


	////algo en lui même
	//je definis un certain nombre de ligne qui vont servir de reference appellées lignes base de données (bdd)
	//le X lignes bdd sont comparées une a une au nouveau point
	//et l'on rmplis le tableau des point les plus proches de n_k specifié en parametre.

	for (int indice_ligne_test = nb_lignes_bdd; indice_ligne_test < nb_lignes; indice_ligne_test++) { // on parcours toutes les lignes inconnues	
		for (int i = 0; i < n_k; i++)
		{
			points_proches[i].distance = DIST_INF;
			points_proches[i].ligue = LEAGUE_NULL;
		}
		for (int indice_ligne_bdd = 0; indice_ligne_bdd < nb_lignes_bdd; indice_ligne_bdd++) { //par rapport au lignes connus
			
			tmp_distance = get_distance(matrice_data[indice_ligne_test], matrice_data[indice_ligne_bdd]);

			if (tmp_distance < points_proches[n_k - 1].distance) {
				points_proches[n_k - 1].distance = tmp_distance;
				points_proches[n_k - 1].ligue = matrice_data[indice_ligne_bdd][nb_critere];

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
			//cout << "ligue point proche : " << points_proches[i].ligue << endl;
		}
		int ligue_trouve = roundf((float)moyenne_ligue_trouve / (float)n_k);
		int ligue_reelle = matrice_data[indice_ligne_test][nb_critere];
		//int ligue_trouve = points_proches[0].ligue;
		cout << "ligue : " << ligue_reelle << ". Valeur prédite : " << ligue_trouve << endl;
		if (ligue_trouve == ligue_reelle) {
			resultat.nb_reponses_correctes++;
			precisions[ligue_trouve - 1]++;
			rappels[ligue_trouve - 1]++;
			predictions_correctes[ligue_trouve - 1]++;
		}
		else {
			resultat.nb_reponse_fausses++;
			errmoys[ligue_reelle - 1] += abs(ligue_reelle-ligue_trouve);
			errmoy += abs(ligue_reelle - ligue_trouve);
		}
		league_count_dataset[ligue_reelle - 1]++;
		league_count_test[ligue_trouve - 1]++;
	}

	//metrics
	for (int i = 0; i < 8; i++) {
		precisions[i] = precisions[i]/ league_count_test[i];
		rappels[i] = rappels[i] / league_count_dataset[i];
		fmesures[i] = (2* precisions[i] * rappels[i])/(precisions[i] + rappels[i]);
		errmoys[i] = errmoys[i] / league_count_dataset[i];
	}
	errmoy = errmoy / (nb_lignes - nb_lignes_bdd);

	double taux = ((double)resultat.nb_reponses_correctes / (double)(resultat.nb_reponses_correctes + resultat.nb_reponse_fausses)) * 100;
	printf("%d lignes de base de conaissances, %d lignes teste, n_k = %d, taux de bonne reponse : %.2f\n", nb_lignes_bdd, nb_lignes- nb_lignes_bdd, n_k, taux);

	cout << endl << "Erreur moyenne: " << errmoy << endl;
	cout << endl << "Metriques par ligue: " << endl << endl;

	for (int i = 0; i < 8; i++) {
		if (league_count_dataset[i] > 0) {
			cout << "Ligue: " << i + 1 << endl;
			cout << "Nombre de tuples dans la base de connaissance: " << league_count_dataset[i] << endl;
			cout << "Predictions correctes: " << predictions_correctes[i] << endl;
			cout << "Precision: " << precisions[i] << endl;
			cout << "Rappel: " << rappels[i] << endl;
			cout << "F mesure: " << fmesures[i] << endl;
			cout << "Erreur moyenne: " << errmoys[i] << endl << endl;
		}
	}

	system("pause");
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
		nb_critere = split_char(txt_buf, title_data, ',');

		int i = 0;
		while (fgets(txt_buf, 500, file) != NULL)
		{
			if (txt_buf == "\n") break;
			matrice_data[i] = (double*)malloc(sizeof(double)*(nb_critere + NB_ATTRIBUTS_PERTINENTS));
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
			(double)matrice_data[i][j+nb_critere] = (double)matrice_data[i][tab_indice[j]];
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
	for (int i = nb_critere + 1; i < nb_critere + NB_ATTRIBUTS_PERTINENTS; i++) {
		tmp += (point_A[i] - point_B[i])*(point_A[i] - point_B[i]);
	}
	return sqrt(tmp);
}

void normalize_data() {
	for (int j = nb_critere + 1; j < nb_critere + NB_ATTRIBUTS_PERTINENTS; j++) {
		double max = 0;
		double min = DBL_MAX;
		for (int i = 0; i < nb_lignes; i++) {
			if (matrice_data[i][j] > max)
				max = matrice_data[i][j];
			if (matrice_data[i][j] < min)
				min = matrice_data[i][j];
		}
		for (int i = 0; i < nb_lignes; i++) {
			matrice_data[i][j] = (matrice_data[i][j] - min) / (max - min);
		}
	}
}





