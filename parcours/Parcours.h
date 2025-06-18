#include <QVector>
#include "../etape/Etape.h"

#ifndef SAE201_PARCOURS_H
#define SAE201_PARCOURS_H


class Parcours {
    QString nom;
    QString ville;
    int departement;
    unsigned int difficulte;
    float duree;
    float kilometre;
    QString image;
    QVector<Etape>;
};


#endif //SAE201_PARCOURS_H
