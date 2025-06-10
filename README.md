# 🔢 Devine le Nombre – Projet C avec GTK 3

Ce projet est une application graphique développée en C qui permet de jouer au jeu **"Devine le nombre"**. L'interface utilisateur a été créée à l'aide de la bibliothèque **GTK 3**.

## 🧠 Principe du jeu

L'application choisit un nombre aléatoire entre 1 et 10. Le joueur doit deviner ce nombre. Après chaque tentative, l'application indique si le nombre à trouver est plus **grand** ou plus **petit** que celui proposé.  
Il y a la possibilité de personaliser le nombre maximum avec un compteur de tentative quand on trouve le nombre

## 🛠 Technologies utilisées

- Langage : **C**
- Interface graphique : **GTK 3**
- Génération de nombre aléatoire : `rand()` (standard C)
- Outils de compilation : `gcc`, `pkg-config`

## 🚀 Lancer l'application

### Prérequis

Assurez-vous d’avoir installé GTK 3 et les outils de compilation :

```bash
sudo apt update
sudo apt install libgtk-3-dev build-essential
