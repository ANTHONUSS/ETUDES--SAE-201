#include <cmath>
#include "Etape.h"

//constructeur
Etape::Etape(const QString &titre, float latitude, float longitude, const QString &dialog, int reponse)
        : titre(titre),
          latitude(latitude),
          longitude(longitude),
          dialog(dialog),
          reponse(reponse) {}

Etape::Etape() {}

Etape::Etape(const Etape &e)
        : titre(e.titre),
          latitude(e.latitude),
          longitude(e.longitude),
          dialog(e.dialog),
          reponse(e.reponse) {}

//desctructeur
Etape::~Etape() {
    std::cout<<"[-]etape"<<std::endl;
}
void Etape::setLatitude(int d, int m, int s, QChar NS) {
    if(NS=='N'){
        latitude = d+(m/60.0)+(s/3600.0);
    }
    if(NS=='S'){
        latitude = (d+(m/60.0)+(s/3600.0))*-1;
    }
}
void Etape::setLongitude(int d, int m, int s, QChar WE) {
    if(WE=='E'){
        longitude = d+(m/60.0)+(s/3600.0);
    }
    if(WE=='W'){
        longitude = (d+(m/60.0)+(s/3600.0))*-1;
    }
}

//méthodes
QString Etape::getCoordonnee(){
    QString texte;
    float temp=fabsf(latitude);
    int degree=temp;
    int minute=(temp-degree)*60;
    int seconde=((temp-degree)*60-minute)*60;

    if(latitude<0){
        texte+= "S";
    }
    else{
        texte+="N";
    }
    texte +=QString::fromStdString(std::to_string(degree)+ "°"+std::to_string(minute)+ "."+std::to_string(seconde)+ "'") ;
    texte+="/";

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
    texte +=QString::fromStdString(std::to_string(degree)+ "°"+std::to_string(minute)+ "."+std::to_string(seconde)+ "'");


    return texte;
}

std::ostream& operator<<(std::ostream& os, const Etape& e) {
    os << "Etape: " << e.titre.toStdString()
       << "\n  Latitude: " << e.latitude
       << "\n  Longitude: " << e.longitude
       << "\n  Dialog: " << e.dialog.toStdString()
       << "\n  Reponse: " << e.reponse;

    return os;
}
