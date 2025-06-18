#include <QVector>
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

	void addEtape(const QString &titre, float latitude, float longitude, const QString &dialog, int reponse);

	friend std::ostream &operator<<(std::ostream &os, const Parcours &p);

};


#endif //SAE201_PARCOURS_H
