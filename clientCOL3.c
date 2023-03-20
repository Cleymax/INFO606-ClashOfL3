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

	logClientCOL3(info, "testConnexionServeur",
				  "le clan[%s] crée une socket pour tester le serveur",
				  MONCLAN.nomDuClan);

	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3, MONCLAN.portTP1, MONCLAN.monToken, MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET)
	{
		logClientCOL3(info, "testConnexionServeur",
					  "le clan[%s] a validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}
	else
	{
		logClientCOL3(error, "testConnexionServeur",
					  "le clan[%s] n'a pas validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}

	// concatenate MSG_SITE + MSG_DELIMITER  + MSG_QUEST

	char *msg = malloc(strlen(MSG_SITE) + strlen(MSG_DELIMITER) + strlen(MSG_QUEST) + 1);
	strcpy(msg, MSG_SITE);
	strcat(msg, MSG_DELIMITER);
	strcat(msg, MSG_QUEST);

	// send message
	if (envoiMessageCOL3_s(socket, msg) == -1)
	{
		logClientCOL3(error, "testConnexionServeur",
					  "le clan[%s] n'a pas pu envoyer %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}
	else
	{
		logClientCOL3(info, "testConnexionServeur",
					  "le clan[%s] a envoyé %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}

	// receive message
	if (lireStructureCOL3_s(socket, &MONCLAN.mesCapacites, sizeof(MONCLAN.mesCapacites)) == -1)
	{
		logClientCOL3(error, "testConnexionServeur",
					  "le clan[%s] n'a pas pu lire %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}
	else
	{
		logClientCOL3(info, "testConnexionServeur",
					  "le clan[%s] a lu %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}

	afficheCapaciteDuClan(MONCLAN.mesCapacites);
	close(socket);
}

/* ===================================
	 fonction de recupération de MP
	   (a completer)
  ==================================*/


// struct params
struct params
{
	char site[10];
	int socket;
};
// fonction gestionChariot

void gestionChariot(void* arg)
{
	struct params p = *(struct params *)(arg);
	char* site = p.site;

	// print site
	logClientCOL3(info, "gestionChariot", "site = %s", site);
	

	int socket;
	logClientCOL3(info, "gestionChariot",
				  "le clan[%s] crée une socket pour tester le serveur",
				  MONCLAN.nomDuClan);

	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3, MONCLAN.portTP1, MONCLAN.monToken, MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET)
	{
		logClientCOL3(info, "gestionAppro",
					  "le clan[%s] a validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}
	else
	{
		logClientCOL3(error, "gestionAppro",
					  "le clan[%s] n'a pas validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}

	// concat MSG_CHARIOT + MSG_DELIMITER + MSG_QUEST

	char *msg = malloc(strlen(MSG_CHARIOT) + strlen(MSG_DELIMITER) + strlen(MSG_QUEST) + 1);
	strcpy(msg, MSG_CHARIOT);
	strcat(msg, MSG_DELIMITER);
	strcat(msg, MSG_QUEST);

	// send message
	if (envoiMessageCOL3_s(socket, msg) == -1)
	{
		logClientCOL3(error, "gestionAppro",
					  "le clan[%s] n'a pas pu envoyer %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}
	else
	{
		logClientCOL3(info, "gestionAppro",
					  "le clan[%s] a envoyé %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}

	char msgrecu[TAILLE_MAX_MSG];

	lireMessageCOL3_s(socket, msgrecu);

	logClientCOL3(info, "gestionAppro2",
				  "le clan[%s] a reçu  en brut ==> '%s' %b ",
				  MONCLAN.nomDuClan,&msgrecu, debug_ok);

	if(strcmp(msgrecu, "ROK") == 0){
		logClientCOL3(info, "gestionAppro",
					  "le clan[%s] a reçu MSG_CHARIOT_OK %b ",
					  MONCLAN.nomDuClan, debug_ok);

		logClientCOL3(info, "gestionAppo", "envoie de MSG_CHARIOT + MSG_DELIMITER + idSite[%s]", site);
		char msg[TAILLE_MAX_MSG];
		strcpy(msg, MSG_CHARIOT);
		strcat(msg, MSG_DELIMITER);
		strcat(msg, site);

		// print msg
		logClientCOL3(info, "gestionAppro",
					  "le clan[%s] envoie %s %b ",
					  MONCLAN.nomDuClan, msg, debug_ok);
		

		if(envoiMessageCOL3_s(socket, &msg)== -1){
				logClientCOL3(error, "gestionAppro2",
					  "le clan[%s] n'a pas pu envoyer %b ",
					  MONCLAN.nomDuClan, debug_nok);
		}else {

			strcat(msgrecu, "");
			int r = lireMessageCOL3_s(socket, msgrecu);
			//print r
			logClientCOL3(info, "gestionAppro2",
						"le clan[%s] lit message return %d %b ",
						MONCLAN.nomDuClan,r, debug_ok);

			logClientCOL3(info, "gestionAppro2",
						"le clan[%s] a reçu apres tout: %s %b ",
						MONCLAN.nomDuClan,&msgrecu, debug_ok);

			if(strcmp(msgrecu, MSG_STOP) == 0){
				logClientCOL3(error, "gestionAppro2",
					  "le clan[%s] a reçu MSG_STOP %b ",
					  MONCLAN.nomDuClan, debug_nok);
			}else {
				// tout good
				logClientCOL3(info, "gestionAppro2",
					  "le clan[%s] a reçu %s %b ",
					  MONCLAN.nomDuClan,&msgrecu, debug_ok);

				int m,qt;
				sscanf(&msgrecu, "MA:%d:QT:%d", &m, &qt);

				// char token;
				// int matiere, quantite;
				// token = strtok(&msgrecu, ":");
				// token = strtok(NULL, ":");
				// matiere = atoi(token);

				// token = strtok(NULL, ":");
				// token = strtok(NULL, ":");
				// quantite = atoi(token);

				printf("matiere: %d\n", m);
				printf("quantite: %d\n", qt);

				MONCLAN.maHutte.stock[m] += qt;
			}
					
		}

	}else {
		logClientCOL3(error, "gestionAppro2",
					  "le clan[%s] a recu MSG_STOP %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}
	pthread_exit(NULL);
}

void launch(pthread_t threadChariot[], struct params parameters[]){
	int i;
	for (i = 0; i < MONCLAN.mesCapacites.nbChariotDisponible; i++)
	{
		if(pthread_create(&threadChariot[i], NULL, gestionChariot, (void*) &parameters[i]) == 0){
			logClientCOL3(info, "gestionAppro", "le clan[%s] a créé le thread %d", MONCLAN.nomDuClan, i);
		}else {
			logClientCOL3(error, "gestionAppro", "le clan[%s] n'a pas créé le thread %d", MONCLAN.nomDuClan, i);
		}
	}

	for(i = 0; i < 1; i++){
		pthread_join(threadChariot[i], NULL);
	}
}


void gestionAppro()
{

	printf("\n  *** au boulot ... ****\n");

	MONCLAN.maHutte.tps_debut = time(NULL);

	// gérer les approvisionnements, c'est à dire que vous envoyez autant de chariots que vous avez le droit avec un thread par chariot

	int socket;

	logClientCOL3(info, "gestionAppro",
				  "le clan[%s] crée une socket pour tester le serveur",
				  MONCLAN.nomDuClan);

	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3, MONCLAN.portTP1, MONCLAN.monToken, MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET)
	{
		logClientCOL3(info, "gestionAppro",
					  "le clan[%s] a validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_ok);
	}
	else
	{
		logClientCOL3(error, "gestionAppro",
					  "le clan[%s] n'a pas validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}

	logClientCOL3(info, "gestionAppro", "le clan[%s] va envoyer %d chariots au serveur", MONCLAN.nomDuClan, MONCLAN.mesCapacites.nbChariotDisponible);

	int i = 0;
	pthread_t threadChariot[MONCLAN.mesCapacites.nbChariotDisponible];


	

	int k = 0;
	for(k = 0; k < 5; k++){
		
		char site[3];
		printf("site: ");
		scanf("%s", site);

		struct params parameters[MONCLAN.mesCapacites.nbChariotDisponible];
		parameters[0].socket = socket;
		strcpy(parameters[0].site, site);

		parameters[1].socket = socket;
		strcpy(parameters[1].site, "12");
		launch(threadChariot, parameters);
	}

	// save MONCLAN.mahute.stock in file
	//delete stock.txt if exist
	remove("stock.txt");

	FILE *f = fopen("stock.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	int j;
	for (j = 1; j < 6; j++)
	{
		fprintf(f, "%d:%d\n",j, MONCLAN.maHutte.stock[j]);
	}

	logClientCOL3(info, "gestionAppro", "le clan[%s] a fini de travailler", MONCLAN.nomDuClan);

	fclose(f);

	close(socket);
}

/*  ======================================
	  fonction de test d'échange initiale
	  avec le serveur
	=====================================*/

int testConnexionServeur()
{
	int socket;
	int testOK = 0;

	logClientCOL3(info, "testConnexionServeur",
				  "le clan[%s] crée une socket pour tester le serveur",
				  MONCLAN.nomDuClan);

	/* creation et connexion au serveur de socket ( on met MSG_TEST à la place du TOKEN ) */
	socket = connexionServeurCOL3(MONCLAN.adresseSrvCol3, MONCLAN.portTP1, MSG_TEST, MONCLAN.nomDuClan);

	/* si la socket est valide*/
	if (socket != INVALID_SOCKET)
	{
		logClientCOL3(info, "testConnexionServeur",
					  "le clan[%s] a validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_ok);
		close(socket);
		testOK = 1;
	}
	else
	{
		logClientCOL3(error, "testConnexionServeur",
					  "le clan[%s] n'a pas validé son test de connexion  %b ",
					  MONCLAN.nomDuClan, debug_nok);
	}
	return testOK;
}

void afficheMesInfos()
{
	logClientCOL3(info, "afficheMesInfos",
				  "Nom du Clan   = %s",
				  MONCLAN.nomDuClan);
	logClientCOL3(info, "afficheMesInfos",
				  "Token du Clan = %s",
				  MONCLAN.monToken);
	logClientCOL3(info, "afficheMesInfos",
				  "Serveur COL3  = %s | port TP1 = %d | port TP2 = %d | port TP3 = %d",
				  MONCLAN.adresseSrvCol3, MONCLAN.portTP1, MONCLAN.portTP2, MONCLAN.portTP3);
}

/**
 * fonction qui calcul si le menu est valide
 *
 */
int estMenuValide(char menu[])
{
	menu[1] = '\0'; /* on veut juste le premier caractere */
	int ret = (strcmp(menu, "1") == 0) || (strcmp(menu, "2") == 0) ||
			  (strcmp(menu, "3") == 0) || (strcmp(menu, "4") == 0) ||
			  (strcmp(menu, "5") == 0) ||
			  (strcmp(menu, "q") == 0);
	return ret;
}

/**
 * procedure affichant le menu
 *
 */
void afficheMenuClient()
{
	char clavier[10];

	// read stock.txt
	FILE *f = fopen("stock.txt", "r");
	if (f == NULL)
	{
		printf("Error opening file!\n");
	}else {
		int i;
		for (i = 1; i < 6; i++)
		{
			fscanf(f, "%d:%d\n", &i, &MONCLAN.maHutte.stock[i]);
		}
		fclose(f);
	}


	do
	{
		do
		{
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

		if (strcmp(clavier, "1") == 0)
			afficheMesInfos();
		else if (strcmp(clavier, "2") == 0)
			testConnexionServeur();
		else if (strcmp(clavier, "3") == 0)
			recupSiteExtraction(MONCLAN.adresseSrvCol3,
								MONCLAN.portTP1,
								MONCLAN.monToken,
								MONCLAN.nomDuClan);
		else if (strcmp(clavier, "4") == 0)
		{
			recupSiteExtraction(MONCLAN.adresseSrvCol3,
								MONCLAN.portTP1,
								MONCLAN.monToken,
								MONCLAN.nomDuClan);
			gestionAppro(MONCLAN.adresseSrvCol3,
						 MONCLAN.portTP1,
						 MONCLAN.monToken,
						 MONCLAN.nomDuClan, 5);
		}
		else if (strcmp(clavier, "5") == 0)
			afficheHutte(MONCLAN.maHutte);

	} while (!(strcmp(clavier, "q") == 0));

	printf("Bye-bye %s !\n\n", MONCLAN.nomDuClan);
}
