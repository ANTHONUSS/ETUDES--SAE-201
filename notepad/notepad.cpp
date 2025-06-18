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
    // Création d'étapes et d'un parcours de test
    Parcours parcoursToulouse("Découverte de Toulouse", "Toulouse", 31, 2, 3.5, 5.2, "toulouse.jpg");

    // Ajout d'étapes au parcours
    Etape etape1("Capitole", 43.6045, 1.4442, "Place principale de Toulouse", 1);
    Etape etape2("Basilique Saint-Sernin", 43.6073, 1.4415, "Plus grande église romane d'Europe", 2);
    Etape etape3("Jardin des Plantes", 43.5922, 1.4504, "Jardin botanique historique", 3);
    Etape etape4("Cité de l'Espace", 43.5883, 1.4933, "Parc à thème spatial", 4);

    parcoursToulouse.addEtape(etape1);
    parcoursToulouse.addEtape(etape2);
    parcoursToulouse.addEtape(etape3);
    parcoursToulouse.addEtape(etape4);

    // exporte la map en HTML
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter la carte", "", "Fichiers HTML (*.html)");
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".html", Qt::CaseInsensitive))
        fileName += ".html";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'exporter la carte : " + file.errorString());
        return;
    }

    // Version simplifiée HTML avec Leaflet intégré directement
    QTextStream out(&file);
    out << "<!DOCTYPE html>\n\
<html>\n\
<head>\n\
    <meta charset=\"utf-8\">\n\
    <title>Carte du parcours</title>\n\
    <style>\n\
        html, body { height: 100%; margin: 0; padding: 0; }\n\
        #map { height: 600px; width: 100%; }\n\
        .info { padding: 6px 8px; background: white; background: rgba(255,255,255,0.8); box-shadow: 0 0 15px rgba(0,0,0,0.2); border-radius: 5px; }\n\
        #debug { position: fixed; bottom: 10px; left: 10px; background: white; padding: 10px; z-index: 1000; border: 1px solid red; display: none; }\n\
    </style>\n\
    <link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.css\" />\n\
</head>\n\
<body>\n\
    <h2>Parcours: " + parcoursToulouse.nom + "</h2>\n\
    <div id=\"map\"></div>\n\
    <div id=\"debug\"></div>\n\
    <script src=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.js\"></script>\n\
    <script>\n\
        // Fonction de débogage\n\
        function debug(msg) {\n\
            var debugElement = document.getElementById('debug');\n\
            debugElement.style.display = 'block';\n\
            debugElement.innerHTML += msg + '<br>';\n\
            console.log(msg);\n\
        }\n\
\n\
        // Attendre que tout soit chargé\n\
        window.onload = function() {\n\
            debug('Page chargée');\n\
\n\
            // Vérifier que Leaflet est chargé\n\
            if (typeof L === 'undefined') {\n\
                debug('ERREUR: Leaflet n\\'est pas chargé');\n\
                return;\n\
            }\n\
\n\
            debug('Leaflet trouvé, initialisation de la carte...');\n\
\n\
            try {\n\
                // Création de la carte\n\
                var map = L.map('map').setView([43.6045, 1.4442], 13);\n\
                debug('Carte créée');\n\
\n\
                // Fonds de carte\n\
                L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {\n\
                    attribution: '&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors'\n\
                }).addTo(map);\n\
                debug('Fond de carte ajouté');\n\
\n\
                // Points d'intérêt\n";

    // Ajouter les marqueurs d'étapes manuellement
    for (int i = 0; i < parcoursToulouse.etapes.size(); i++) {
        Etape* etape = parcoursToulouse.etapes[i];
        out << "                L.marker([" << etape->getLatitude() << ", " << etape->getLongitude() << "]).addTo(map)\n\
                    .bindPopup(\"" << etape->getTitre() << "\");\n\
                debug('Marqueur " << i+1 << " ajouté');\n";
    }

    // Continuer avec le tracé du parcours
    out << "\n\
                // Tracé du parcours\n\
                var pathCoords = [\n";

    // Ajouter les coordonnées du parcours
    for (int i = 0; i < parcoursToulouse.etapes.size(); i++) {
        Etape* etape = parcoursToulouse.etapes[i];
        out << "                    [" << etape->getLatitude() << ", " << etape->getLongitude() << "]";
        if (i < parcoursToulouse.etapes.size() - 1) {
            out << ",\n";
        } else {
            out << "\n";
        }
    }

    out << "                ];\n\
                var path = L.polyline(pathCoords, {color: 'blue', weight: 4}).addTo(map);\n\
                debug('Tracé du parcours ajouté');\n\
\n\
                // Ajuster la vue pour voir tout le parcours\n\
                map.fitBounds(path.getBounds());\n\
                debug('Zoom ajusté');\n\
\n\
                // Afficher info\n\
                var info = L.control({position: 'topleft'});\n\
                info.onAdd = function() {\n\
                    var div = L.DomUtil.create('div', 'info');\n\
                    div.innerHTML = '<h4>" + parcoursToulouse.nom + "</h4>' +\n\
                        'Ville: " + parcoursToulouse.ville + "<br>' +\n\
                        'Difficulté: " + QString::number(parcoursToulouse.difficulte) + "/5<br>' +\n\
                        'Durée: " + QString::number(parcoursToulouse.duree) + " h<br>' +\n\
                        'Distance: " + QString::number(parcoursToulouse.kilometre) + " km';\n\
                    return div;\n\
                };\n\
                info.addTo(map);\n\
                debug('Informations du parcours ajoutées');\n\
                \n\
                // Tout s'est bien passé, cacher le débogage\n\
                setTimeout(function() {\n\
                    document.getElementById('debug').style.display = 'none';\n\
                }, 3000);\n\
                \n\
            } catch(e) {\n\
                debug('ERREUR: ' + e.message);\n\
            }\n\
        };\n\
    </script>\n\
</body>\n\
</html>";

    file.close();

    QMessageBox::information(this, "Exportation de la carte",
                           "Carte exportée avec succès en HTML.", QMessageBox::Ok);

    // Ouverture de la carte dans le navigateur
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


