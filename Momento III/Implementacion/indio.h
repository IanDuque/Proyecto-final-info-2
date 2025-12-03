#ifndef INDIO_H
#define INDIO_H

#include "personaje.h"
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QList>

class Indio : public Personaje
{
    Q_OBJECT

public:
    explicit Indio(int vida_inicial = 100, QObject *parent = nullptr);

    void actualizarAnimacion();
    void setControlEnabled(bool enable);

    void recibirDanio(int valor);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void disparar();
    void setLanzaLibre();

private:
    bool controlEnabled;
    bool lanzaEnAire;

    // Animación
    QTimer *timerAnimacion;

    QPixmap spriteQuieto;
    QList<QPixmap> framesCorrer;

    int frameActual;

    // Estado de teclas (movimiento)
    bool arriba;
    bool abajo;
    bool izquierda;
    bool derecha;
};

#endif // INDIO_H
