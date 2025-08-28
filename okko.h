#ifndef OKKO_H
#define OKKO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "dechet.h"

class Okko : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    explicit Okko(QObject *parent = nullptr, bool isOk = true);
    void start();
    Dechet * dechet;
private slots:
    void onAnimFinished();
private:
    QPropertyAnimation anim;
    const bool isOk;

};

#endif // OKKO_H
