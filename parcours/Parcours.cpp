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


//méthodes
void Parcours::addEtape(const Etape &e) {
    Etape* step=new Etape(e.getTitre(), e.getLatitude() ,e.getLongitude(), e.getTexte(), e.getReponse());
    etapes.push_back(step);
}


//desctructeur
Parcours::~Parcours() {

    for (Etape* e:etapes) {
        delete e;
    }

    etapes.clear();
    std::cout<<"[-] parcours"<<std::endl;
}



