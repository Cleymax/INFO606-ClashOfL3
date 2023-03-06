# col3-client-etu

Squelette du programme client Clash of L3

# Installation


## Clonage du dépôt

Le projet utilise un sous-module git [^1] [^2] pour la bibliothèque partagée avec le client (sous dossier col3-bibtp).  
Lorsqu'on clone le dépôt git, il faut aussi obtenir la partie qui correspond au sous-module. Cela peut se faire 
soit en utilisant la commande : 
```
git clone --recurse-submodules https://manouchian.univ-smb.fr/syncproc/col3-client-etu.git
```  
soit avec un clonage normal, suivit par une initialisation du sous module :
```
git clone https://manouchian.univ-smb.fr/syncproc/col3-client-etu.git
# initialisation sous-module
git submodule init
# recuperation de sous-module (commit approprie)
git submodule update
```


[^2]: Pro Git book https://git-scm.com/book/fr/v2/Utilitaires-Git-Sous-modules
[^1]: Documentation Git https://git-scm.com/docs/git-submodule



## Construction du projet


### Construction manuelle
C'est un projet 'autotools', donc il suffit d'aller dans le dossier projet et d'utiliser les commandes habituelles:
```
autoreconf -i
./configure
make
```
### Construction avec Anjuta

1. Lancez Anjuta
1. Cliquez sur 'Import a projet'
1. Cliquez sur 'Import from folder' puis sélectionnez le dossier racine du projet
1. Cliquez sur 'Import'
1. Sélectionnez le moteur 'Base Autotools'
1. Une fois le projet importé, cliquez sur 'Build project' puis 'Execute'

