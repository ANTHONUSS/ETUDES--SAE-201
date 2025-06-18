//
// Created by ANTHONUS on 11/06/2025.
//

#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QPrinter>
#include <iostream>
#include "../parcours/Parcours.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow {
Q_OBJECT

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() override;

    void addParcours(const QString &nom, const QString &ville, int departement, unsigned int difficulte, float duree,
             float kilometre, const QString &image, const QString &dialog);

    QString getDialog(QTextStream& in) const;

private slots:
    void newDocument();
    void open();
    void save();
    void saveAs();
    void exportPDF();
    void insertImage();

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



private:
    Ui::Notepad *ui;

    QString currentFilePath;

    QVector<Parcours*> parcoursList;
};


#endif //NOTEPAD_H
