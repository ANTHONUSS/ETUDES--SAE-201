#ifndef SAE201_ETAPE_H
#define SAE201_ETAPE_H

#include <iostream>
#include <QString>


class Etape {
    QString titre;
    float latitude;
    float longitude;
    QString texte;
    int reponse;
public:
    Etape();
    Etape(const QString &titre, float latitude, float longitude, const QString &texte, int reponse);
    Etape(const Etape &e);

    const QString &getTitre() const;
    float getLatitude() const;
    float getLongitude() const;
    const QString &getTexte() const;
    int getReponse() const;
    void setLatitude(int d, float m, float s);
    void setLongitude(float d, float m, float s);

    ~Etape();
};


#endif //SAE201_ETAPE_H
