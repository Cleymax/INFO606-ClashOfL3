/*************************
   Clash of L3 - 2021
    (ne pas modifier)
**************************/

#ifndef COMMUNCOL3_TP_H_INCLUDED
#define COMMUNCOL3_TP_H_INCLUDED

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define TAILLE_MAX 4196

#define MAX_CLAN 70

#define TAILLE_MAX_NOM_CLAN 15
#define TAILLE_MAX_NOM_SITE 15
#define TAILLE_MAX_MSG 1024

#define MAX_SITE_EXTRACTION 25
#define TAILLE_MAX_ARMEE 15

#define MAX_FORGE_BLE 3	/* max de BLE par forge */
#define MAX_FORGE_BLO 2	/* max de BLO par forge */

#define TPS_FAB_BLO 5	/* temps de fabrication des BLO */
#define TPS_FAB_BLE 2	/* temps de fabrication des BLE */

#define MAX_CHARIOT_PAR_CLAN 5	/* max de chariot par clan */

#define MAX_TYPESERVEURCOL3 6
#define MAX_MATIERE 6

#include <netinet/in.h>


/* ===========================
       les types enumérés
   ===========================*/ 


/* type de matiere premiere */
typedef enum {vide=-1, bois=0, salpetre=1, charbon=2, soufre=3, fer=4, chanvre=5} 
		matieres_premieres;

/* type de chariot et capacite */
typedef enum {lourd=12, leger=5} type_chariot;

/* type de balise (BLO= Baliste LOurde, BLE=Baliste LEgere) */
typedef enum {BLO=1,BLE=0} type_baliste;

/* type de demande envoyé entre client et serveur */
typedef enum {req_nom=0, 
             req_statutarmee=1, 
             req_fichierarmee=2, 
             req_chariot=3, 
             req_site=4, 
			 req_vuemonde=5, 
             req_vuebat=6,
             req_vueetat=7,
             req_test=8,
             req_gtk=9,
             req_cmd=10,
             req_hutte=11} 
		type_demande;

/* type indicateur au log si le message est OK ou NOK */
typedef enum {debug_ok,debug_nok} debug_log;

/* niveau d'affichage des log *
   - full : toutes les traces
   - debug : les traces de debuggage
   - info  : les traces d'information 
   - error : les traces d'erreur
   - none  : aucune trace */
typedef enum {full,debug,info,error,none} niveau_log;


typedef enum {notp=-1,tp1=0,tp2=1,tp3=2, tpgtk=3,tpbat=4,tpcmd=5} type_serveurCOL3;     /* type de tp */

/* ====================================
       les constantes défines dans .c
   ====================================*/ 



extern const int DEGAT_BALISTE[2]; 			/* degat des balistes BLE et BLO */
extern const int PRECISION_BALISTE[2]; 		/* précision de tir des balistes BLE et BLO */
extern const int PVIE_BALISTE[2];			/* point de vie des balistes BLE et BLO */


/* ===========================
       pour le TP1 
   ===========================*/ 

/*  --- les délimiteurs pour les split de chaine --- */
extern const char MSG_DELIMITER[];
extern const char MSG_DELIMITERSITE[];

/*  --- les codes des messages envoyé au serveur --- */
extern const char MSG_SITE[];
extern const char MSG_ECLAIREUR[];
extern const char MSG_CLAN[];
extern const char MSG_NOMCLAN[];
extern const char MSG_LONGITUDE[];
extern const char MSG_LATITUDE[];
extern const char MSG_DUREE[];
extern const char MSG_QUANTITE[];
extern const char MSG_MATIERE[];
extern const char MSG_HUTTE[];
extern const char MSG_NOMSITE[];
extern const char MSG_CHARIOT[];
extern const char MSG_CAMP[];
extern const char MSG_QUEST[];
extern const char MSG_QUEST_GUERRE[];
extern const char MSG_QUEST_FEU[];
extern const char MSG_TEST[];
extern const char MSG_GTK[];
extern const char MSG_CMD[];

/*  --- les codes de retour ---*/
extern const char MSG_STOP[];			/* indique l'arret de la connexion */
extern const char MSG_TOKEN_OK[];	    /* le token du clan est OK */	
extern const char MSG_TOKEN_NOK[];	    /* le token du clan n'est pas OK */
extern const char MSG_NOMCLAN_OK[];	    /* le nom du clan est OK */
extern const char MSG_CHARIOT_OK[];     /* le message du chariot est OK */ 
extern const char MSG_CHARIOT_NOK[];    /* le message du chariot n'est pas OK */
extern const char MSG_TEST_OK[];
extern const char MSG_TEST_NOK[];
extern const char MSG_HUTTE_OK[];
extern const char MSG_HUTTE_NOK[];
extern const char MSG_CMD_OK[];
extern const char MSG_CMD_NOK[];



/* ===========================
       pour le TP2 et TP3 
   ===========================*/ 

extern const char MSG_DELIMITERARMEE[];
extern const char MSG_FICHIER_ARMEE[];
extern const char MSG_FICHIER_ARMEE_BALISTE[];
extern const char MSG_STATUT_ARMEE[];
extern const char MSG_STATUT_ARMEE_ENFAB[];
extern const char MSG_STATUT_ARMEE_PRET[];

extern const char MSG_GTK_MONDE_OK[];		/* code de validation MONDE recu par client GTK */
extern const char MSG_GTK_BATAILLE_OK[]; 	/* code de validation BATAILLE recue par client GTK */
extern const char MSG_GTK_ETAT_OK[]; 	    /* code de validation ETAT recue par client GTK */


extern const char MSG_IDCMP[];
extern const char MSG_NBCHA[];
extern const char MSG_LSSIT[];
extern const char MSG_DLSSIT[];

extern const char MSG_CMD_EN_BATAILLE[];
extern const char MSG_CMD_QT_BATAILLE[];
extern const char MSG_CMD_RS_BATAILLE[];


extern const char MSG_STATUT_BATAILLE_ENATTENTE[];
extern const char MSG_STATUT_BATAILLE_ENCOURS[];
extern const char MSG_STATUT_BATAILLE_FINI[];


extern const int MATERIAUX_BALISTE[2][MAX_MATIERE]; 	/* matériaux pour la construction des 
 											   balistes BLE et BLO */

extern const int CAPACITE_MAX_HUTTE[MAX_MATIERE];  /* capacité maximale d'une hutte */


extern int NCURSE ;     		/* indique si on utilise NCURSE */  
extern niveau_log NIVEAULOG;	/* indique le niveau de log */


/* ===========================
       les structures
   ===========================*/ 

/* définition d'un site extraction */
typedef struct {
    char nomSite[TAILLE_MAX_NOM_SITE];
	int idSite;
    int longitude;
    int latitude;
    int quantite;
    matieres_premieres matiere;
    int duree;
} site_extraction;

/* liste des sites d'extractions du monde CoL3   */
typedef site_extraction lessitesdumonde[MAX_SITE_EXTRACTION];

/* structure indiquant la capacité d'extraction d'un clan */
typedef struct {
	int idClan;
	char nomClan[TAILLE_MAX_NOM_CLAN];
    lessitesdumonde sitesAccessibles;
    int nbChariotDisponible;
} capacite_clan;

/* structure d'une hutte de stockage  */
typedef struct {
    char nomClanHutte[TAILLE_MAX_NOM_CLAN];   /* nom du clan */
    int stock[MAX_MATIERE];							  /* quantité par matiere */	
    time_t tps_debut;						  /* date premier remplissage */
    time_t tps_fin;                           /* date fin remplissage */
} hutte;

/* structure d'une armmée composé de baliste  */
typedef struct {
    int forge[TAILLE_MAX_ARMEE];			/* num forge ou est produite la baliste*/
    type_baliste baliste[TAILLE_MAX_ARMEE]; /* type de baliste */
    int pvie[TAILLE_MAX_ARMEE];				/* point de vie de la baliste */
    int nbbaliste;							/* nb de baliste produite */
} armee;

/* structure préparatoire d'une attaque (recu par chaque clan avant attaque) 
   a recevoir par le serveur */
typedef struct {
    int dispoBaliste[2];							     /* nb de baliste disponible BLO et BLE pour l'attaque */
    type_baliste balisterestante[TAILLE_MAX_ARMEE];      /* type de chaque baliste restante pour l'attaque */
    int pvieBaliste[TAILLE_MAX_ARMEE];                   /* point de vie de chaque baliste restante pour l'attaque */
    int nbcl;										     /* nb clan restant */
    int pvieClan;									     /* point de vie du clan */
    char listeClan[MAX_CLAN][TAILLE_MAX_NOM_CLAN] ;     /* liste des nom de clan srestant */
    int listeIdClan[MAX_CLAN];
	int tourEnCours;
} preAttaque;



/* structure d'une attaque : nb baliste et id clan attaqué (-1 pour utiliser toutes
   les balistes en défense ) 
   à envoyer au serveur */
typedef struct {
    int balisteAttaque[2];     /* nb de baliste attaquantes par type (BLO et BLE) */
    int idClan;                /* identifiant du clan attaqué */
} attaque;


/* ===========================
       les vues GTK
   ===========================*/ 



/* Cette structure décrit la vue d'un CLAN */
typedef struct {
	int idClan;
    int longitude;
    int latitude;	
	int pVie;
    char nom[TAILLE_MAX_NOM_CLAN];
	int estConnecte;
	int estInstalle;
	int aPretresseFeu;
	int aPretresseGuerre;
} clanVue;

/* Cette structure décrit la vue du MONDE */
typedef struct {
    int portServ;
    char addrServ[20];
    clanVue clans[MAX_CLAN];
    int nbclan;
} mondeVue;

/* cette structure décrit la vue d'une BATAILLE-*/
typedef struct { 
    int debutCombat;
	int finCombat;
    int numTour;
    int nbClanEnBataille;
	int nbclan;
    attaque lesAttaques[MAX_CLAN];
    armee lesArmees[MAX_CLAN];
	hutte leshuttes[MAX_CLAN];
} batailleVue;

/* cette structure décrit la vue du SERVEUR-*/
typedef struct {
  type_serveurCOL3 tpEnCours[MAX_TYPESERVEURCOL3];
  int nbConnexionActive[MAX_TYPESERVEURCOL3];
  int maxConnexion[MAX_TYPESERVEURCOL3];
  int nbClanParTP[MAX_TYPESERVEURCOL3];
  int nbThreadClientActifParTP[MAX_TYPESERVEURCOL3];
  int etatThreadInterne[MAX_TYPESERVEURCOL3];
} serveurVue;


typedef struct {
  serveurVue leServeurGTK;
  batailleVue laBatailleGTK;
  mondeVue leMondeGTK;
} gtkVue;

/**
    cette fonction crée affiche le log sur le meme principe que printf

    entrées : 
     - fn = fonction appellante
     - src = chaine à afficher avec les formats
                %d pour entier
 				%s pour string
 				%b pour le code debug
 				%f pour un double
 				%X pour un unsigned long
     - ..., = variable à afficher
 
     p.pernelle - 2021
*/
void  logClientCOL3(niveau_log niv,const char * fn, const char * src, ...);
size_t displayInteger(int value);
size_t displayULong(unsigned long value);

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
int connexionServeurCOL3(const char *adresse_serveur,int port, const char *token, const char *nomduclan);


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
int envoiMessageCOL3_s(int sock, const char * msgenvoye);


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
int envoiStructureCOL3_s(int sock, void * mastructure,int taille);


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
int lireMessageCOL3_s(int sock, char * msgrecu);


/**
    cette fonction lit une structure (mastructure) sur la socket (sock) et
    retourne le nb octet recu (version securisé)
	
	entrées : 
     - sock : numéro de la socket
     - mastructure : structure recue
	 - mastructure : taille de la structure recue
 
      p.pernelle / s.talbot / d.wayntal - 2021
*/
int lireStructureCOL3_s(int sock, void * mastructure, int taille);

/* ===================================================
       les fonctions de conversion
   ===================================================*/ 

/**
    cette fonction convertit un message en strucure de type hutte 

    p.pernelle / d.wayntal - 2019
*/
int messageToHutte(char * msg, hutte * mahutte);
/**
    cette fonction convertit un message en strucure de type 
	site extraction

    p.pernelle / d.wayntal - 2019
*/
int messageToSiteExtraction(char * msg, site_extraction * site);
/**
    cette fonction convertit une strucure de type hutte
	en un message

    p.pernelle / d.wayntal - 2019
*/
void hutteToMessage(hutte * mahutte,  char * msg);
/**
    cette fonction convertit une strucure de type site extraction
	en un message

    p.pernelle / d.wayntal - 2019
*/
void SiteExtractionToMessage(site_extraction * site,  char * msg, const char * nomClan);

/**
    fonction de conversion matiere --> string

    p.pernelle 2020
*/
void matiereToString(matieres_premieres mat, char matiereEnChaine[]); 

/**
    fonction de conversion type_serveurCOL3 --> string

    p.pernelle 2023
*/
char* typeServeurCOL3toString(type_serveurCOL3 tp);


/* ===================================================
       les fonctions d'affichage
   ===================================================*/ 

/**
    cette procedure affiche la hutte d'un clan
 
    p.pernelle / d.wayntal - 2019
*/
void afficheHutte(hutte mahutte);
/**
    cette procedure affiche la capacité d'extraction d'un clan clan

    p.pernelle / d.wayntal / talbot - 2021
*/
void afficheCapaciteDuClan(capacite_clan capa) ;

/**
    cette procedure affiche l'armee d'un clan c'est à dire
 	l'ensemble de ses balistes

    p.pernelle / d.wayntal - 2020
*/
void afficheArmee(armee monarmee);

int estDemandeConforme(type_demande demande, char * msg);
int estReponseConforme(type_demande demande, char * valeurReponse, char * msg);

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
char** split(char* chaine,const char* delim,int vide);




#endif // COMMUNCOL3_TP2_H_INCLUDED

