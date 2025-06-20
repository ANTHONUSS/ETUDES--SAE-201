//
// Created by p2401552 on 18/06/2025.
//
#include "Parcours.h"


//constructeur
Parcours::Parcours() {
}

Parcours::Parcours(const QString& nom, const QString& ville, int departement, unsigned int difficulte, float duree,
                   float kilometre, const QString& image, const QString& entete)
	: nom(nom), ville(ville),
	  departement(departement),
	  difficulte(difficulte),
	  duree(duree),
	  kilometre(kilometre),
	  image(image),
	  entete(entete)
{
	std::cout << "\t[+]parcours" << std::endl;
}

Parcours::Parcours(const Parcours& p)
	: nom(p.nom),
	  ville(p.ville),
	  departement(p.departement),
	  difficulte(p.difficulte),
	  duree(p.duree),
	  kilometre(p.kilometre),
	  image(p.image),
	  entete(p.entete)
{
	std::cout << "\t[+C]parcours" << std::endl;
}


//desctructeur
Parcours::~Parcours() {
	for (Etape* e : etapes) {
		delete e;
	}

	etapes.clear();
	std::cout << "\t[-]parcours" << std::endl;
}
void Parcours::addEtape(const QString &titre, const QString &dialog, int reponse,
		int latD, float latM, QString NS,
		int lonD, float lonM, QString WE) {
	etapes.push_back(new Etape(titre, dialog, reponse,
		latD, latM, NS,
		lonD, lonM, WE));
}

void Parcours::addEtape(int i, const QString &titre, const QString &dialog, int reponse,
		int latD, float latM, QString NS,
		int lonD, float lonM, QString WE) {
	etapes.insert(i,new Etape(titre, dialog, reponse,
		latD, latM, NS,
		lonD, lonM, WE));
}

std::ostream& operator<<(std::ostream& os, const Parcours& p) {
	os << "Parcours: " << p.nom.toStdString()
	   << "\n  Ville: " << p.ville.toStdString()
	   << "\n  Departement: " << p.departement
	   << "\n  Difficulte: " << p.difficulte
	   << "\n  Duree: " << p.duree
	   << "\n  Kilometre: " << p.kilometre
	   << "\n  Image: " << p.image.toStdString()
	   << "\n  Entete: \n" << p.entete.toStdString();

	for (Etape* e : p.etapes) {
	    os << "\n  " << *e;
	}

	return os;
}

void Parcours::supprimerEtape(int index) {
	etapes.remove(index);
}

void Parcours::modifierParcours(const QString& nom, const QString& ville, int departement, unsigned int difficulte,
	float duree, float kilometre, const QString& image, const QString& entete, Etape* etape, int etapeIndex) {
	this->nom = nom;
	this->ville = ville;
	this->departement = departement;
	this->difficulte = difficulte;
	this->duree = duree;
	this->kilometre = kilometre;
	this->image = image;
	this->entete = entete;
	if (etapeIndex >= 0 && etapeIndex < etapes.size()) {
		etapes[etapeIndex] = etape; // Remplace l'étape à l'index spécifié
	} else {
		std::cerr << "Index d'étape invalide pour la modification du parcours." << std::endl;
	}
}
