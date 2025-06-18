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

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() override;

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



private:
    Ui::Notepad *ui;

    QString currentFilePath;
};


#endif //NOTEPAD_H
