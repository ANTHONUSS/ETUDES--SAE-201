#include "Personnage.h"

Personnage::Personnage(){}

Personnage::Personnage(QString n, QString c){
    nom=n;
    chemin=c;
}
Personnage::~Personnage(){}

const QString &Personnage::getNom() const {
    return nom;
}

void Personnage::setNom(const QString &nom) {
    Personnage::nom = nom;
}

const QString &Personnage::getChemin() const {
    return chemin;
}

void Personnage::setChemin(const QString &chemin) {
    Personnage::chemin = chemin;
}
