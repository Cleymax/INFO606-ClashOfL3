#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "col3-bibtp/communCOL3-TP.h"
#include "clientCOL3.h"


extern clan_client MONCLAN;

/* ===================================
     fonction d'extraction des sites 
       (a completer)
  ==================================*/
		   
void recupSiteExtraction()
{
	int socket;

	logClientCOL3(info,"testConnexionServeur", 
		"le clan[%s] crée une socket pour tester le serveur",
		MONCLAN.nomDuClan);
	
	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3,MONCLAN.portTP1,MONCLAN.monToken,MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET) {
		logClientCOL3(info,"testConnexionServeur", 
			"le clan[%s] a validé son test de connexion  %b ",
			MONCLAN.nomDuClan,debug_ok); 
	} 	else {
		logClientCOL3(error,"testConnexionServeur", 
			"le clan[%s] n'a pas validé son test de connexion  %b ",
			MONCLAN.nomDuClan,debug_nok); 
	}

	//concatenate MSG_SITE + MSG_DELIMITER  + MSG_QUEST

	char* msg = malloc(strlen(MSG_SITE) + strlen(MSG_DELIMITER) + strlen(MSG_QUEST) + 1);
	strcpy(msg, MSG_SITE);
	strcat(msg, MSG_DELIMITER);
	strcat(msg, MSG_QUEST);

	//send message
	if(envoiMessageCOL3_s(socket, msg) == -1) {
		logClientCOL3(error,"testConnexionServeur", 
			"le clan[%s] n'a pas pu envoyer %b ",
			MONCLAN.nomDuClan,debug_nok);
	}else {
		logClientCOL3(info,"testConnexionServeur", 
			"le clan[%s] a envoyé %b ",
			MONCLAN.nomDuClan,debug_ok);
	}

	//receive message
	if(lireStructureCOL3_s(socket, &MONCLAN.mesCapacites, sizeof(MONCLAN.mesCapacites))== -1){
		logClientCOL3(error,"testConnexionServeur", 
			"le clan[%s] n'a pas pu lire %b ",
			MONCLAN.nomDuClan,debug_nok);
	}else {
		logClientCOL3(info,"testConnexionServeur", 
			"le clan[%s] a lu %b ",
			MONCLAN.nomDuClan,debug_ok);
	}

	afficheCapaciteDuClan(MONCLAN.mesCapacites);
	close(socket);
}


/* ===================================
     fonction de recupération de MP 
       (a completer)
  ==================================*/
		   
void gestionAppro()
{

 printf("\n  *** au boulot ... ****\n");
	
}


/*  ======================================
	  fonction de test d'échange initiale 
      avec le serveur
    =====================================*/ 

int testConnexionServeur() {	
	int socket;
	int testOK = 0;

	logClientCOL3(info,"testConnexionServeur", 
		"le clan[%s] crée une socket pour tester le serveur",
		MONCLAN.nomDuClan);
	
	/* creation et connexion au serveur de socket ( on met MSG_TEST à la place du TOKEN ) */
	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3,MONCLAN.portTP1,MSG_TEST,MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET) {
		logClientCOL3(info,"testConnexionServeur", 
			"le clan[%s] a validé son test de connexion  %b ",
			MONCLAN.nomDuClan,debug_ok); 
		close(socket);
		testOK=1;
	} 	else {
		logClientCOL3(error,"testConnexionServeur", 
			"le clan[%s] n'a pas validé son test de connexion  %b ",
			MONCLAN.nomDuClan,debug_nok); 
	}
	return testOK;
}
	
void afficheMesInfos()
{
  logClientCOL3(info,"afficheMesInfos", 
		"Nom du Clan   = %s",
		MONCLAN.nomDuClan);
  logClientCOL3(info,"afficheMesInfos", 
		"Token du Clan = %s",
		MONCLAN.monToken);
  logClientCOL3(info,"afficheMesInfos", 
		"Serveur COL3  = %s | port TP1 = %d | port TP2 = %d | port TP3 = %d",
		MONCLAN.adresseSrvCol3,MONCLAN.portTP1,MONCLAN.portTP2,MONCLAN.portTP3);
}

/**
 * fonction qui calcul si le menu est valide
 * 
 */
int estMenuValide(char menu[])
{
  menu[1]='\0'; /* on veut juste le premier caractere */
  int ret = (strcmp(menu,"1")==0) || (strcmp(menu,"2")==0) ||
				(strcmp(menu,"3")==0) || (strcmp(menu,"4")==0) ||
				(strcmp(menu,"5")==0) ||
				(strcmp(menu,"q")==0) ;
  return ret;
}

/**
 * procedure affichant le menu
 * 
 */
void afficheMenuClient() {
	char clavier[10];

	do {
		do {
			printf("\n\n============= menu COL3 ====================\n");
			printf("1 - Afficher les informations de mon clan\n");
			printf("2 - Tester la communication avec le serveur\n");
			printf("3 - Récupérer la capacité d'extraction de mon clan\n");
			printf("4 - Envoyer mes chariots et remplir ma hutte\n");
 			printf("5 - Afficher ma hutte\n");
			printf("Q - quitter\n\n");
			printf("Choix ? ");
			fgets(clavier, 10, stdin);	
			printf("============================================\n\n");
		} while (!estMenuValide(clavier));

	if (strcmp(clavier,"1")==0) afficheMesInfos();
	else if (strcmp(clavier,"2")==0) 
	  testConnexionServeur();
	else if (strcmp(clavier,"3")==0) 
	  recupSiteExtraction (MONCLAN.adresseSrvCol3,
	                       MONCLAN.portTP1,
	                       MONCLAN.monToken,
	                       MONCLAN.nomDuClan);
	else if (strcmp(clavier,"4")==0) 
	  {
	   recupSiteExtraction (MONCLAN.adresseSrvCol3,
	                       MONCLAN.portTP1,
	                       MONCLAN.monToken,
	                       MONCLAN.nomDuClan);
	   gestionAppro(MONCLAN.adresseSrvCol3,
	                MONCLAN.portTP1,
	                MONCLAN.monToken,
	                MONCLAN.nomDuClan,5);
	}
	else if (strcmp(clavier,"5")==0) 
	   afficheHutte(MONCLAN.maHutte);

	} while (!(strcmp(clavier,"q")==0));

    printf("Bye-bye %s !\n\n",MONCLAN.nomDuClan);

}
