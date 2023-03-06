# Installation

## clonage du dépôt 

Pour cloner le dépôt depuis un terminal
```
git clone https://manouchian.univ-smb.fr/syncproc/col3-bibtp.git

```
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
