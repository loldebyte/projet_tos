# Projet de théorie des systèmes d'exploitations

## Structure

La totalité du code est dans `launcher.c`.

Pour compiler le projet, il suffit d'exécuter `make`.

Pour supprimer les fichiers résiduels (.o etc...) exécuter `make clean`.

Ne supporte pas les chemins relatifs `.` et `..`.

Attends que l'instruction entrée s'exécute avant de demander une nouvelle commande, sauf quand le dernier argument de cette dernière est un `&`.
