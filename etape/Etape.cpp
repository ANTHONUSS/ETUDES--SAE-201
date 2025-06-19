#include <cmath>
#include "Etape.h"

//constructeur
Etape::Etape(const QString &titre, const QString &dialog, int reponse,
        int latD, float latM, QString NS,
        int lonD, float lonM, QString WE)
        : titre(titre),
          dialog(dialog),
          reponse(reponse)
{
    setLatitude(latD, latM, NS);
    setLongitude(lonD, lonM, WE);

    std::cout << "\t[+]Etape" << std::endl;
}

Etape::Etape() {
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

//desctructeur
Etape::~Etape() {
    std::cout<<"\t[-]Etape"<<std::endl;
}

void Etape::setLatitude(int d, float m, QString NS) {
    if(NS=="N"){
        latitude = d+m/60.0;
    }
    if(NS=="S"){
        latitude = (d+m/60.0)*-1;
    }
    std::cout << "Latitude set to: " << latitude << std::endl;
}
void Etape::setLongitude(int d, float m, QString WE) {
    if(WE=="E"){
        longitude = d+m/60.0;
    }
    if(WE=="W"){
        longitude = (d+m/60.0)*-1;
    }
    std::cout << "Longitude set to: " << longitude << std::endl;
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
