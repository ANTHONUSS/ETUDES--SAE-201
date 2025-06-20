//
// Created by ANTHONUS on 11/06/2025.
//

#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include<QDesktopServices>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QPrinter>
#include <QImageReader>
#include <iostream>

#include "../parcours/Parcours.h"
#include "../etape/Etape.h"
#include "../personnage/Personnage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow {
Q_OBJECT

	Ui::Notepad *ui;
	QString currentFilePath;
	QVector<Parcours*> parcoursList;
    QVector<Personnage*> persoList;

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() override;

    void addParcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image, const QString &dialog);

    QString getDialog(QTextStream& in) const;

	void afficherEtape(int index);
	void afficherParcours(int index);
	void createParcours(int parcoursIndex);
	void createEtape(int parcoursIndex, int etapeIndex);

    bool PersoExiste(const QString &p);
    void initPerso();

private slots:
    void newDocument();
    void open();
    void save();
    void exportPDF();
    void insertImage();
    void exportMap();

    void selectFont();
    void setItalic();
    void setBold();
    void setUnderline();
    void setColor();

    void copy();
    void cut();
    void paste();
    void undo();
    void redo();

    void showAbout();

	void onNumEtapeChanged(int value);
	void onNumParcoursChanged(int value);
	void supprEtape();
	void supprParcours();
	void ajouterEtape();
	void selectionnerImage();

};


#endif //NOTEPAD_H
