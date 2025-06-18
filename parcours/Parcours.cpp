//
// Created by p2401552 on 18/06/2025.
//
#include "Parcours.h"


//constructeur
Parcours::Parcours() {}

Parcours::Parcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
                   float kilometre, const QString &image)
        : nom(nom), ville(ville),
          departement(departement),
          difficulte(difficulte),
          duree(duree),
          kilometre(kilometre),
          image(image){}

Parcours::~Parcours() {

    for (Etape* e:etapes) {
        delete e;
    }

    etapes.clear();
    std::cout<<"[-] parcours"<<std::endl;
}



