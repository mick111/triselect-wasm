#ifndef TRISELECT_H
#define TRISELECT_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDialog>
#include "poubelle.h"
#include "dechet.h"
#include "okko.h"
#include "welcomedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Triselect;
}
QT_END_NAMESPACE

class Triselect : public QMainWindow
{
    Q_OBJECT

public:
    Triselect(QWidget *parent = nullptr);
    ~Triselect();
    std::list<Poubelle> poubelles;
    void dechet_jete(Dechet &dechet, QPointF pos);
    QString joueur;
    std::tuple<size_t, bool> points();

private:
    Ui::Triselect *ui;
    QGraphicsScene scene;
    std::list<Dechet> dechets;
    void affiche_resultat(size_t points);
    Okko ok;
    Okko ko;

public slots:
    void setJoueur(const QString &joueur);
};
#endif // TRISELECT_H
