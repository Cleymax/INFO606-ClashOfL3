

/*************************
   Clash of L3 - 2020
    (ne pas modifier)
**************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <float.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h> 
#include "communCOL3-TP.h"


/* ====================================
       paramètres généreaux à adapter
   ====================================*/ 



/* quantité de materiaux pour fabrique les balistes BLO et BLE 
    - dimension 1 : type de baliste 
 	- dimension 2 : type de matérieux */ 
const int MATERIAUX_BALISTE[2][6]=
						{{5,2,1,1,1,2},
						 {9,3,3,2,3,2}};

const int DEGAT_BALISTE[]={8,12};
const int PRECISION_BALISTE[]={8,4};
const int PVIE_BALISTE[]={6,10};

const int CAPACITE_MAX_HUTTE[6] ={40,20,16,20,30,24 };  /* capacité maximale d'une hutte */

/* ===========================
       pour le TP1 
   ===========================*/ 

/*  --- les délimiteurs pour les split de chaine --- */
const char MSG_DELIMITER[]=":";
const char MSG_DELIMITERSITE[]="-";

/*  --- les codes des messages envoyés au serveur --- */
const char MSG_SITE[]="ST";
const char MSG_ECLAIREUR[]="EC";
const char MSG_CLAN[]="CL";
const char MSG_NOMCLAN[]="?";
const char MSG_LONGITUDE[]="LO";
const char MSG_LATITUDE[]="LA";
const char MSG_DUREE[]="TS";
const char MSG_QUANTITE[]="QT";
const char MSG_MATIERE[]="MA";
const char MSG_HUTTE[]="HU";
const char MSG_NOMSITE[]="NS";
const char MSG_CHARIOT[]="CH";
const char MSG_CAMP[]="CP";
const char MSG_QUEST[]="?";
const char MSG_QUEST_GUERRE[]="?G";
const char MSG_QUEST_FEU[]="?F";
const char MSG_TEST[]="TE";
const char MSG_GTK[]="GT";
const char MSG_CMD[]="CD";

/*  --- les codes de retour ---*/
const char MSG_STOP[]="SP";			  /* indique l'arret de la connexion */
const char MSG_TOKEN_OK[]="TOK";	  /* le token du clan est OK */	
const char MSG_TOKEN_NOK[]="TNOK";	  /* le token du clan n'est pas OK */
const char MSG_NOMCLAN_OK[]="COK";	  /* le nom du clan est OK */
const char MSG_CHARIOT_OK[]="ROK";    /* le message du chariot est OK */ 
const char MSG_CHARIOT_NOK[]="RNOK";  /* le message du chariot n est pas OK */ 
const char MSG_GTK_MONDE_OK[]="MOK";
const char MSG_GTK_BATAILLE_OK[]="BOK";
const char MSG_GTK_ETAT_OK[]="AOK"; 
const char MSG_TEST_OK[]="EOK";
const char MSG_TEST_NOK[]="ENOK";
const char MSG_HUTTE_OK[]="HOK";
const char MSG_HUTTE_NOK[]="HNOK";
const char MSG_CMD_OK[]="DOK";
const char MSG_CMD_NOK[]="DNOK";

/* ===========================
       pour le TP2 et TP3 
   ===========================*/ 

const char MSG_DELIMITERARMEE[]=",";
const char MSG_FICHIER_ARMEE[]="FA";
const char MSG_FICHIER_ARMEE_BALISTE[]="BA";
const char MSG_STATUT_ARMEE[]="SA";
const char MSG_STATUT_ARMEE_ENFAB[]="En cours";
const char MSG_STATUT_ARMEE_PRET[]= "Pret    ";

const char MSG_STATUT_BATAILLE_ENATTENTE[]="En attente";
const char MSG_STATUT_BATAILLE_ENCOURS[]=  "En cours  ";
const char MSG_STATUT_BATAILLE_FINI[]=     "Finie     ";

const char MSG_IDCMP[]="IC";
const char MSG_NBCHA[]="NC";
const char MSG_LSSIT[]="LS";
const char MSG_DLSSIT[]="-";

const char MSG_CMD_EN_BATAILLE[]="SRTBAT";
const char MSG_CMD_QT_BATAILLE[]="QTEBAT";
const char MSG_CMD_RS_BATAILLE[]="RSTBAT";


/**
    cette fonction crée affiche le log sur le meme principe que printf

    entrées : 
     - fn = fonction appellante
     - src = chaine à afficher avec les formats
                %d pour entier
 				%s pour string
 				%b pour le code debug
 				%f pour un double
 				%X pour un long
     - ..., = variable à afficher
 
     p.pernelle - 2021
*/
size_t displayInteger( int value ) {
	if ( value < 0 ) {
        putchar( 0x2d );
		putchar( 0x30 + (-value) );
        return 2;
    }
    if ( value < 10 ) {
        putchar( 0x30 + value );
        return 1;
    }
    size_t size = displayInteger( value / 10);
    putchar( 0x30 + value % 10 );
    return 1 + size;
}
size_t displayULong( unsigned long value) {
    if ( value < 10 ) {
        putchar( 0x30 + value );
        return 1;
    }
    size_t size = displayULong( value / 10);
    putchar( 0x30 + value % 10 );
    return 1 + size;
}

void  logClientCOL3(niveau_log niv,const char * fn, const char * src, ...)
{
	va_list args;
	va_start(args, src); 

	size_t writtenCharacters = 0;
	int integerValue;
	long unsigned longValue;
	debug_log debugValue;
	double doubleValue;
	char currentChar;
	const char * string;	
	char * tempString;
	char tempString2[]="[xxxxxx] ";


	if ((NIVEAULOG!=none) && 
	    ((NIVEAULOG==full && (niv==full || niv==debug || niv==info || niv==error)) ||
	     (NIVEAULOG==debug && (niv==debug || niv==info || niv==error)) ||
	     (NIVEAULOG==info  && (niv==info || niv==error)) ||
	     (NIVEAULOG==error && niv==error)
	    )
	   )
	{

		switch(niv)
		{
			case full :  strcpy(tempString2,"[debug+] ");break;
			case debug : strcpy(tempString2,"[debug]  ");break;
			case info : strcpy(tempString2, "[info]   ");break;
			case error : strcpy(tempString2,"[error]  ");break;
			default : break;
		}
		
		printf("%s %s %25s %s",tempString2, "fn[",fn,"] | ");
	
		while ((currentChar = *src) != '\0') 
		{ 
			src++;
			if ( currentChar != '%' ) {
				putchar( currentChar );
				writtenCharacters++;
				continue;
			}
			switch( *src ++ ) {
				case 'd':
					{
						integerValue = (int) va_arg( args, int );
						writtenCharacters+= displayInteger(integerValue);
					}
					break;
				case 'X':
					{
						longValue = (long unsigned) va_arg( args, long unsigned );
						writtenCharacters += displayULong( longValue );
					}
					break;	
				case 'f':
					{
						doubleValue = (double) va_arg( args, double );
						int leftPart = (int) doubleValue;
						writtenCharacters += displayInteger( leftPart );
						doubleValue -= leftPart;

						putchar( '.' );
						writtenCharacters ++;

						while( ( doubleValue - (int) doubleValue ) > 10e-6 ) {
							doubleValue *= 10;
						}
						writtenCharacters += displayInteger( (int) doubleValue );
					}
					break;
				case 's':
					{
						string = (const char *) va_arg( args, const char * );
						while( *string != '\0' ) {
							putchar( *string ++ );
							writtenCharacters ++;
						}
					}
					break;
				case 'b':
					{
						debugValue = (debug_log) va_arg( args, debug_log);
						tempString = (char *) malloc(10*sizeof(char));
						if (debugValue==debug_ok) {
							strcpy(tempString,"[OK]");
						}
						else {
							strcpy(tempString,"[NOK]");
						}
						while( *tempString != '\0' ) {
							putchar( *tempString ++ );
							writtenCharacters++;
						}
						//free(tempString);
					}
					break;
				default:
					fprintf( stderr, "Specified format is not supported!" );
					abort();
			}
		}
		printf("%s"," \n");
	}
	
	va_end(args);	
}




/* ===================================================
       les fonctions de connexions avec le serveur
   ===================================================*/ 

/**
    cette fonction crée une socket et tente une connexion TCP à un serveur COL3. 
    si la connexion TCP est validée, la fonction echange avec le serveur pour valider
    le tocken du client 

    entrées : 
     - adresse IP du serveur à connecter
     - port du serveur à connecter
     - token id client 

    retour:
     - valeur de la socket créé (meme code que socket())
 
     p.pernelle - 2021
*/
int connexionServeurCOL3(const char *adresse_serveur,int port, const char *token, const char *nomduclan) {

	int sock;
	int nbrecu = 0;
 
	char msgrecu[TAILLE_MAX_MSG];

	
    static struct addrinfo addrHint = {.ai_socktype=SOCK_STREAM, .ai_protocol=0, .ai_family=AF_UNSPEC, .ai_flags=(AI_ADDRCONFIG|AI_V4MAPPED)};
    struct addrinfo* addrInfo;
    char sport[10];  /* port en chaine */
    int e;

	/* resolution adresse destination */
    sprintf(sport,"%d",port);
    if ((e = getaddrinfo(adresse_serveur, sport, &addrHint, &addrInfo))==1){
        if (errno != 0) 
			logClientCOL3(error,"impossible de résoudre le host [%s] port [%s] %b",
		              adresse_serveur, sport,(int)debug_nok);
        else logClientCOL3(error,"ereur getaddrinfo [%s] %b",
		              gai_strerror(e),(int)debug_nok); 
        exit(EXIT_FAILURE);
    }
    if (addrInfo == NULL){
		logClientCOL3(error,"impossible de résoudre le host [%s] port [%s] %b",
		              adresse_serveur, sport,(int)debug_nok);
        exit(EXIT_FAILURE);
    }
	/* récupération de l'adresse de résolution */
    struct sockaddr addr = *(addrInfo->ai_addr);

    /* creation socket */
    sock = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);

    /* nettoyage */
    freeaddrinfo(addrInfo);
	
    /* creation socket en mode connecté */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		logClientCOL3(error,"connexionServeurCOL3",
		              "creation de la socket                 | %b ",(int)debug_nok);
		sock=SOCKET_ERROR;
	}
	else
	{
		logClientCOL3(full,"connexionServeurCOL3",
	                   "creation de la socket                 | %b | socket= %d",debug_ok,sock);
		/* affectation de l'adresse */
		//struct sockaddr_in sin;
		//sin.sin_addr.s_addr = inet_addr(adresse_serveur);
		//sin.sin_family = AF_INET;
		//sin.sin_port = htons(port);

		logClientCOL3(full,"connexionServeurCOL3",
		              "configuration adresse                 | %b | port=%d | adresse IP=%s ",
		              debug_ok,port,adresse_serveur);

		/* lancement de la demande de connexion */
		if( connect(sock, &addr, sizeof(struct sockaddr)) == SOCKET_ERROR){
			logClientCOL3(error,"connexionServeurCOL3",
			              "connection TCP au serveur COL3        | %b ",(int)debug_nok);

			sock=SOCKET_ERROR;
		} 
		else 
		{	
			logClientCOL3(full,"connexionServeurCOL3",
			              "connection TCP au serveur COL3        | %b",debug_ok);

			
			if (envoiMessageCOL3_s(sock, token)  < 0) {
				 logClientCOL3(error,"connexionServeurCOL3",
			              		"envoi token au serveur COL3           | %b",debug_nok);
				 sock=SOCKET_ERROR;
			}
			else 
			{
				logClientCOL3(full,"connexionServeurCOL3",
			              		"envoi token au serveur COL3           | %b",debug_ok);

				/* attente en reception d'un message emis par le serveur */
				if((nbrecu = lireMessageCOL3_s(sock, msgrecu)) <= 0) {
				   logClientCOL3(error,"connexionServeurCOL3",
					          		"reponse au token du serveur COL3      | %b",debug_nok);
				   sock=SOCKET_ERROR;
				} 
				else 
				{
					/* test si le message recu est MSG_TOKEN_OK */
					if (strcmp(msgrecu,MSG_TOKEN_OK)==0)
					{
						logClientCOL3(full,"connexionServeurCOL3",
					          		"reponse au token du serveur COL3      | %b | code retour = %s ",
						          debug_ok,msgrecu);
						strcpy(msgrecu,MSG_CLAN);
						strcat(msgrecu,MSG_DELIMITER);
						strcat(msgrecu,nomduclan);
						if (envoiMessageCOL3_s(sock,msgrecu)  < 0) {
							logClientCOL3(error,"connexionServeurCOL3",
									"envoi du nom du clan au serveur COL3  | %b | clan = %s",debug_nok,nomduclan);
							sock=SOCKET_ERROR;
						}
						else
						{
							if((nbrecu = lireMessageCOL3_s(sock, msgrecu)) <= 0) {
				   				logClientCOL3(error,"connexionServeurCOL3",
					          		"reponse au nom du clan du serveur COL3| %b |",debug_nok);
								sock=SOCKET_ERROR;
							} 
							else
							{
								if (strcmp(msgrecu,MSG_NOMCLAN_OK)==0)
								{
									logClientCOL3(debug,"connexionServeurCOL3",
					          		"reponse au nom du clan du serveur COL3| %b | code retour = %s ",
						        	  debug_ok,msgrecu);
								}
								else
								{
									logClientCOL3(error,"connexionServeurCOL3",
									"reponse au nom du clan du serveur COL3| %b | code retour = %s ",
									 debug_nok,msgrecu);
									sock=SOCKET_ERROR;
								}
							}
						}
						
					}
					else
					{
						logClientCOL3(error,"connexionServeurCOL3",
					          		"reponse au token du serveur COL3      | %b | code retour = %s ",
						          debug_nok,msgrecu);
						sock=SOCKET_ERROR;
					}
					
				}
			}
		}
	}
	
	return sock;
}





/* ======================================================
       les fonctions d'envoi de données vers le serveur
   ======================================================*/ 

/**
    cette fonction envoi un message (msgresponse) par la socket (sock)

	entrées : 
     - sock : numéro de la socket
     - msgenvoye : chaine à envoyé
 
    retour : 
     retourne -1 en cas d'echec
     retourne 1 en cas de succes
  
    p.pernelle / s.talbot / d.wayntal - 2021
 */ 
int envoiMessageCOL3_s(int sock, const char * msgresponse) {
	struct sockaddr_in addresIPClient ;
	socklen_t longAdresseIPClient= sizeof(addresIPClient);
	int ret =-1 ;

	if (getpeername(sock,(struct sockaddr *) &addresIPClient,&longAdresseIPClient) == -1)
	{
		logClientCOL3(error,"envoiMessageCOL3_s",
					   "socket %d non connectée |  envoi %b",sock,debug_nok);
	}
	else
	{
		if (send(sock, msgresponse, strlen(msgresponse), 0) < 0) {
			logClientCOL3(full,"envoiMessageCOL3_s",
					   "envoi %b | socket = %d |  msg = %s",debug_nok,sock,msgresponse);
			
		}
		else {
			logClientCOL3(full,"envoiMessageCOL3_s",
					   "envoi %b | socket = %d  vers IP = %s |  msg = %s",
			            debug_ok,sock,inet_ntoa(addresIPClient.sin_addr),msgresponse);
			ret=1;
		}
	}
	return ret;
}





/**
   cette fonction envoi une structure (mastructure) par la socket (sock) 
   de façon securisée
  
 	 entrées : 
     - sock : numéro de la socket
     - mastructure : structure envoye
  
    retour : 
     retourne -1 en cas d'echec
     retourne 1 en cas de succes
  
    p.pernelle / s.talbot / d.wayntal - 2021
*/ 
int envoiStructureCOL3_s(int sock, void * mastructure,int taille) {

	struct sockaddr_in addresIPClient ;
	socklen_t longAdresseIPClient= sizeof(addresIPClient);
	int ret =-1 ;

	if (getpeername(sock,(struct sockaddr *) &addresIPClient,&longAdresseIPClient) == -1)
	{
		logClientCOL3(error,"envoiStructureCOL3_s",
					   "envoi structure impossible            | %b | socket %d non connectée ",debug_nok,sock); 
	}
	else
	{
		if (send(sock, (char *) mastructure, taille, 0) < 0) {
			logClientCOL3(full,"envoiStructureCOL3_s",
					   "envoi structure incomplete            | %b | socket = %d  ",debug_nok,sock);
		}
		else {
			logClientCOL3(full,"envoiStructureCOL3_s",
					   "envoi structure complete              | %b | socket = %d  vers IP = %s |  taille = %d",
			            debug_ok,sock,inet_ntoa(addresIPClient.sin_addr),taille);
			ret=1;
		}
	}
	return ret;
}




/* ===========================================================
       les fonctions de lecture de données depuis le serveur
   ===========================================================*/ 

/**
    cette fonction lit un message texte en TCP (msgrecu) sur la socket (sock)
	(version securisé)
	
	entrées : 
     - sock : numéro de la socket
     - msgrecu : chaine recue
 
     p.pernelle / s.talbot / d.wayntal - 2021
*/
int lireMessageCOL3_s(int sock, char * msgrecu) {

	struct sockaddr_in addresIPClient ;
	socklen_t longAdresseIPClient= sizeof(addresIPClient);
	int ret =-1 ;
	int nbrecu = 0;
    char temp[TAILLE_MAX_MSG];


	if (getpeername(sock,(struct sockaddr *) &addresIPClient,&longAdresseIPClient) == -1)
	{
		logClientCOL3(error,"lireMessageCOL3_s",
					   "socket %d non connectée |  lecture %b",sock,debug_nok);
	}
	else
	{
		/* attente en reception d'un message emis par le serveur */
		if((nbrecu = recv(sock, temp, sizeof(temp), 0)) <= 0) {
			logClientCOL3(full,"lireMessageCOL3_s",
					   "socket %d connectée |  reception message %b | nbrecu = %d ",
			              sock,debug_nok,nbrecu);
		    strcpy(msgrecu,"");
		} 
		else 
		{
			temp[nbrecu] = '\0';
			strcpy(msgrecu,temp);			
			logClientCOL3(full,"lireMessageCOL3_s",
					   "socket %d connectée |  reception message %b | nbrecu = %d  | msg = {%s}",
			              sock,debug_ok,nbrecu,msgrecu);
			ret =1;
		}
	}
	return ret;
}





/**
    cette fonction lit une structure (mastructure) sur la socket (sock) et
    retourne le nb octet recu (version securisé)
	
	entrées : 
     - sock : numéro de la socket
     - mastructure : structure recue
	 - mastructure : taille de la structure recue
 
      p.pernelle / s.talbot / d.wayntal - 2021
*/
int lireStructureCOL3_s(int sock, void * mastructure, int taille) {

	struct sockaddr_in addresIPClient ;
	socklen_t longAdresseIPClient= sizeof(addresIPClient);

    int nbrecu = 0, nbtotal=0 ;

	if (getpeername(sock,(struct sockaddr *) &addresIPClient,&longAdresseIPClient) == -1)
	{
		logClientCOL3(error,"lireStructureCOL3_s",
					   "lecture d une structure               | %b | socket %d non connectée ",debug_nok,sock);
		nbrecu=-1;
	}
	else
	{
		logClientCOL3(full,"lireStructureCOL3_s",
					   "lecture d une structure encours       | attendu taille = %d ...",taille);
		while (nbtotal<taille && (nbrecu != -1))
			{
				nbrecu = recv(sock, ((char *) mastructure)+nbtotal, taille, 0);
				if (nbrecu<= 0) {
					logClientCOL3(full,"lireStructureCOL3_s",
					   "lecture d une structure icomplete     | %b | nbrecu = %d \n",debug_nok,nbrecu);
					nbrecu=-1;
				}
				else nbtotal+=nbrecu;
			}
		if (nbrecu == -1) 
		    logClientCOL3(error,"lireStructureCOL3_s",
					   "lecture d une structure terminee      | %b | socket %d non connectée ",debug_nok,sock);
		if (nbrecu != -1) 
		    logClientCOL3(full,"lireStructureCOL3_s",
					   "lecture d une structure terminee      | %b | nbrecu = %d ",debug_ok,nbrecu);
	}

    return nbrecu;
}




/* ===================================================
       les fonctions de conversion
   ===================================================*/ 

/**
    cette fonction convertit un message en strucure de type hutte 

    p.pernelle / d.wayntal - 2019
*/
int messageToHutte(char * msg, hutte * mahutte){

    char** splitmessage;
    int ret=1;
    int i;

    splitmessage=split(msg,MSG_DELIMITER,1);

    for(i=1;splitmessage[i]!=NULL;i=i+4) {
        mahutte->stock[(matieres_premieres)atoi(splitmessage[i])]= atoi(splitmessage[i+2]);
		logClientCOL3(full,"messageToHutte",
		  "hutte genere %b | msg  = %s | hutte[%d] mat=%d quantite=%d \n",
		   debug_nok,msg,i,atoi(splitmessage[i]),atoi(splitmessage[i+2]));
		free(splitmessage[i-1]);
        free(splitmessage[i]);
        free(splitmessage[i+1]);
		free(splitmessage[i+2]);
    }
    free(splitmessage);
    return ret;
}


/**
    cette fonction convertit un message en strucure de type 
	site extraction

    p.pernelle / d.wayntal - 2019
*/
int messageToSiteExtraction(char * msg, site_extraction * site){

    char** splitmessage;
    int ret=1;
    int i;

    splitmessage=split(msg,MSG_DELIMITER,1);

    for(i=0;splitmessage[i]!=NULL;i++) {

        if (strcmp(splitmessage[i],MSG_SITE)==0)
        {
			//printf("%s %s %s %d %s","### message2site ### , msg=",splitmessage[i], "id=",atoi(splitmessage[i+1])," \n");
			site->idSite=atoi(splitmessage[i+1]);
            
        }
		if (strcmp(splitmessage[i],MSG_NOMSITE)==0)
        {
            strcpy(site->nomSite,splitmessage[i+1]);
        }
        if (strcmp(splitmessage[i],MSG_LONGITUDE)==0)
        {
            site->longitude=atoi(splitmessage[i+1]);
        }
        if (strcmp(splitmessage[i],MSG_LATITUDE)==0)
        {
            site->latitude=atoi(splitmessage[i+1]);
        }
        if (strcmp(splitmessage[i],MSG_DUREE)==0)
        {
            site->duree=atoi(splitmessage[i+1]);
        }
        if (strcmp(splitmessage[i],MSG_QUANTITE)==0)
        {
            site->quantite=atoi(splitmessage[i+1]);
        }
        if (strcmp(splitmessage[i],MSG_MATIERE)==0)
        {
            site->matiere=(matieres_premieres)atoi(splitmessage[i+1]);
        }

        free(splitmessage[i]);
    }
    free(splitmessage);
    return ret;
}





/**
    cette fonction convertit une strucure de type hutte en un message

    p.pernelle / d.wayntal - 2018
*/
void hutteToMessage(hutte * mahutte,  char * msg) {

  char tmp2[10];
  int i;

  strcpy(msg,MSG_MATIERE);
  strcat(msg,MSG_DELIMITER);
  sprintf(tmp2,"%d",(int)bois);
  strcat(msg,tmp2);
  strcat(msg,MSG_DELIMITER);
  strcat(msg,MSG_QUANTITE);
  strcat(msg,MSG_DELIMITER);
  sprintf(tmp2,"%d",mahutte->stock[(int)bois]);
  strcat(msg,tmp2);

  for (i=1;i<6;i++){
    strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_MATIERE);
	strcat(msg,MSG_DELIMITER);
    sprintf(tmp2,"%d",i);
    strcat(msg,tmp2);
    strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_QUANTITE);
	strcat(msg,MSG_DELIMITER);
    sprintf(tmp2,"%d",mahutte->stock[i]);
    strcat(msg,tmp2);
  }
  logClientCOL3(full,"hutteToMessage",
					   "message genere %b | msg  = %s \n",debug_nok,msg);
}


/**
    cette fonction convertit une strucure de type site extraction
	en un message

    p.pernelle / d.wayntal - 2019
*/
void SiteExtractionToMessage(site_extraction * site,  char * msg, const char * nomClan) {
    char temp[20];
	
	strcpy(msg,MSG_SITE);
	strcat(msg,MSG_DELIMITER);			
	sprintf(temp,"%d",site->idSite);
	strcat(msg,temp);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_NOMSITE);
	strcat(msg,site->nomSite);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_CLAN);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,nomClan);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_LONGITUDE);
	strcat(msg,MSG_DELIMITER);
	sprintf(temp,"%d",site->longitude);
	strcat(msg,temp);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_LATITUDE);
	strcat(msg,MSG_DELIMITER);
	sprintf(temp,"%d",site->latitude);
	strcat(msg,temp);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_DUREE);
	strcat(msg,MSG_DELIMITER);
	sprintf(temp,"%d",site->duree);
	strcat(msg,temp);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_QUANTITE);
	strcat(msg,MSG_DELIMITER);
	sprintf(temp,"%d",site->quantite);
	strcat(msg,temp);
	strcat(msg,MSG_DELIMITER);
	strcat(msg,MSG_MATIERE);
	strcat(msg,MSG_DELIMITER);
	sprintf(temp,"%d",(int)site->matiere);
	strcat(msg,temp);
}



void matiereToString(matieres_premieres mat, char matiereEnChaine[])
{
	switch(mat)
		{
			case  bois     : strcpy(matiereEnChaine,"bois    "); break;
			case  salpetre : strcpy(matiereEnChaine,"salpetre"); break;
			case  charbon  : strcpy(matiereEnChaine,"charbon "); break;
			case  soufre   : strcpy(matiereEnChaine,"soufre  "); break;
			case  fer      : strcpy(matiereEnChaine,"fer     "); break;
			case  chanvre  : strcpy(matiereEnChaine,"chanvre "); break;
			default        : strcpy(matiereEnChaine,"        "); break;
		}
}

/**
    fonction de conversion type_serveurCOL3 --> string

    p.pernelle 2023
*/
char* typeServeurCOL3toString(type_serveurCOL3 tp) {
    char * ret ;
    ret= malloc(10*sizeof(char));
    switch (tp)
    {
        case tp1 : strcpy(ret,"TP1"); break;
        case tp2 : strcpy(ret,"TP2"); break;
        case tp3 : strcpy(ret,"TP3"); break;
		case tpgtk : strcpy(ret,"GTK"); break;		
		case tpbat : strcpy(ret,"BAT"); break;
	 	case tpcmd : strcpy(ret,"CMD"); break;
        default : strcpy(ret,"");
    }
    return ret;
}


/* ===================================================
       les fonctions d'affichage
   ===================================================*/ 

/**
    cette procedure affiche la hutte d'un clan
 
    p.pernelle / d.wayntal - 2019
*/
void afficheHutte(hutte mahutte) {
    int i;
    char temp[30];

	logClientCOL3(info,"afficheHutte", "[info]   --- Contenu de la Hutte du clan [%s] ---  ",mahutte.nomClanHutte);
	for(i=0;i<6;i++)
	{
		matiereToString(i,temp);
		logClientCOL3(info,"afficheHutte", "[info]     Matiere [%s] : %d Kg  (MAX=%d kg)",temp,mahutte.stock[i],CAPACITE_MAX_HUTTE[i]);		
	}
	strftime(temp, 20, "%d/%m/%Y %H:%M:%S", localtime(&mahutte.tps_debut));
	logClientCOL3(info,"afficheHutte","[info]   Temps debut = %s",temp);
	strftime(temp, 20, "%d/%m/%Y %H:%M:%S", localtime(&mahutte.tps_fin));
	logClientCOL3(info,"afficheHutte","[info]   Temps debut = %s",temp);
	
}

/**
    cette procedure affiche la capacité d'extraction d'un clan clan

    p.pernelle / d.wayntal - 2019
*/
void afficheCapaciteDuClan(capacite_clan capa) {
	int i;
	char temp[30];

	logClientCOL3(info,"afficheCapaciteDuClan","[info]   --- Capacite d'extration du clan [%s] ---  ",capa.nomClan);
	logClientCOL3(info,"afficheCapaciteDuClan","[info]   Id Clan = %d",capa.idClan); 
	logClientCOL3(info,"afficheCapaciteDuClan","[info]   Nb Chariot disponible = %d",capa.nbChariotDisponible); 
    for (i=0;i<MAX_SITE_EXTRACTION;i++)
        {
		  matiereToString(capa.sitesAccessibles[i].matiere,temp);
          if(capa.sitesAccessibles[i].idSite != 0) 
				logClientCOL3(info,"afficheCapaciteDuClan","[info]   site[%d] : nom = %s \t ID=%d long. = %d lat. = %d \t %s : Q. = %d kg  \t Tps. = %d s",
				              i,capa.sitesAccessibles[i].nomSite,capa.sitesAccessibles[i].idSite,
				              capa.sitesAccessibles[i].longitude, capa.sitesAccessibles[i].latitude,temp, 
				              capa.sitesAccessibles[i].quantite,capa.sitesAccessibles[i].duree);
        }

}

/*
    cette procedure affiche l'armee d'un clan c'est à dire
 	l'ensemble de ses balistes

    p.pernelle / d.wayntal - 2020
*/
void afficheArmee(armee monarmee) {
    int i;

	printf("============[ Armée du clan ]  NB Baliste = %2d ============\n",monarmee.nbbaliste);
    if (monarmee.nbbaliste==0) {
        printf("  armee vide : aucune baliste fabriquee \n");
    }
    else
    {
        for(i=0;i<monarmee.nbbaliste;i++)
        {
            printf("    - Balise [%2d] = %s  fabriqué par la forge %d \n ",i,
                   monarmee.baliste[i] == BLE ? "BLE" : "BLO", 
                   monarmee.forge[i]);
        }
    }
	printf("===========================================================\n");
}



/**
    cette fonction test si le message recu est du type Demande ?

    p.pernelle / d.wayntal - 2019
*/
int estDemandeConforme(type_demande demande, char * msg){

    char** splitmessage;
    char demandeatester[255];
    int ret=0;
    int i,nbelement=0;

	//printf("%s %d %s","--> test (estDemandeConforme) demande  =", (int) demande,"\n");
	strcpy(demandeatester,"");
    splitmessage=split(msg,MSG_DELIMITER,1);
	for(i=0;splitmessage[i]!=NULL;i++) 
	//{
	//	printf("%s %s %s","--> test (estDemandeConforme) split =", splitmessage[i],"\n");
	//}
	nbelement=i+1;
	//printf("%s %d %s","--> test (estDemandeConforme) demande  =", (int) demande,"\n");
    switch (demande)
    {
		case req_hutte : strcpy(demandeatester,MSG_HUTTE); break;
		case req_test : strcpy(demandeatester,MSG_TEST); break;
	    case req_cmd : strcpy(demandeatester,MSG_CMD); break;
	    case req_gtk : strcpy(demandeatester,MSG_GTK); break;
		case req_nom : strcpy(demandeatester,MSG_CLAN); break;
        case req_statutarmee : strcpy(demandeatester,MSG_STATUT_ARMEE); break;
        case req_fichierarmee : strcpy(demandeatester,MSG_FICHIER_ARMEE); break;
		case req_chariot : strcpy(demandeatester,MSG_CHARIOT); break;
		case req_site : strcpy(demandeatester,MSG_SITE); break;
        case req_vuebat : strcpy(demandeatester,MSG_GTK_BATAILLE_OK); break;
		case req_vuemonde : strcpy(demandeatester,MSG_GTK_MONDE_OK); break;
		case req_vueetat : strcpy(demandeatester,MSG_GTK_ETAT_OK); break;
        default : strcpy(demandeatester,"[NOK]");
    }

    for(i=0;splitmessage[i]!=NULL;i++) {
        if (( (strcmp(splitmessage[i],demandeatester)==0) && (i+1<nbelement) && 
              (strcmp(splitmessage[i+1],MSG_QUEST)==0 || strcmp(splitmessage[i+1],MSG_QUEST_FEU)==0 || strcmp(splitmessage[i+1],MSG_QUEST_GUERRE)==0))
            || ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_vuebat) )
            || ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_vuemonde) )
			|| ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_vueetat) )
			|| ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_hutte) )
			|| ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_test) )
			|| ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_cmd) )
			|| ( (strcmp(splitmessage[i],demandeatester)==0) && (demande==req_gtk) )
			)
            {
                ret=1;
            }
        free(splitmessage[i]);
    }
    free(splitmessage);

	return ret;

}



/**
    cette fonction test si le message recu est du type reponse à demande 

    p.pernelle / d.wayntal / Talbot 2021
*/
int estReponseConforme(type_demande demande, char * valeurReponse, char * msg) {
    char** splitmessage;
    int ret=0;
    int i;
    char demandeatester[20];
    int nbelement=0;

    //printf("--> lance le split pour tester le nom de clan recu ...\n");
    splitmessage=split(msg,MSG_DELIMITER,1);

	for(i=0;splitmessage[i]!=NULL;i++) 
	nbelement=i+1;
    
    switch (demande)
    {
        case req_nom : strcpy(demandeatester,MSG_CLAN); break;
        case req_statutarmee : strcpy(demandeatester,MSG_STATUT_ARMEE); break;
        case req_fichierarmee : strcpy(demandeatester,MSG_FICHIER_ARMEE); break;
		case req_site : strcpy(demandeatester,MSG_SITE); break;
		case req_chariot : strcpy(demandeatester,MSG_CHARIOT); break;
        default : strcpy(demandeatester,"[NOK]");
    }
    //printf("%s %s %s","--> test (estReponseConforme) demande à tester =", demandeatester,"\n");

    for(i=0;splitmessage[i]!=NULL;i++) {
        //printf("%s %s","\n -----SPLIT=",splitmessage[i]);
        if ( (strcmp(splitmessage[i],demandeatester)==0) && (i+1<nbelement) && (strcmp(splitmessage[i+1],MSG_QUEST)!=0) )
            {
                ret = 1;

                //nomclan=(char *)malloc(TAILLE_MAX_NOM_CLAN);
                strcpy(valeurReponse,splitmessage[i+1]);
                //printf("%s %s %s","--> test (estReponseConforme) reponse =", valeurReponse,"\n");
            }
        free(splitmessage[i]);
    }
    free(splitmessage);

    return ret;

}



/* ===================================================
       les fonctions génériques de traitement de chaine
   ===================================================*/ 

/** source : www.code-source.com
    Retour tableau des chaines recupérer. Terminé par NULL.
    chaine : chaine à splitter
    delim : delimiteur qui sert à la decoupe
    vide : 0 : on n'accepte pas les chaines vides
           1 : on accepte les chaines vides
*/
char** split(char* chaine,const char* delim,int vide) {

    char** tab=NULL;                    //tableau de chaine, tableau resultat
    char *ptr;                     //pointeur sur une partie de
    int sizeStr;                   //taille de la chaine à recupérer
    int sizeTab=0;                 //taille du tableau de chaine
    char* largestring;             //chaine à traiter

    int sizeDelim=strlen(delim);   //taille du delimiteur

    largestring = chaine;          //comme ca on ne modifie pas le pointeur d'origine
                                   //(faut ke je verifie si c bien nécessaire)


    while( (ptr=strstr(largestring, delim))!=NULL ){
           sizeStr=ptr-largestring;

           //si la chaine trouvé n'est pas vide ou si on accepte les chaine vide
           if(vide==1 || sizeStr!=0){
               //on alloue une case en plus au tableau de chaines
               sizeTab++;
               tab= (char**) realloc(tab,sizeof(char*)*sizeTab);

               //on alloue la chaine du tableau
               tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
               strncpy(tab[sizeTab-1],largestring,sizeStr);
               tab[sizeTab-1][sizeStr]='\0';
           }

           //on decale le pointeur largestring  pour continuer la boucle apres le premier elément traiter
           ptr=ptr+sizeDelim;
           largestring=ptr;
    }

    //si la chaine n'est pas vide, on recupere le dernier "morceau"
    if(strlen(largestring)!=0){
           sizeStr=strlen(largestring); 
           sizeTab++;
           tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
           tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
           strncpy(tab[sizeTab-1],largestring,sizeStr);
           tab[sizeTab-1][sizeStr]='\0';
    }
    else if(vide==1){ //si on fini sur un delimiteur et si on accepte les mots vides,on ajoute un mot vide
           sizeTab++;
           tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
           tab[sizeTab-1]=(char*) malloc( sizeof(char)*1 );
           tab[sizeTab-1][0]='\0';

    }

    //on ajoute une case à null pour finir le tableau
    sizeTab++;
    tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
    tab[sizeTab-1]=NULL;

        //on ajoute une case à null pour finir le tableau
    sizeTab++;
    tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
    tab[sizeTab-1]=NULL;

    return tab;
}




