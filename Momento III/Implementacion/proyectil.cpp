#include "proyectil.h"
#include <QtMath>
#include <QGraphicsScene>
#include <QList>

#include "indio.h"
#include "espaniol.h"

Proyectil::Proyectil(double xIni, double yIni, double angulo, double velocidad, int tipo, QObject *parent)
    : QObject(parent), tipoProyectil(tipo)
{
    QPixmap imagen;

    // Cargar sprite según tipo
    if (tipo == 1) {
        imagen.load(":/Imagenes/lanza.png");
        setPixmap(imagen.scaled(100, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (tipo == 2) {
        imagen.load(":/Imagenes/bolacanon.png");
        setPixmap(imagen.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (tipo == 3) {
        imagen.load(":/Imagenes/bolacanon.png");  // usamos el mismo sprite pero con un reescalado mas pequeño
        setPixmap(imagen.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Inicializar posición
    x = xIni;
    y = yIni;
    setPos(x, y);

    // Convertir ángulo a radianes
    double rad = qDegreesToRadians(angulo);

    // Velocidades iniciales
    vx = velocidad * qCos(rad);
    vy = -velocidad * qSin(rad);

    // Gravedad
    if (tipo == 1) {
        gravedad = 1.0;
    } else if (tipo == 2) {
        gravedad = 1.0;
    } else {
        gravedad = 0.0;
    }

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Proyectil::actualizarPosicion);
    timer->start(20);
}

void Proyectil::actualizarPosicion()
{
    // -------------------------------------
    // Movimiento rectilíneo
    // -------------------------------------
    if (tipoProyectil == 3) {
        x += vx;
        setPos(x, y);

        // Eliminar fuera de pantalla
        if (x < 0 || x > 800)
            delete this;

        // Colisiones
        QList<QGraphicsItem*> colisiones = collidingItems();
        for (QGraphicsItem *item : colisiones) {
            Indio *jugador = dynamic_cast<Indio*>(item);
            if (jugador) {
                jugador->recibirDanio(10);
                delete this;
                return;
            }
        }

        return;
    }

    // -------------------------------------
    // Movimiento parabólico
    // -------------------------------------
    x += vx;
    vy += gravedad;
    y += vy;

    setPos(x, y);

    // Rotación solo para lanza
    if (tipoProyectil == 1)
        setRotation(qRadiansToDegrees(qAtan2(vy, vx)));

    // Colisiones con personajes
    QList<QGraphicsItem*> colisiones = collidingItems();

    for (QGraphicsItem *item : colisiones) {

        // Lanza golpea al Español
        if (tipoProyectil == 1) {
            Espaniol *enemigo = dynamic_cast<Espaniol*>(item);
            if (enemigo) {
                enemigo->recibirDanio(20);

                // avisa que la lanza ya no existe
                QList<QGraphicsItem*> lista = scene()->items();
                for (QGraphicsItem *it : lista) {
                    Indio *indio = dynamic_cast<Indio*>(it);
                    if (indio) {
                        indio->setLanzaLibre();
                        break;
                    }
                }
                delete this;
                return;
            }
        }

        // Bala de cañón golpea al Indio
        if (tipoProyectil == 2) {
            Indio *jugador = dynamic_cast<Indio*>(item);
            if (jugador) {
                jugador->recibirDanio(20);
                delete this;
                return;
            }
        }
    }

    // Eliminar fuera de pantalla
    if (y > 600 || x < 0 || x > 800){
        if (tipoProyectil == 1) {
            // buscar al Indio en la escena
            QList<QGraphicsItem*> items = scene()->items();
            for (QGraphicsItem *it : items) {
                Indio *indio = dynamic_cast<Indio*>(it);
                if (indio) {
                    indio->setLanzaLibre();
                    break;
                }
            }
        }
        delete this;
        return;
    }
}
