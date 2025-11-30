#include "proyectil.h"
#include <QtMath> // Para qSin, qCos
#include "personaje.h"
#include <QList>
#include "indio.h"
#include "espaniol.h"

Proyectil::Proyectil(double xIni, double yIni, double angulo, double velocidad, int tipo, QObject *parent)
    : QObject(parent), tipoProyectil(tipo)
{
    QPixmap imagen;

    if (tipo == 1) { // Lanza del Indio
        // Lanza (472x103) - Es muy ancha, la escalamos
        imagen.load(":/Imagenes/lanza.png");
        setPixmap(imagen.scaled(80, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else { // Granada/Bala del Español
        // Granada (17x27)
        imagen.load(":/Imagenes/granada.png"); // Ojo con el nombre "grananda" vs "granada"
        setPixmap(imagen.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    x = xIni;
    y = yIni;

    // ... resto del código de física (grados a radianes, etc.) igual ...
    double rad = qDegreesToRadians(angulo);
    vx = velocidad * qCos(rad);
    vy = -velocidad * qSin(rad);
    gravedad = 1.8;
    setPos(x, y);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Proyectil::actualizarPosicion);
    timer->start(20);
}

void Proyectil::actualizarPosicion()
{
    // FÍSICA: Movimiento parabólico
    // x = x + vx * t
    // vy = vy + g * t
    // y = y + vy * t

    x += vx;
    vy += gravedad; // La gravedad aumenta la velocidad de caída
    y += vy;

    setPos(x, y);

    // Rotación visual (opcional, para que la lanza apunte a donde va)
    if (tipoProyectil == 1) {
        setRotation(qRadiansToDegrees(qAtan2(vy, vx)));
    }

    // --- COLISIONES ---
    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem *item : colisiones) {

        // Si soy lanza (tipo 1) y choco con Español
        if (tipoProyectil == 1) {
            Espaniol *enemigo = dynamic_cast<Espaniol*>(item);
            if (enemigo) {
                enemigo->recibirDanio(20); // Daño fijo o usar clase Armas
                delete this; // Eliminar proyectil
                return;
            }
        }

        // Si soy bala (tipo 2) y choco con Indio
        if (tipoProyectil == 2) {
            Indio *jugador = dynamic_cast<Indio*>(item);
            if (jugador) {
                jugador->recibirDanio(15);
                delete this;
                return;
            }
        }
    }

    // Eliminar si sale de la pantalla (suelo o lados)
    if (y > 600 || x > 800 || x < 0) {
        delete this;
    }
}
