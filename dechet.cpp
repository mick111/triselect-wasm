#include "dechet.h"
#include "utils.h"
#include "triselect.h"
#include <QGraphicsSceneMouseEvent>

Dechet::Dechet(const QString &filename, const Type type,
               Triselect &triselect)
    : QGraphicsPixmapItem(nullptr), type(type), triselect(triselect),
      poubelle_choisie(nullptr), type_poubelle(Poubelle::Type::TOUT_VENANT)
{
    setPixmap(QPixmap(filename));
    setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    setScale(0.1);
    setToolTip(nom(type));
    setFlag(GraphicsItemFlag::ItemIsMovable);
    setFlag(GraphicsItemFlag::ItemSendsScenePositionChanges);

    switch (type)
    {
    case SACHET:
        type_poubelle = Poubelle::Type::TOUT_VENANT;
        break;
    case TROGNON:
        type_poubelle = Poubelle::Type::ORGANIQUES;
        break;
    case CANETTE:
        type_poubelle = Poubelle::Type::RECYCLABLES;
        break;
    case BOUTEILLE_VERRE:
        type_poubelle = Poubelle::Type::VERRE;
        break;
    default:
        break;
    }
}

Dechet::Dechet(const Type type, Triselect &triselect)
    : Dechet(QString(":/images/dechet_%1.png").arg(type), type, triselect) {};

const QString Dechet::nom(const Type type)
{
    switch (type)
    {
    case SACHET:
        return "Sachet";
    case TROGNON:
        return "Trognon";
    case CANETTE:
        return "Canette";
    case BOUTEILLE_VERRE:
        return "Bouteille en verre";
    default:
        return "???";
    }
    return "!!!";
}

bool Dechet::est_valide() const
{
    if (poubelle_choisie == nullptr)
    {
        return false;
    }
    return poubelle_choisie->type == type_poubelle;
}

bool Dechet::touche(const Poubelle &poubelle) const
{
    return poubelle.imageSceneBoundingRect().intersects(
        halfRect(sceneBoundingRect()));
}

void Dechet::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
    QGraphicsPixmapItem::mousePressEvent(event);
}

void Dechet::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

void Dechet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Get position in scene coordinates

    for (Poubelle &poubelle : triselect.poubelles)
    {
        if (touche(poubelle))
        {
            QPointF pos = event->scenePos();
            hide();
            poubelle_choisie = &poubelle;
            triselect.dechet_jete(*this, pos);
        }
        poubelle.setSelected(false);
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

QVariant Dechet::itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value)
{
    if (change ==
        QGraphicsItem::GraphicsItemChange::ItemScenePositionHasChanged)
    {
        for (Poubelle &poubelle : triselect.poubelles)
        {
            poubelle.setSelected(touche(poubelle));
        }
    }
    return QGraphicsPixmapItem::itemChange(change, value);
}

bool Dechet::feminin(Dechet::Type type)
{
    switch (type)
    {
    case Dechet::CANETTE:
    case Dechet::BOUTEILLE_VERRE:
        return true;
    default:
        return false;
    }
}
