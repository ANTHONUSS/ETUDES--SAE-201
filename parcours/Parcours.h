#include <QVector>
#include <QFile>
#include "../etape/Etape.h"

#ifndef SAE201_PARCOURS_H
#define SAE201_PARCOURS_H


class Parcours {

private:
    QString nom;
    QString ville;
    int departement;
    unsigned int difficulte;
    float duree;
    float kilometre;
    QString image;
    QVector<Etape*> etapes;


public:
    Parcours();
    Parcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image);

    void addEtape(const Etape &e);
    ~Parcours();


};


#endif //SAE201_PARCOURS_H
