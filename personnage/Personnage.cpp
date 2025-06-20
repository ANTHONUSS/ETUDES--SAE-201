#include "Personnage.h"

Personnage::Personnage(){}

Personnage::Personnage(const QString &n, const QString &c):nom(n),chemin(c){
    std::cout<<"\t[+]Personnage"<<std::endl;
}
Personnage::~Personnage(){
    std::cout<<"\t[-]Personnage"<<std::endl;
}

const QString &Personnage::getNom() const {return nom;}
void Personnage::setNom(const QString &n) {nom = n;}
const QString &Personnage::getChemin() const {return chemin;}
void Personnage::setChemin(const QString &c) {chemin = c;}
