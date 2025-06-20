# SAE201
Repo pour la SAE201 QT C++

# Build
Pour compiler le projet, il faut la version 6.9.1 de Qt.
Pour préciser le chemin vers QT, il faut copier le fichier `config.user.cmake.template`, le renommer en `config.user.cmake` et modifier le code en choisissant un nom et le chemin vers le compilateur de Qt.
Voici cela à quoi cela ressemble :

```cmake
# Template de configuration utilisateur
set(USER_NAME "ANTHONUSS")

# Chemins spécifiques à l'utilisateur
set(QT_PATH "C:/Qt/6.9.1/mingw_64")
# Autres configurations spécifiques...
```