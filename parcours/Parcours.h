#include <QVector>
#include "../etape/Etape.h"

#ifndef SAE201_PARCOURS_H
#define SAE201_PARCOURS_H


class Parcours {
public:
    Parcours();
    Parcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image);

    ~Parcours();

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

};


#endif //SAE201_PARCOURS_H
