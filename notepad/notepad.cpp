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

    connect(ui->actionImage, &QAction::triggered, this, &Notepad::insertImage);

    connect(ui->actionExporter, &QAction::triggered, this, &Notepad::exportPDF);
    connect(ui->exportMap, &QPushButton::clicked, this, &Notepad::exportMap);
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
    QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", "",
        "Documents texte (*.txt);;Documents HTML (*.html);;Tous les fichiers (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    currentFilePath = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'ouvrir le fichier : " + file.errorString());
        return;
    }

    setWindowTitle("Notepad : " + fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    if (fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        ui->textArea->setPlainText(text);
    } else {
        ui->textArea->setHtml(text);
    }

    file.close();
}

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

void Notepad::exportPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                  "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    ui->textArea->document()->print(&printer);

    QMessageBox::information(this, "Exportation PDF",
                           "Document exporté avec succès en PDF.", QMessageBox::Ok);
}

void Notepad::exportMap() {
// exporte la map en HTML
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter la carte",
                                                    "", "Fichiers HTML (*.html)");
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".html", Qt::CaseInsensitive))
        fileName += ".html";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'exporter la carte : " + file.errorString());
        return;
    }

    QTextStream out(&file);
    // Écrire le contenu de la carte dans le fichier HTML
    out << "<!DOCTYPE html>\
<html>\
<head>\
    <title>Carte du monde avec Leaflet</title>\
    <meta charset=\"utf-8\" />\
    <link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet/dist/leaflet.css\" />\
    <style>\
        #map { height: 600px; width: 100%; }\
    </style>\
</head>\
<body>\
    <div id=\"map\"></div>\
    <script src=\"https://unpkg.com/leaflet/dist/leaflet.js\"></script>\
    <script>\
        try {\
            var map = L.map('map').setView([20, 0], 2);\
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\
                attribution: '&copy; OpenStreetMap contributors'\
            }).addTo(map);\
            console.log('Carte chargée avec succès');\
        } catch(error) {\
            console.error('Erreur de chargement de la carte:', error);\
            document.body.innerHTML += '<p style=\"color:red\">Erreur: ' + error.message + '</p>';\
        }\
    </script>\
</body>\
</html>";

    file.close();
    QMessageBox::information(this, "Exportation de la carte",
                           "Carte exportée avec succès en HTML.", QMessageBox::Ok);

//oucerture de la carte dans le navigateur
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void Notepad::insertImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Insérer une image", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::warning(this, "Erreur", "Impossible de charger l'image.");
            return;
        }

        QTextCursor cursor = ui->textArea->textCursor();
        QTextDocument *document = ui->textArea->document();

        // Ajouter l'image au document
        QUrl url = QUrl::fromLocalFile(fileName);
        document->addResource(QTextDocument::ImageResource, url, QVariant(image));

        // Insérer l'image au curseur actuel
        cursor.insertImage(fileName);
    }
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
    QTextCharFormat format;
    format.setFontItalic(!ui->textArea->textCursor().charFormat().fontItalic());
    ui->textArea->textCursor().mergeCharFormat(format);
}

void Notepad::setBold() {
    QTextCharFormat format;
    int weight = ui->textArea->textCursor().charFormat().fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold;
    format.setFontWeight(weight);
    ui->textArea->textCursor().mergeCharFormat(format);
}

void Notepad::setUnderline() {
    QTextCharFormat format;
    format.setFontUnderline(!ui->textArea->textCursor().charFormat().fontUnderline());
    ui->textArea->textCursor().mergeCharFormat(format);
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
    QMessageBox::about(this, "À propos", "Tèrr’Aventura creator - Application de création de parcours\n");
}


