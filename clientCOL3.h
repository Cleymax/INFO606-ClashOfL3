#ifndef CLIENTCOL3_H_INCLUDED
#define CLIENTCOL3_H_INCLUDED

#include "col3-bibtp/communCOL3-TP.h"

/* 
   structure d'un clan. contient tous les champs nécessaires
   aux TPs
*/
typedef struct {
	char nomDuClan[TAILLE_MAX_NOM_CLAN];
	char monToken[255];
	int portTP1;
	int portTP2;
	int portTP3;
	char adresseSrvCol3[20];
    capacite_clan mesCapacites;
    hutte maHutte;
} clan_client ;

/* 
  variable externe utilisée dans les
  fonctions des TPS
*/
extern clan_client MONCLAN;

void afficheMenuClient();
int  estMenuValide(char menu[]);

void afficheMesInfos();
int  testConnexionServeur();
void recupSiteExtraction();
void gestionAppro();







#endif // CLIENTCOL3_H_INCLUDED
