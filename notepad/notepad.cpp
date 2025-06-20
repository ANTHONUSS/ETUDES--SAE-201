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

    connect(ui->numEtape, QOverload<int>::of(&QSpinBox::valueChanged), this, &Notepad::onNumEtapeChanged);
    connect(ui->numParcours, QOverload<int>::of(&QSpinBox::valueChanged), this, &Notepad::onNumParcoursChanged);

    std::cout << "\t[+]NotePad" << std::endl;
    connect(ui->exportMap, &QPushButton::clicked, this, &Notepad::exportMap);

}

Notepad::~Notepad() {
    delete ui;

    for (Parcours* parcours : parcoursList) {
        delete parcours;
    }
    parcoursList.clear();

    for (Personnage* perso : persoList) {
        delete perso;
    }
    persoList.clear();

    std::cout << "\t[-]Notepad" << std::endl;
}

//on ajoute un nouveau parcours
void Notepad::addParcours(const QString& nom, const QString& ville, int departement, unsigned int difficulte,
    float duree, float kilometre, const QString& image, const QString& entete)
{
    parcoursList.push_back(new Parcours(nom, ville, departement, difficulte, duree, kilometre, image, entete));
}

//initialise la liste des personnages
void Notepad::initPerso() {

    QDir dir(":/Personnages/personnages-parcours");

    for (const QFileInfo &fichier : dir.entryInfoList(QDir::Files)){
//        std::cout<<fichier.baseName().toStdString()<<std::endl;
//        std::cout<<fichier.absoluteFilePath().toStdString()<<std::endl;
        Personnage* p=new Personnage(fichier.baseName(),fichier.absoluteFilePath());
        persoList.push_back(p);
    }
}

//on check si le personnage se trouve dans la liste
bool Notepad::PersoExiste(const QString &p){
    for (Personnage* x:persoList){
        if(comparesEqual(x->getNom(),p)){
            return true;
        }
    }
    return false;
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
    currentFilePath = fileName;
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
        QStringList latSplit = parts[2].split('.', Qt::SkipEmptyParts);
        int latM = latSplit[0].toInt();
        int latS = latSplit[1].toInt();
        QString WE = parts[3];
        int lonD = parts[4].toInt();
        QStringList lonSplit = parts[5].split('.', Qt::SkipEmptyParts);
        int lonM = lonSplit[0].toInt();
        int lonS = lonSplit[1].toInt();


        int reponse = in.readLine().toInt();
        QString dialog = getDialog(in);

        lastParcours->addEtape(titre, dialog, reponse,
            latD, latM, latS, NS,
            lonD, lonM, lonS, WE);
    }

    ui->numParcours->setMaximum(parcoursList.size());
    ui->numParcours->setValue(parcoursList.size());
    ui->numEtape->setMaximum(lastParcours->getNombreEtapes());
    ui->numEtape->setValue(1);

    afficherParcours(parcoursList.size() - 1);
    afficherEtape(0);

    setWindowTitle("Notepad : " + fileName);
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
    ui->spinBox->setValue(etape->getReponse());

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
        ui->
        imagePath->setText(parcours->getImage());
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
    <h2>Parcours: " + parcours->getNom() + "</h2>\n\
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
        out << "                L.marker([" << etape->getLatitude() << ", " << etape->getLongitude() << "]).addTo(map)\n\
                    .bindPopup(\"" << etape->getTitre() << "\");\n\
                debug('Marqueur " << i+1 << " ajouté');\n";
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


