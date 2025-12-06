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
    void detenerAcciones();
    void morir();

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

    QPixmap spriteDerrotado;
    bool muerto;


    // Cańón
    QGraphicsPixmapItem *canon;
    QPixmap spriteCanonApuntando;
    QPixmap spriteCanonDisparando;
    bool disparandoCanon;

signals:
    void enemigoderrotado();

};

#endif // ESPANIOL_H
