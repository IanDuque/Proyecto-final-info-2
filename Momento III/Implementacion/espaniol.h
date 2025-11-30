#ifndef ESPANIOL_H
#define ESPANIOL_H

#include "personaje.h"
#include <QTimer>

class Espaniol : public Personaje
{
    Q_OBJECT
public:
    explicit Espaniol(QObject *parent = nullptr);

private slots:
    void disparar();
    void actualizarAnimacion();
    void finalizarDisparo(); // Para volver a estado quieto

private:
    QTimer *timerAtaque;
    QTimer *timerAnimacion;

    QPixmap spriteSheetQuieto;
    QPixmap spriteSheetDisparando;
    QPixmap *spriteActual;

    int frameActual;
    int numFrames;
    int anchoFrame;
    int altoFrame;
    bool atacando;
};

#endif // ESPANIOL_H
