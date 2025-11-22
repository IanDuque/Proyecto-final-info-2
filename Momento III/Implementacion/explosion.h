#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Explosion(QObject *parent = nullptr);
};

#endif // EXPLOSION_H
