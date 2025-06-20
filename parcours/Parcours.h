#include <QVector>
#include <QFile>
#include "../etape/Etape.h"

#ifndef SAE201_PARCOURS_H
#define SAE201_PARCOURS_H


class Parcours {
	QString nom;
	QString ville;
	unsigned short int departement;
	unsigned short int difficulte;
	float duree;
	float kilometre;
	QString image;
	QString entete;
	QVector<Etape*> etapes;


public:
    Parcours();
    Parcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image, const QString &entete);
	Parcours(const Parcours &p);
    ~Parcours();


	void addEtape(int i, const QString &titre, const QString &dialog, int reponse,
		int latD, float latM, QString NS,
		int lonD, float lonM, QString WE);
	void addEtape(const QString &titre, const QString &dialog, int reponse,
		int latD, float latM, QString NS,
		int lonD, float lonM, QString WE);

	friend std::ostream &operator<<(std::ostream &os, const Parcours &p);

	QString getNom() const { return nom; }
	QString getVille() const { return ville; }
	unsigned short int getDepartement() const { return departement; }
	unsigned short int getDifficulte() const { return difficulte; }
	float getDuree() const { return duree; }
	float getKilometre() const { return kilometre; }
	QString getImage() const { return image; }
	QString getEntete() const { return entete; }
	int getNombreEtapes() const { return etapes.size(); }
	QVector<Etape*> getEtapes() const { return etapes; }
	Etape* getEtape(int index) { return (index >= 0 && index < etapes.size()) ? etapes[index] : nullptr; }
	void supprimerEtape(int index);
	void modifierParcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
			 float kilometre, const QString &image, const QString &entete, Etape* etape, int etapeIndex);
};


#endif //SAE201_PARCOURS_H
