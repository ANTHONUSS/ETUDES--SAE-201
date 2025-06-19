#ifndef SAE201_ETAPE_H
#define SAE201_ETAPE_H

#include <iostream>
#include <QString>


class Etape {
    QString titre;
    float latitude;
    float longitude;
    QString dialog;
    int reponse;

public:
    Etape();
    Etape(const QString &titre, const QString &dialog, int reponse,
        int latD, float latM, QString NS,
        int lonD, float lonM, QString WE);
    Etape(const Etape &e);
    ~Etape();

    void setLatitude(int d, float m, QString NS);
    void setLongitude(int d, float m, QString WE);

    QString getCoordonnee(bool toFichier = false);

    friend std::ostream &operator<<(std::ostream &os, const Etape &e);

    QString getTitre() const { return titre; }
    float getLatitude() const { return latitude; }
    float getLongitude() const { return longitude; }
    QString getDialog() const { return dialog; }
    int getReponse() const { return reponse; }


};


#endif //SAE201_ETAPE_H