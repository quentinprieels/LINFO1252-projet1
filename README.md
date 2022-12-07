# linfo1252-projet1
Projet réalisé dans le cadre du cours LINFO1252 - Systèmes Informatiques par Quentin Prieels et Nathan Zeep (mercredi, groupe : 26)

# Index
- [linfo1252-projet1](#linfo1252-projet1)
- [Index](#index)
- [Utilisation](#utilisation)
  - [**Prérequis**](#prérequis)
  - [**Lister les commandes**](#lister-les-commandes)
- [Structure du Projet](#structure-du-projet)
  - [**bins**](#bins)
  - [**inginious**](#inginious)
  - [**measures**](#measures)
  - [**performances**](#performances)
  - [**src**](#src)

# Utilisation

## **Prérequis**
* **make** : pour la compilation du programme
* **gcc** : pour la compilation du programme
* **python3** : pour exécuter les scripts python
* **valgrind** : pour exécuter les commandes de vérification

## **Lister les commandes**
```sh
make help
```
liste les différentes actions possibles à l'aide de make à utiliser en remplacant help par le mot-clé
# Structure du Projet
## **bins**
Ce dossier contient tous les exécutables binaires générés
## **inginious**
Ce dossier contient :
  - des fichiers `.txt` correspondant aux retours des différentes soumissions sur inginious 
  - un script `split.py` permettant de transformer ces fichiers .txt en fichier .csv se trouvant dans le dossier [measures](#measures)
## **measures**
Ce dossier contient toutes les mesures de temps dans des fichiers `.csv`
## **performances**
Ce dossier contient un script bash `time_measures.sh` qui encode en .csv le temps nécessaire pour run un programme et l'enregistre dans [measures](#measures)
ainsi que 3 scripts python :
  - `plot_same.py` : permet d'afficher plusieurs graphiques sur le même plot
  - `plot_simple.py` : permet d'afficher un seul graphique sur le plot
  - `plot_subplot.py`: permet d'afficher plusieurs graphiques sur différents subplots
## **src**
Ce dossier est séparé en deux parties :
- un sous-dossier `headers` contenant les headers
- les différents fichiers `.c` utiles à la résolution des tâches