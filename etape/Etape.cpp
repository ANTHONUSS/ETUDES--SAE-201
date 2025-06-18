//
// Created by p2401552 on 18/06/2025.
//

#include "Etape.h"

//constructeur

Etape::Etape() {}


Etape::Etape(const QString &titre, float latitude, float longitude, const QString &texte, int reponse)
        : titre(titre),
          latitude(latitude),
          longitude(longitude),
          texte(texte),
          reponse(reponse) {}


Etape::Etape(const Etape &e)
        : titre(e.titre),
          latitude(e.latitude),
          longitude(e.longitude),
          texte(e.texte),
          reponse(e.reponse) {}


//getters setters
const QString &Etape::getTitre() const {
    return titre;
}
float Etape::getLatitude() const {
    return latitude;
}
float Etape::getLongitude() const {
    return longitude;
}
const QString &Etape::getTexte() const {
    return texte;
}

int Etape::getReponse() const {
    return reponse;
}


void Etape::setLatitude(int d, float m, float s) {
    latitude = d+(m/60)+(s/3600);
}
void Etape::setLongitude(float d, float m, float s) {
    longitude = d+(m/60)+(s/3600);
}


//méthodes
QString Etape::getCoordonnee(){
    int entier=latitude;
    float degree=entier;
    float minute=latitude-entier*60;
    entier=minute;
    float seconde=minute-entier*60;
    QString texte=std::to_string(degree)+"°"+std::to_string(minute)+"'"+std::to_string(seconde);
    return texte;
}





//desctructeur
Etape::~Etape() {
    std::cout<<"[-]etape"<<std::endl;
}

