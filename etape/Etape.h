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
    Etape(const QString &titre, float latitude, float longitude, const QString &dialog, int reponse);
    Etape(const Etape &e);
    ~Etape();

    void setLatitude(int d, float m, QChar NS);
    void setLongitude(int d, float, QChar EW);

    friend std::ostream &operator<<(std::ostream &os, const Etape &e);

};


#endif //SAE201_ETAPE_H
