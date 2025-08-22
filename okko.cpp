#include "okko.h"

Okko::Okko(QObject *parent, bool isOk)
    : QObject(parent), QGraphicsPixmapItem(), anim(this, "opacity", this), isOk(isOk)
{
    auto filename = QString(isOk ? ":/images/icones/coche_verte.png" : ":/images/icones/croix_rouge.png");
    this->setPixmap(QPixmap(filename));
    anim.setDuration(3000);
    anim.setStartValue(0.0);
    anim.setKeyValueAt(0.5, 1);
    anim.setEndValue(0.0);
    anim.setEasingCurve(QEasingCurve::OutBounce);
    this->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    this->setScale(0.3);
    this->setOpacity(0);
}

void Okko::start()
{
    qDebug()<< "start" << isOk;
    anim.start();
}
