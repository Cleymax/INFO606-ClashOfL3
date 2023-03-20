#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "col3-bibtp/communCOL3-TP.h"
#include "clientCOL3.h"

/* variables globales */

niveau_log NIVEAULOG=info;			// mode debug
int NCURSE =0;						// affichage avec NCURSE

clan_client MONCLAN;				// les données de mon clan
hutte mahutte;

/*  programme principal du client CoL3  */
int main(int argc, char *argv[])
{
	int i=1 ;
	int help=0;
	int test=0;
	int menu=0;
	
	/* --- cette zone est à modifier à l'issue du jeu --- */
	strcpy(MONCLAN.nomDuClan,"LesBranguignol"); 
	char *env = getenv("TOKEN");
	if (env != NULL) {
		strcpy(MONCLAN.monToken,env);
	}

	strcpy(MONCLAN.adresseSrvCol3,"134.214.147.120");
	MONCLAN.portTP1 = 8081;
	MONCLAN.portTP2 = 8082;
	MONCLAN.portTP3 = 8083;
	MONCLAN.maHutte = mahutte;
	strcpy(mahutte.nomClanHutte, MONCLAN.nomDuClan);
    /* --- cette zone est à modifier à l'issue du jeu --- */

	
	/* boucle de traitements des arguments */
    while(i < argc && help==0) {

		/* si demande d'aide */
		if ((0 == strcmp("-h",argv[i])) || (0 == strcmp("--help",argv[i]))){
			help=1;
        } 
		else if (0 == strcmp("--test",argv[i])){
			test=1;
        } 
		else if ((0 == strcmp("-m",argv[i])) || (0 == strcmp("--menu",argv[i]))){
			menu=1;
        }
		else if (0 == strcmp("--full",argv[i])){
			NIVEAULOG=full;
        } 
		else if (0 == strcmp("--debug",argv[i])){
			NIVEAULOG=debug;
        } 
		else if (0 == strcmp("--info",argv[i])){
			NIVEAULOG=info;
        } 
		else if (0 == strcmp("--error",argv[i])){
			NIVEAULOG=error;
        } 
        else if (0 == strcmp("-p1",argv[i])){
			if (argv[i+1]!= NULL) MONCLAN.portTP1  = (int) strtol(argv[i+1], (char **)NULL, 10);
		} 
	    else if (0 == strcmp("-p2",argv[i])){
			if (argv[i+1]!= NULL) MONCLAN.portTP2  = (int) strtol(argv[i+1], (char **)NULL, 10);
		}
	 	else if (0 == strcmp("-p3",argv[i])){
			if (argv[i+1]!= NULL) MONCLAN.portTP3  = (int) strtol(argv[i+1], (char **)NULL, 10);
		}
		else if (0 == strcmp("-n",argv[i])){
            if (argv[i+1]!= NULL)  strcpy(MONCLAN.nomDuClan,argv[i+1]);
        }
		else if (0 == strcmp("-t",argv[i])){
            if (argv[i+1]!= NULL)  strcpy(MONCLAN.monToken,argv[i+1]);
		}
		else if (0 == strcmp("-a",argv[i])){
            if (argv[i+1]!= NULL)  strcpy(MONCLAN.adresseSrvCol3 ,argv[i+1]);
        }		
        i++;
    }

	/* en cas d'aide : affichage des options */
	if ((argc == 1) || (help == 1))
	{
		printf(" Description : pgm client Clash of L3 \n");
		printf(" Aide : [-h | --help] \n");
		printf(" Usage : col3-client-etu [--menu] \n");
		printf("            -a [adresseIP] -p1 [port] -p2 [port] -p3 [port] \n");
		printf("            -n [nomduclan] -t [token]   \n");
		printf("	        [--test]\n");
		printf("	        [--full | --debug | --info | --error] \n");
		printf("  --menu ou -m   : lance le mode avec menu \n");
		printf("  -p1 [port]     : port d'accès du TP1 (valeur par defaut = 8081) \n");
		printf("  -p2 [port]     : port d'accès du TP1 (valeur par defaut = 8082) \n");
	    printf("  -p3 [port]     : port d'accès du TP1 (valeur par defaut = 8083) \n");
		printf("  -a [adresseIP] : adresse IP du serveur  (valeur par defaut = 127.0.0.1) \n");
		printf("  -n [nomduclan] : nom du clan) \n");
		printf("  -t [token]     : token du clan) \n");
		printf("  --test         : lance le test de connexion avec le serveur CoL3\n");
		printf("  --full | --debug | --info | --error : lance le client avec un niveau de log \n\n");
	}
	/* sinon je lance la fonction principale */
	else
	{
		printf("\n  *** Clash Of L3 ****\n");

		printf("\n%s %s", "nom du Clan = ",MONCLAN.nomDuClan);
		printf("\n%s %s", "token du Clan = ",MONCLAN.monToken);
		printf("\n%s %s", "adresse IP du serveur= ",MONCLAN.adresseSrvCol3);
		printf("\n%s %d", "port TP1 du serveur = ",MONCLAN.portTP1);
	 	printf("\n%s %d", "port TP2 du serveur = ",MONCLAN.portTP2);
	 	printf("\n%s %d", "port TP3 du serveur = ",MONCLAN.portTP3);
		printf("\n");

		/* si le menu est activé*/
		if (menu) afficheMenuClient();

		/* sinon c'est le mode batch*/
		else
		{
			if (test) testConnexionServeur();
			printf("\n a finir \n");
		}
	}
		
    return 0;
}







