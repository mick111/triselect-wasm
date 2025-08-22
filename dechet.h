#ifndef DECHET_H
#define DECHET_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "poubelle.h"

class Triselect;

class Dechet : public QGraphicsItemGroup
{
public:
    enum Type : size_t
    {
        ALIMENTAIRE,
        BOCAL,
        BOUTEILLE_VERRE,
        CANETTE,
        COUVERTS_BOIS,
        COUVERTS_PLASTIQUE,
        DOSETTE,
        EPLUCHURES,
        GOBELET,
        MARC_CAFE,
        MASQUE,
        MOUCHOIR,
        POSTIT,
        RESTES_REPAS,
        SACHET_PLASTIQUE,
        SACHET_THE,
        STYLO,
        YAOURT
    };

    static const QMap<Dechet::Type, Poubelle::Type> typeToPoubelle;
    static const QMap<Dechet::Type, QString> typeToNom;
    static const QMap<Dechet::Type, QString> typeToImage;

    Dechet(const Type type, Triselect &triselect);
    const Type type;
    const Poubelle::Type type_poubelle;
    const QString nom;
    const bool feminin;
    const bool pluriel;

    const unsigned int position;

    Poubelle *poubelle_choisie;

    bool est_valide() const;
    bool touche(const Poubelle &poubelle) const;
    QRectF imageSceneBoundingRect() const
    {
        return QRectF(
            sceneBoundingRect().topLeft(),
            pixmapItem.pixmap().size() * pixmapItem.scale());
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    QGraphicsPixmapItem pixmapItem;
    QGraphicsTextItem textItem;
    QGraphicsPathItem fondBlanc;

private:
    Triselect &triselect;
};

#endif // DECHET_H
