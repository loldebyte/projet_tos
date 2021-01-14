# Projet de théorie des systèmes d'exploitations

## Structure

La totalité du code est dans `launcher.c`.

Pour compiler le projet, il suffit d'exécuter `make`.

Pour supprimer les fichiers résiduels (.o etc...) exécuter `make clean`.

Ne supporte pas les chemins relatifs `.` et `..`.

L'objectif de cette branche est de supporter l'exécution asynchrone d'une commande, lorsque l'instruction est terminée par un `&`.
