#ifndef OKKO_H
#define OKKO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Okko : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    explicit Okko(QObject *parent = nullptr, bool isOk = true);
    void start();
private:
    QPropertyAnimation anim;
    const bool isOk;
};

#endif // OKKO_H
