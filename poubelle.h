#ifndef POUBELLE_H
#define POUBELLE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QWidget>

class Poubelle : public QGraphicsItemGroup
{
public:
    enum Type : size_t
    {
        ORDURES,
        ORGANIQUES,
        RECYCLABLES,
        VERRE
    };
    Poubelle(const QString &filename, const Type type);
    Poubelle(const Type type);
    const Type type;
    QString nom(Type type) const;
    const QString nom() const { return nom(type); }
    QRectF imageSceneBoundingRect() const
    {
        return QRectF(
            sceneBoundingRect().topLeft(),
            pixmapItem.pixmap().size() * pixmapItem.scale());
    }

    void setSelected(bool selected)
    {
        QGraphicsItemGroup::setSelected(selected);
        pixmapItem.setSelected(selected);
    }

protected:
    QGraphicsPixmapItem pixmapItem;
};

#endif // POUBELLE_H
