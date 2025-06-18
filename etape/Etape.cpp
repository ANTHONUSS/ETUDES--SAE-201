//
// Created by p2401552 on 18/06/2025.
//

#include "Etape.h"

//constructeur
Etape::Etape() {}

Etape::Etape(const QString &titre, float latitude, float longitude, const QString &dialog, int reponse)
        : titre(titre),
          latitude(latitude),
          longitude(longitude),
          dialog(dialog),
          reponse(reponse)
{
	std::cout << "\t[+]Etape" << std::endl;
}

Etape::Etape(const Etape &e)
        : titre(e.titre),
          latitude(e.latitude),
          longitude(e.longitude),
          dialog(e.dialog),
          reponse(e.reponse)
{
	std::cout << "\t[+C]Etape" << std::endl;
}

Etape::~Etape() {
    std::cout<<"\t[-]etape"<<std::endl;
}

std::ostream& operator<<(std::ostream& os, const Etape& e) {
os << "Etape: " << e.titre.toStdString()
   << "\n  Latitude: " << e.latitude
   << "\n  Longitude: " << e.longitude
   << "\n  Dialog: " << e.dialog.toStdString()
   << "\n  Reponse: " << e.reponse;

	return os;
}
