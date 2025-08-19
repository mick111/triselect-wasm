#ifndef DECHET_H
#define DECHET_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "poubelle.h"

class Triselect;

class Dechet : public QGraphicsPixmapItem
{
public:
    enum Type : size_t
    {
        SACHET,
        TROGNON,
        CANETTE,
        BOUTEILLE_VERRE
    };

    Dechet(const QString &filename, const Type type, Triselect &triselect);
    Dechet(const Type type, Triselect &triselect);
    const Type type;
    bool touche(const Poubelle &poubelle) const;
    Poubelle *poubelle_choisie;
    Poubelle::Type type_poubelle;
    static const QString nom(Type type);
    const QString nom() const { return Dechet::nom(type); }
    bool est_valide() const;
    static bool feminin(const Type type);
    bool feminin() const { return Dechet::feminin(type); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private:
    Triselect &triselect;
};

#endif // DECHET_H
