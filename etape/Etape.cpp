//
// Created by p2401552 on 18/06/2025.
//

#include "Etape.h"

//constructeur
Etape::Etape(const QString &titre, float latitude, float longitude, const QString &texte, int reponse)
        : titre(titre),
          latitude(latitude),
          longitude(longitude),
          texte(texte),
          reponse(reponse) {}

Etape::Etape() {}

Etape::Etape(const Etape &e)
        : titre(e.titre),
          latitude(e.latitude),
          longitude(e.longitude),
          texte(e.texte),
          reponse(e.reponse) {}

Etape::~Etape() {
    std::cout<<"\t[-]etape"<<std::endl;
}
