#ifndef RESULTATS_H
#define RESULTATS_H

#include <QDialog>
#include "dechet.h"
#include <QLabel>

namespace Ui
{
    class Resultats;
}

class Resultats : public QDialog
{
    Q_OBJECT

public:
    explicit Resultats(QWidget *parent = nullptr, const std::list<Dechet> &dechets = {});
    ~Resultats();
    void ajouteDechet(const Dechet &dechet);
    void setPoints(int points, int total);
    QLabel *label;

private:
    Ui::Resultats *ui;
};

#endif // RESULTATS_H
