#include "poubelle.h"

Poubelle::Poubelle(const QString &filename, const Type type) : QGraphicsItemGroup(nullptr),
                                                               type(type),
                                                               pixmapItem(this)
{
    pixmapItem.setPixmap(QPixmap(filename));
    pixmapItem.setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    pixmapItem.setScale(0.1);
    QGraphicsTextItem *textItem = new QGraphicsTextItem(this);
    textItem->setPlainText(this->nom(type));
    textItem->setPos(pixmapItem.boundingRect().width() * pixmapItem.scale() / 2 - textItem->boundingRect().width() / 2,
                     pixmapItem.boundingRect().height() * pixmapItem.scale() + 5);

    this->setFlag(GraphicsItemFlag::ItemIsSelectable);
}

Poubelle::Poubelle(const Type type) : Poubelle(QString(":/images/poubelle_%1.png").arg(type), type)
{
}

QString Poubelle::nom(const Type type) const
{
    switch (type)
    {
    case ORDURES:
        return "Ordures ménagères";
    case ORGANIQUES:
        return "Organiques";
    case RECYCLABLES:
        return "Recyclables";
    case VERRE:
        return "Verre";
    default:
        return "???";
    }
    return "!!!";
}
