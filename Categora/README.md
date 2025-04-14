# Categora

**Serveur backend pour gérer les données d'utilisateurs, ainsi que leurs requêtes**

## Dépendences

Categora utilise seulement le framework Qt, et a été écrit en C++. Ainsi, la seule dépendence requise est Qt, avec les modules additionnels Network, HttpServer et Sql de Qt. La version préféré de Qt est de 6 et plus.

## Build
Pour build Categora, il faut s'assurer que Qt est disponible ainsi que les modules nécessaires.
Ensuite, vous n'avez qu'à utiliser cmake pour build le projet. Le fichier CMakeLists.txt racine est située dans ce dossier même.

## Déploiment
Pour déployer et éxécuter le serveur Categora, il suffit de créer un fichier .env, et déclarer 2 variables d'environment nécéssaire à l'éxécution du programme.

1. DB_PATH est le chemin qui va être utiliser par l'application pour créer la base de données SQlite3 
2. Ensuite il est nécessaire d'ajouter une clef API pour le modèle LLM qui va être utilisé. Voici les options qui ont déjà été intégré:

2.1. DEEPSEEK_API_KEY
2.2. OPENAI_API_KEY
