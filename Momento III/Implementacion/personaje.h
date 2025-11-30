#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Personaje(int vidaInicial, QObject *parent = nullptr);

    int getVida() const;
    void recibirDanio(int danio);
    bool estaVivo() const;

protected:
    int vida;
    int vidaMax;
};

#endif // PERSONAJE_H
