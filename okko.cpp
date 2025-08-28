#include "okko.h"

Okko::Okko(QObject *parent, bool isOk)
    : QObject(parent), QGraphicsPixmapItem(), anim(this, "opacity", this), isOk(isOk), dechet(nullptr)
{
    auto filename = QString(isOk ? ":/images/icones/coche_verte.png" : ":/images/icones/croix_rouge.png");
    this->setPixmap(QPixmap(filename));
    anim.setDuration(800);
    anim.setStartValue(0.0);
    anim.setKeyValueAt(0.3, 1.0);
    anim.setEndValue(1.0);
    anim.setEasingCurve(QEasingCurve::OutCurve);
    this->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
    this->setScale(0.3);
    connect(&anim, &QPropertyAnimation::finished, this, &Okko::onAnimFinished);
}

void Okko::start()
{
    qDebug()<< "start" << isOk;
    anim.start();
}

void Okko::onAnimFinished()
{
    qDebug()<< "finished" << isOk;
    this->setOpacity(0);
    if (this->dechet) {
        this->dechet->setEnabled(true);
    }
    this->dechet = nullptr;
}
