//
// Created by p2401552 on 18/06/2025.
//
#include <cmath>
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
    float temp=fabsf(latitude);
    int degree=temp;
    int minute=(temp-degree)*60;
    int seconde=((temp-degree)*60-minute)*60;
    QString texte =QString::fromStdString(std::to_string(degree)+ "°"+std::to_string(minute)+ "'"+std::to_string(seconde)+ "''") ;
    if(latitude<0){
        texte+= "S";
    }
    else{
        texte+="N";
    }
    if(longitude<0){
        texte+= "W";
    }
    else{
        texte+="E";
    }

    temp=fabsf(longitude);
    degree=temp;
    minute=(temp-degree)*60;
    seconde=((temp-degree)*60-minute)*60;
    texte +=QString::fromStdString(std::to_string(degree)+ "°"+std::to_string(minute)+ "'"+std::to_string(seconde)+ "''") ;git


    return texte;
}






//desctructeur
Etape::~Etape() {
    std::cout<<"[-]etape"<<std::endl;
}

