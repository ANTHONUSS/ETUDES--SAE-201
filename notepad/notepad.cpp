//
// Created by ANTHONUS on 11/06/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Notepad.h" resolved

#include "notepad.h"
#include "ui_Notepad.h"


Notepad::Notepad(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::Notepad)
{
    ui->setupUi(this);

    // Connecter le bouton "ui->BOUTON" quand il est trigerred à la classe "this", et ensuite à la méthode (qui est définie dans les slots du .h)
    connect(ui->actionNouveau, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOuvrir, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionEnregistrer, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionEnregistrer_sous, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionQuitter, &QAction::triggered, this, &QWidget::close);

    connect(ui->actionPolice, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionItalique, &QAction::triggered, this, &Notepad::setItalic);
    connect(ui->actionGras, &QAction::triggered, this, &Notepad::setBold);
    connect(ui->actionSouligner, &QAction::triggered, this, &Notepad::setUnderline);
    connect(ui->actionCouleur, &QAction::triggered, this, &Notepad::setColor);

    connect(ui->actionCopier, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCouper, &QAction::triggered, this, &Notepad::cut);
    connect(ui->actionColler, &QAction::triggered, this, &Notepad::paste);
    connect(ui->actionAnnuler, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRetablir, &QAction::triggered, this, &Notepad::redo);

    connect(ui->actionAbout, &QAction::triggered, this, &Notepad::showAbout);
}

Notepad::~Notepad() {
    delete ui;
}

// On créé un nouveau document
void Notepad::newDocument() {
    currentFilePath.clear();
    ui->textArea->setText(QString());
    setWindowTitle("Notepad : Nouveau Document");
}

// On ouvre un document
void Notepad::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", "", "Documents HTML (*.html);;Tous les fichiers (*)");
    QFile file(fileName);
    currentFilePath = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'ouvrir le fichier : " + file.errorString());
        return;
    }
    setWindowTitle("Notepad : " + fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textArea->setHtml(text);
    file.close();
}

// On sauvegarde un document
void Notepad::save() {
    QString filePath;
    if (currentFilePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "Sauvegarder le fichier", "", "Documents HTML (*.html)");
        currentFilePath = filePath;
    } else {
        filePath = currentFilePath;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'enregistrer le fichier : " + file.errorString());
        return;
    }
    setWindowTitle("Notepad : " + filePath);
    QTextStream out(&file);
    out << ui->textArea->toHtml();
    file.close();
}

void Notepad::saveAs() {
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "", "Documents HTML (*.html)");
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'enregistrer le fichier : " + file.errorString());
        return;
    }
    currentFilePath = filePath;
    setWindowTitle("Notepad : " + filePath);
    QTextStream out(&file);
    out << ui->textArea->toHtml();
    file.close();
}



void Notepad::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected) {
        QTextCharFormat format;
        format.setFont(font);
        ui->textArea->textCursor().mergeCharFormat(format);
    }
}

void Notepad::setItalic() {
    ui->textArea->setFontItalic(!ui->textArea->fontItalic());
}

void Notepad::setBold() {
    int weight = ui->textArea->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold;
    ui->textArea->setFontWeight(weight);
}

void Notepad::setUnderline() {
    ui->textArea->setFontUnderline(!ui->textArea->fontUnderline());
}

void Notepad::setColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Text Color");
    if (color.isValid()) {
        ui->textArea->setTextColor(color);
    }
}


void Notepad::copy() {
    ui->textArea->copy();
}
void Notepad::cut() {
    ui->textArea->cut();
}
void Notepad::paste() {
    ui->textArea->paste();
}
void Notepad::undo() {
    ui->textArea->undo();
}
void Notepad::redo() {
    ui->textArea->redo();
}

void Notepad::showAbout() {
    QMessageBox::about(this, "À propos", "Notepad - Application de traitement de texte simple\n"
                                         "Créé par ANTHONUS avec l'aide du tuto QT");
}
