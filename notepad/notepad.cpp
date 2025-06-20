//
// Created by ANTHONUS on 11/06/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Notepad.h" resolved

#include "notepad.h"
#include "ui_Notepad.h"
#include "../parcours/Parcours.h"


Notepad::Notepad(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::Notepad)
{
    ui->setupUi(this);

    // Connecter le bouton "ui->BOUTON" quand il est trigerred à la classe "this", et ensuite à la méthode (qui est définie dans les slots du .h)
    connect(ui->actionNouveau, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOuvrir, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionEnregistrer, &QAction::triggered, this, &Notepad::save);
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

    connect(ui->numEtape, QOverload<int>::of(&QSpinBox::valueChanged), this, &Notepad::onNumEtapeChanged);
    connect(ui->numParcours, QOverload<int>::of(&QSpinBox::valueChanged), this, &Notepad::onNumParcoursChanged);

    std::cout << "\t[+]NotePad" << std::endl;
    connect(ui->exportMap, &QPushButton::clicked, this, &Notepad::exportMap);
    connect(ui->supprEtape, &QPushButton::clicked, this, Notepad::supprEtape );
    connect(ui->ajouterEtape, &QPushButton::clicked, this, &Notepad::ajouterEtape);
}

Notepad::~Notepad() {
    delete ui;

    for (Parcours* parcours : parcoursList) {
        delete parcours;
    }
    parcoursList.clear();

    std::cout << "\t[-]Notepad" << std::endl;
}

void Notepad::addParcours(const QString& nom, const QString& ville, int departement, unsigned int difficulte,
    float duree, float kilometre, const QString& image, const QString& entete)
{
    parcoursList.push_back(new Parcours(nom, ville, departement, difficulte, duree, kilometre, image, entete));
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
        "Documents texte (*.txt);;Tous les fichiers (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'ouvrir le fichier : " + file.errorString());
        return;
    }

    QTextStream in(&file);

    /* chargement de l'entête */
    QString nom = in.readLine();
    QString ville = in.readLine();
    unsigned short int departement = in.readLine().toInt();
    unsigned short int difficulte = in.readLine().toInt();
    float duree = in.readLine().toFloat();
    float kilometre = in.readLine().toFloat();
    QString imagePath = in.readLine();
    QString entete = getDialog(in);

    addParcours(nom, ville, departement, difficulte, duree, kilometre, imagePath, entete);


    /* Chargement des étapes */
    Parcours* lastParcours = parcoursList.last();
    while (!in.atEnd()) {
        unsigned short int index = in.readLine().toInt();
        QString titre = in.readLine();
        QString coords = in.readLine();
        QStringList parts = coords.split(' ', Qt::SkipEmptyParts);

        QString NS = parts[0];
        int latD = parts[1].toInt();
        float latM = parts[2].toFloat();
        QString WE = parts[3];
        int lonD = parts[4].toInt();
        float lonM = parts[5].toFloat();

        int reponse = in.readLine().toInt();
        QString dialog = getDialog(in);

        lastParcours->addEtape(titre, dialog, reponse,
            latD, latM, NS,
            lonD, lonM, WE);
    }

    ui->numParcours->setMaximum(parcoursList.size());
    ui->numParcours->setValue(parcoursList.size());
    ui->numEtape->setMaximum(lastParcours->getNombreEtapes());
    ui->numEtape->setValue(1);

    afficherParcours(parcoursList.size() - 1);
    afficherEtape(0);

    setWindowTitle("Tèrr’Aventura creator : " + fileName);
    file.close();
}

QString Notepad::getDialog(QTextStream& in) const {
    QString dialog;
    QString content = in.readLine();
    while (content != "%") {
        if (content != "#" && content != "%") {
            dialog += content + "\n";
        }
        content = in.readLine();
    }
    return dialog;
}

void Notepad::afficherEtape(int index) {
    Parcours* parcours = parcoursList.at(ui->numParcours->value()-1);
    Etape* etape = parcours->getEtape(index);
    ui->nomEtape->setText(etape->getTitre());
    ui->latitudeSpinBox->setValue(etape->getLatitude());
    ui->LongitudeSpinBox->setValue(etape->getLongitude());
    ui->reponse->setValue(etape->getReponse());

    /*TODO:
     * Ajouter le fait que les noms des bougs soient lues avec des images avant
     */
    QString dialog = etape->getDialog();
    ui->textArea->setHtml(dialog.toHtmlEscaped().replace("\n", "<br>"));
}

void Notepad::afficherParcours(int index) {
    Parcours* parcours = parcoursList.at(index);
    ui->nomParcours->setText(parcours->getNom());
    ui->localisationInput->setText(parcours->getVille());
    ui->dptInput->setValue(parcours->getDepartement());
    ui->diffuculteInput->setValue(parcours->getDifficulte());
    ui->dureeInput->setValue(parcours->getDuree());
    ui->longueurInput->setValue(parcours->getKilometre());
    QImage img(parcours->getImage());
    if (!img.isNull()) {
        ui->sideImage->setPixmap(QPixmap::fromImage(img));
        ui->imagePath->setText(parcours->getImage());
    } else {
        ui->sideImage->setText("Image non trouvée");
        ui->imagePath->setText(QString());
    }
    ui->enteteArea->setText(parcours->getEntete());
    ui->numEtape->setValue(1);
    ui->numEtape->setMaximum(parcours->getNombreEtapes());
    afficherEtape(0);
}

void Notepad::save() {
    QString filePath = "data/saves/" + ui->nomParcours->text() + ".txt";

    QDir dir("data");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QDir dir2("data/saves");
    if (!dir2.exists()) {
        dir2.mkpath(".");
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Impossible d'enregistrer le fichier : " + file.errorString());
        return;
    }
    setWindowTitle("Tèrr’Aventura creator : " + filePath);
    QTextStream out(&file);

    out << ui->nomParcours->text() << "\n";
    out << ui->localisationInput->text() << "\n";
    out << ui->dptInput->value() << "\n";
    out << ui->diffuculteInput->value() << "\n";
    out << ui->dureeInput->value() << "\n";
    out << ui->longueurInput->value() << "\n";
    out << ui->imagePath->text() << "\n";
    out << ui->enteteArea->toPlainText() << "%\n";

    Parcours* parcours = parcoursList.at(ui->numParcours->value()-1);
    int cpt = 1;
    for (Etape* etape : parcours->getEtapes()) {
        out << cpt++ << "\n";
        out << etape->getTitre() << "\n";
        out << etape->getCoordonnee(true) << "\n";
        out << etape->getReponse() << "\n";
        out << etape->getDialog(); //TODO: ajouter les # pour les dialogues
        out << "%\n";
    }

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
    // Vérifier si des parcours sont disponibles
    if (parcoursList.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun parcours disponible pour l'exportation.");
        return;
    }

    // Récupérer le parcours actuellement sélectionné
    int parcoursIndex = ui->numParcours->value() - 1;
    if (parcoursIndex < 0 || parcoursIndex >= parcoursList.size()) {
        QMessageBox::warning(this, "Erreur", "Index de parcours invalide.");
        return;
    }

    Parcours* parcours = parcoursList.at(parcoursIndex);

    // Exporter la map en HTML
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
    // Dans la partie CSS de votre code HTML généré, remplacez :
    out << "<!DOCTYPE html>\n\
<html>\n\
<head>\n\
    <meta charset=\"utf-8\">\n\
    <title>Carte du parcours</title>\n\
    <style>\n\
        html, body { height: 100%; margin: 0; padding: 0; overflow: hidden; }\n\
        #map { position: absolute; top: 0; left: 0; width: 100%; height: 100vh; z-index: 1; }\n\
        .info { padding: 6px 8px; background: white; background: rgba(255,255,255,0.8); box-shadow: 0 0 15px rgba(0,0,0,0.2); border-radius: 5px; z-index: 1000; }\n\
        #debug { position: fixed; bottom: 10px; left: 10px; background: white; padding: 10px; z-index: 1000; border: 1px solid red; display: none; }\n\
    </style>\n\
    <link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.css\" />\n\
</head>\n\
<body>\n\
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
    for (int i = 0; i < parcours->getNombreEtapes(); i++) {
        Etape* etape = parcours->getEtape(i);
        if (etape->getLatitude()!=0.0f && etape->getLongitude()!=0.0f) {
            out << "                L.marker([" << etape->getLatitude() << ", " << etape->getLongitude() << "]).addTo(map)\n\
                    .bindPopup(\"" << etape->getTitre() << "\");\n\
                debug('Marqueur " << i+1 << " ajouté');\n";
        }
    }

    // Continuer avec le tracé du parcours
    out << "\n\
                // Tracé du parcours\n\
                var pathCoords = [\n";

    // Ajouter les coordonnées du parcours
    for (int i = 0; i < parcours->getNombreEtapes(); i++) {
        Etape* etape = parcours->getEtape(i);
        if (etape->getLatitude()!=0.0f && etape->getLongitude()!=0.0f) {
            out << "                    [" << etape->getLatitude() << ", " << etape->getLongitude() << "]";
            if (i < parcours->getNombreEtapes() - 1) {
                out << ",\n";
            } else {
                out << "\n";
            }
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
                    div.innerHTML = '<h4>" + parcours->getNom() + "</h4>' +\n\
                        'Ville: " + parcours->getVille() + "<br>' +\n\
                        'Difficulté: " + QString::number(parcours->getDifficulte()) + "/5<br>' +\n\
                        'Durée: " + QString::number(parcours->getDuree()) + " h<br>' +\n\
                        'Distance: " + QString::number(parcours->getKilometre()) + " km';\n\
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
}

void Notepad::supprEtape() {
    if (ui->numEtape->value() - 1!=0) {
    int numEtape = ui->numEtape->value() - 1;
    int numParcours = ui->numParcours->value() - 1;
    Parcours* parcours = parcoursList.at(numParcours);
    if (numEtape < 0 || numEtape >= parcours->getNombreEtapes()) {
        QMessageBox::warning(this, "Erreur", "Numéro d'étape invalide.");
        return;
    }
    // Supprimer l'étape avec la fonction remove de vector
    parcoursList.at(numParcours)->supprimerEtape(numEtape);
    ui->numEtape->setMaximum(parcours->getNombreEtapes());
    afficherEtape(numEtape-1);
    }
    else {
        // reset toutes les valeurs
        ui->nomEtape->setText("");
        ui->latitudeSpinBox->setValue(0.0f);
        ui->LongitudeSpinBox->setValue(0.0f);
        ui->reponse->setValue(0);
        ui->textArea->setHtml("");
        ui->numEtape->setValue(1);
        ui->numEtape->setMaximum(1);
        ui->numParcours->setValue(1);
        ui->numParcours->setMaximum(1);
        ui->nomParcours->setText("");
        ui->localisationInput->setText("");
        ui->dptInput->setValue(0);
        ui->diffuculteInput->setValue(0);
        ui->dureeInput->setValue(0.0f);
        ui->longueurInput->setValue(0.0f);
        ui->sideImage->setText("Aucune image");
        ui->imagePath->setText("");
        ui->enteteArea->setText("");


    }
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

void Notepad::onNumEtapeChanged(int value) {
    //TODO: enregistrer l'étape avant de changer (pour ça faut faire la fonction save avant)
    if (value < 1 || value > parcoursList.at(ui->numParcours->value()-1)->getNombreEtapes())
        return;
    afficherEtape(value - 1);
}

void Notepad::onNumParcoursChanged(int value) {
    //TODO: enregistrer le parcours avant de changer (pour ça faut faire la fonction save avant)
    if (value < 1 || value > parcoursList.at(ui->numParcours->value()-1)->getNombreEtapes())
        return;
    afficherParcours(value - 1);
}

void Notepad::ajouterEtape() {
    if (parcoursList.size()==0) {
        QMessageBox::warning(this, "Erreur", "Aucun parcours disponible pour ajouter une étape.");
        return;
    }
    //ajout d'une étape sur le parcours courant et champs vides
    Parcours* parcours = parcoursList.at(ui->numParcours->value()-1);
    parcours->addEtape("", "", 0, 0, 0.0f, "N", 0, 0.0f, "E");
    ui->numEtape->setMaximum(parcours->getNombreEtapes());
    ui->numEtape->setValue(parcours->getNombreEtapes());
    afficherEtape(parcours->getNombreEtapes() - 1);
}



