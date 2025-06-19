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
#include <iostream>
#include "../parcours/Parcours.h"

#include "../etape/Etape.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow {
Q_OBJECT

	Ui::Notepad *ui;
	QString currentFilePath;
	QVector<Parcours*> parcoursList;

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() override;

    void addParcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image, const QString &dialog);

    QString getDialog(QTextStream& in) const;

	void afficherEtape(int index);
	void afficherParcours(int index);

private slots:
    void newDocument();
    void open();
    void save();
    void saveAs();
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
};


#endif //NOTEPAD_H
