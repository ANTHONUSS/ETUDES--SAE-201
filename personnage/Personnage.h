#ifndef SAE201_PERSONNAGE_H
#define SAE201_PERSONNAGE_H

#include <iostream>
#include <QVector>
#include <QString>

class Personnage {
    QString nom;
    QString chemin;
public:
    Personnage();
    Personnage(const QString &n, const QString &c);
    ~Personnage();

    const QString &getNom() const;
    void setNom(const QString &n);
    const QString &getChemin() const;
    void setChemin(const QString &c);



};


#endif //SAE201_PERSONNAGE_H
