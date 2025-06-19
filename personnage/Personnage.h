#ifndef SAE201_PERSONNAGE_H
#define SAE201_PERSONNAGE_H

#include <QVector>
#include <QString>

class Personnage {
    QString nom;
    QString chemin;
public:
    Personnage();
    Personnage(QString n, QString c);
    ~Personnage();

    const QString &getNom() const;

    void setNom(const QString &nom);

    const QString &getChemin() const;

    void setChemin(const QString &chemin);
};


#endif //SAE201_PERSONNAGE_H
