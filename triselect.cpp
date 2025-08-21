#include "triselect.h"
#include "ui_triselect.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLabel>
#include "resultats.h"

Triselect::Triselect(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Triselect), scene(this), joueur("Alice"), ok(this, true), ko(this, false)
{
    ui->setupUi(this);
    auto gv = ui->graphicsView;
    gv->setRenderHint(QPainter::RenderHint::Antialiasing);
    scene.setSceneRect(0, 0, 800, 800);
    gv->setScene(&scene);
    poubelles.emplace(poubelles.end(), Poubelle::Type::TOUT_VENANT);
    poubelles.emplace(poubelles.end(), Poubelle::Type::ORGANIQUES);
    poubelles.emplace(poubelles.end(), Poubelle::Type::RECYCLABLES);
    poubelles.emplace(poubelles.end(), Poubelle::Type::VERRE);
    for (Poubelle &poubelle : poubelles)
    {
        scene.addItem(&poubelle);
        poubelle.setPos(poubelle.type * 150 + 100, 350);
    }

    dechets.emplace(dechets.end(), Dechet::Type::BOUTEILLE_VERRE, *this);
    dechets.emplace(dechets.end(), Dechet::Type::CANETTE, *this);
    dechets.emplace(dechets.end(), Dechet::Type::TROGNON, *this);
    dechets.emplace(dechets.end(), Dechet::Type::SACHET, *this);
    for (Dechet &dechet : dechets)
    {
        scene.addItem(&dechet);
        dechet.setPos(dechet.type * 150 + 100, 150);
    }

    scene.addItem(&ok);
    scene.addItem(&ko);
}

Triselect::~Triselect()
{
    dechets.clear();
    poubelles.clear();
    delete ui;
}

void Triselect::affiche_resultat(size_t points)
{
    static auto dialog = Resultats(this, dechets);
    dialog.setWindowTitle(QString("C'est terminé, %1").arg(joueur));
    dialog.setPoints(points, dechets.size());
    for (Dechet &dechet : dechets)
    {
        dialog.ajouteDechet(dechet);
    }
    dialog.envoie(joueur);
    dialog.open();
}

std::tuple<size_t, bool> Triselect::points()
{
    size_t points = 0;
    bool termine = true;
    for (Dechet &dechet : dechets)
    {
        if (dechet.poubelle_choisie == nullptr)
        {
            termine = false;
        }
        else
        {
            if (dechet.est_valide())
            {
                points += 1;
            }
        }
    }
    return std::tuple<size_t, bool>(points, termine);
}

void Triselect::dechet_jete(Dechet &dechet, QPointF pos)
{
    const auto [pts, termine] = points();
    if (termine)
    {
        affiche_resultat(pts);
    }
    auto &okko = dechet.est_valide() ? ok : ko;
    auto size = okko.sceneBoundingRect().size();
    pos -= QPointF(size.width() / 2, size.height() / 2);
    okko.setPos(pos);
    okko.start();
}

void Triselect::setJoueur(const QString &joueur)
{
    this->joueur = joueur;
    if (ui->label)
    {
        ui->label->setText(QString("<b>%1</b>, place les déchets dans la bonne poubelle.").arg(joueur));
    }
}
