#include "resultats.h"
#include "ui_resultats.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#if defined(__EMSCRIPTEN__)
#include "EmscriptenFormSubmitter.h"
#else
#include "FormSubmitter.h"
#endif

Resultats::Resultats(QWidget *parent, const std::list<Dechet> &dechets)
    : QDialog(parent), ui(new Ui::Resultats), points(0)
{
    ui->setupUi(this);
    label = ui->label;
}

Resultats::~Resultats()
{
    delete ui;
}

void Resultats::setPoints(int points, int total)
{
    this->points = points;
    auto string =
        QString("Tu as marqué %1 point%3 sur %2.").arg(points).arg(total).arg(points != 1 ? "s" : "");
    label->setText(string);
}

void Resultats::ajouteDechet(const Dechet &dechet)
{
    QLabel *image = new QLabel(this);
    auto pixmap = QPixmap(dechet.est_valide() ? ":/images/icones/coche16.png" : ":/images/icones/croix16.png");
    image->setPixmap(pixmap);
    image->setFixedSize(16, 16);
    auto poubelle = dechet.poubelle_choisie;
    auto string = QString("<b>%1</b> a été déposé%4 dans <b>%2</b>. %3")
                      .arg(dechet.nom(dechet.type), poubelle->nom(poubelle->type))
                      .arg(dechet.est_valide() ? "Correct !" : "Incorrect !")
                      .arg(dechet.feminin() ? "e" : "");
    QLabel *text = new QLabel(string, this);
    ui->formLayout->addRow(image, text);
}

void Resultats::envoie(QString nom)
{
#ifdef __EMSCRIPTEN__
    static EmscriptenFormSubmitter formSubmitter;
#else
    static FormSubmitter formSubmitter;
#endif
    formSubmitter.submitForm(nom, QString("%1").arg(points));
}
