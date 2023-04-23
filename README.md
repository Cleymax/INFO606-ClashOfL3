# INFO606_INFO - Synchronisation des processus

## Description

Binome:
- Clement PERRIN (L3 - TRI)
- Jérémy DANGREAUX (L3 - TRI)

## Element non traité

- TP2
- TP3


## Construction du projet

### IDE

Nous avons utilisé l'IDE Visual Studio Code, avec l'extension C/C++ de Microsoft.


### Construction manuelle

C'est un projet 'autotools', donc il suffit d'aller dans le dossier projet et d'utiliser les commandes habituelles:
```
autoreconf -i
./configure
make
```
## Utilisation

### Exécution

Pour exécuter le programme, il suffit d'utiliser la commande suivante:
```bash
export TOKEN="yourtoken"
./col3_client_etu -m

# debug mode
./col3_client_etu -m --debug
```
