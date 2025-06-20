#include <cmath>
#include "Etape.h"

//constructeur
Etape::Etape():	titre(""), dialog(""), reponse(0), latitude(0.0f), longitude(0.0f) {
	std::cout << "\t[+]Etape" << std::endl;
}

Etape::Etape(const QString& titre, const QString& dialog, int reponse,
             int latD, float latM, QString NS,
             int lonD, float lonM, QString WE)
	: titre(titre),
	  dialog(dialog),
	  reponse(reponse) {
	setLatitude(latD, latM, NS);
	setLongitude(lonD, lonM, WE);

	std::cout << "\t[+]Etape" << std::endl;
}

Etape::Etape(const Etape& e)
	: titre(e.titre),
	  latitude(e.latitude),
	  longitude(e.longitude),
	  dialog(e.dialog),
	  reponse(e.reponse) {
	std::cout << "\t[+C]Etape" << std::endl;
}

//desctructeur
Etape::~Etape() {
	std::cout << "\t[-]Etape" << std::endl;
}

void Etape::setLatitude(int d, float m, QString NS) {
	if (NS == "N") {
		latitude = d + m / 60.0;
	}
	if (NS == "S") {
		latitude = (d + m / 60.0) * -1;
	}
	std::cout << "Latitude set to: " << latitude << std::endl;
}

void Etape::setLongitude(int d, float m, QString WE) {
	if (WE == "E") {
		longitude = d + m / 60.0;
	}
	if (WE == "W") {
		longitude = (d + m / 60.0) * -1;
	}
	std::cout << "Longitude set to: " << longitude << std::endl;
}

//méthodes
QString Etape::getCoordonnee(bool toFichier) {
	QChar hemiLat = (latitude < 0) ? 'S' : 'N';
	QChar hemiLon = (longitude < 0) ? 'W' : 'E';

	double absLat = qAbs(latitude);
	double absLon = qAbs(longitude);

	int degLat = (int)absLat;
	int degLon = (int)absLon;

	double minLat = (absLat - degLat) * 60.0;
	double minLon = (absLon - degLon) * 60.0;

	QString texte;

	if (toFichier) {
		QString strLat = QString("%1 %2 %3")
							 .arg(hemiLat)
							 .arg(degLat)
							 .arg(QString::number(minLat, 'f', 3));
		QString strLon = QString("%1 %2 %3")
						 .arg(hemiLon)
						 .arg(degLon)
						 .arg(QString::number(minLon, 'f', 3));

		texte = QString("%1 %2").arg(strLat, strLon);

	} else {
		QString strLat = QString("%1%2°%3'")
						 .arg(hemiLat)
						 .arg(degLat, 3, 10, QChar('0')) //ptite explication : on met degLat à la place de %2, de taille de 3 minimum, on remplace par le "char 0" à la fin si jamais notre degLat est inférieur à 10, et c'est en base 10
						 .arg(QString::number(minLat, 'f', 3));
		QString strLon = QString("%1%2°%3'")
						 .arg(hemiLon)
						 .arg(degLon, 3, 10, QChar('0'))
						 .arg(QString::number(minLon, 'f', 3));

		texte = QString("%1%2").arg(strLat, strLon);

	}

	return texte;
}

std::ostream& operator<<(std::ostream& os, const Etape& e) {
	os << "Etape: " << e.titre.toStdString()
		<< "\n  Latitude: " << e.latitude
		<< "\n  Longitude: " << e.longitude
		<< "\n  Dialog: " << e.dialog.toStdString()
		<< "\n  Reponse: " << e.reponse;

	return os;
}
