#include "dechet.h"
#include "utils.h"
#include "triselect.h"
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>

const QMap<Dechet::Type, Poubelle::Type> Dechet::typeToPoubelle{
    {Dechet::Type::ALIMENTAIRE, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::BOCAL, Poubelle::Type::VERRE},
    {Dechet::Type::BOUTEILLE_VERRE, Poubelle::Type::VERRE},
    {Dechet::Type::CANETTE, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::COUVERTS_BOIS, Poubelle::Type::ORDURES},
    {Dechet::Type::COUVERTS_PLASTIQUE, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::DOSETTE, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::EPLUCHURES, Poubelle::Type::ORGANIQUES},
    {Dechet::Type::GOBELET, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::MARC_CAFE, Poubelle::Type::ORGANIQUES},
    {Dechet::Type::MASQUE, Poubelle::Type::ORDURES},
    {Dechet::Type::MOUCHOIR, Poubelle::Type::ORDURES},
    {Dechet::Type::POSTIT, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::RESTES_REPAS, Poubelle::Type::ORGANIQUES},
    {Dechet::Type::SACHET_PLASTIQUE, Poubelle::Type::ORDURES},
    {Dechet::Type::SACHET_THE, Poubelle::Type::RECYCLABLES},
    {Dechet::Type::STYLO, Poubelle::Type::ORDURES},
    {Dechet::Type::YAOURT, Poubelle::Type::RECYCLABLES}};

const QMap<Dechet::Type, QString> Dechet::typeToNom{
    {Dechet::Type::ALIMENTAIRE, "Contenant alimentaire"},
    {Dechet::Type::BOCAL, "Bocal"},
    {Dechet::Type::BOUTEILLE_VERRE, "Bouteille en verre"},
    {Dechet::Type::CANETTE, "Canette"},
    {Dechet::Type::COUVERTS_BOIS, "Couverts en bois"},
    {Dechet::Type::COUVERTS_PLASTIQUE, "Couverts en plastique"},
    {Dechet::Type::DOSETTE, "Dosette"},
    {Dechet::Type::EPLUCHURES, "Épluchures"},
    {Dechet::Type::GOBELET, "Gobelet"},
    {Dechet::Type::MARC_CAFE, "Marc de café"},
    {Dechet::Type::MASQUE, "Masque"},
    {Dechet::Type::MOUCHOIR, "Mouchoir"},
    {Dechet::Type::POSTIT, "Post-it"},
    {Dechet::Type::RESTES_REPAS, "Restes de repas"},
    {Dechet::Type::SACHET_PLASTIQUE, "Sachet en plastique"},
    {Dechet::Type::SACHET_THE, "Sachet de thé"},
    {Dechet::Type::STYLO, "Stylo"},
    {Dechet::Type::YAOURT, "Yaourt avec opercule détaché"}};

const QMap<Dechet::Type, QString> Dechet::typeToImage{
    {Dechet::Type::ALIMENTAIRE, ":/images/dechets/alimentaire.png"},
    {Dechet::Type::BOCAL, ":/images/dechets/bocal.png"},
    {Dechet::Type::BOUTEILLE_VERRE, ":/images/dechets/bouteille_verre.png"},
    {Dechet::Type::CANETTE, ":/images/dechets/canette.png"},
    {Dechet::Type::COUVERTS_BOIS, ":/images/dechets/couverts_bois.png"},
    {Dechet::Type::COUVERTS_PLASTIQUE, ":/images/dechets/couverts_plastique.png"},
    {Dechet::Type::DOSETTE, ":/images/dechets/dosette.png"},
    {Dechet::Type::EPLUCHURES, ":/images/dechets/epluchures.png"},
    {Dechet::Type::GOBELET, ":/images/dechets/gobelet.png"},
    {Dechet::Type::MARC_CAFE, ":/images/dechets/marc_cafe.png"},
    {Dechet::Type::MASQUE, ":/images/dechets/masque.png"},
    {Dechet::Type::MOUCHOIR, ":/images/dechets/mouchoir.png"},
    {Dechet::Type::POSTIT, ":/images/dechets/postit.png"},
    {Dechet::Type::RESTES_REPAS, ":/images/dechets/restes_repas.png"},
    {Dechet::Type::SACHET_PLASTIQUE, ":/images/dechets/sachet_plastique.png"},
    {Dechet::Type::SACHET_THE, ":/images/dechets/sachet_the.png"},
    {Dechet::Type::STYLO, ":/images/dechets/stylo.png"},
    {Dechet::Type::YAOURT, ":/images/dechets/yaourt.png"}};

Dechet::Dechet(const Type type, Triselect &triselect)
    : QGraphicsItemGroup(nullptr), type(type), triselect(triselect),
      nom(typeToNom.value(type)),
      poubelle_choisie(nullptr), type_poubelle(typeToPoubelle.value(type)),
      feminin(false),
      pluriel(false),
      position(QRandomGenerator::global()->bounded(4)),
      pixmapItem(this),
      textItem(this),
      fondBlanc(this)
{
    pixmapItem.setPixmap(QPixmap(typeToImage.value(type)));
    pixmapItem.setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    pixmapItem.setScale(0.2);
    textItem.setPlainText(this->nom);
    textItem.setPos(pixmapItem.boundingRect().width() * pixmapItem.scale() / 2 - textItem.boundingRect().width() / 2,
                    pixmapItem.boundingRect().height() * pixmapItem.scale() + 5);
    // Creer un fond blanc
    fondBlanc.setBrush(Qt::white);
    fondBlanc.setZValue(-1); // Placer le fond blanc derrière le texte
    fondBlanc.setRect(textItem.boundingRect());
    fondBlanc.setPos(textItem.pos());
    hide();
    setFlag(GraphicsItemFlag::ItemIsMovable);
    setFlag(GraphicsItemFlag::ItemSendsScenePositionChanges);
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
        halfRect(imageSceneBoundingRect()));
}

void Dechet::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
    QGraphicsItemGroup::mousePressEvent(event);
}

void Dechet::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItemGroup::mouseMoveEvent(event);
}

void Dechet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    for (Poubelle &poubelle : triselect.poubelles)
    {
        if (touche(poubelle))
        {
            QPointF pos = event->scenePos();
            poubelle_choisie = &poubelle;
            triselect.dechet_jete(*this, pos);
        }
        poubelle.setSelected(false);
    }
    QGraphicsItemGroup::mouseReleaseEvent(event);
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
    return QGraphicsItemGroup::itemChange(change, value);
}
