#ifndef ESPANIOL_H
#define ESPANIOL_H

#include "personaje.h"
#include <QTimer>
#include <QPixmap>
#include <QList>
#include <QGraphicsPixmapItem>

class Espaniol : public Personaje
{
    Q_OBJECT
public:
    explicit Espaniol(QObject *parent = nullptr);

    void recibirDanio(int danio);

private slots:
    void disparar();
    void actualizarAnimacion();
    void finalizarDisparo();

private:
    QTimer *timerAtaque;
    QTimer *timerAnimacion;

    // Sprites del soldado
    QPixmap spriteQuieto;
    QList<QPixmap> framesDisparar;

    int frameActual;
    bool atacando;
    bool usandoCanon;
    int reduccionDanio;

    // Cańón
    QGraphicsPixmapItem *canon;
    QPixmap spriteCanonApuntando;
    QPixmap spriteCanonDisparando;
    bool disparandoCanon;
};

#endif // ESPANIOL_H
