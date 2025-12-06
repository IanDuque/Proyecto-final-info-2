#include "recolectables.h"
#include <QGraphicsScene>
#include <QDebug>

recolectables::recolectables(QObject *parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    velocidadY(0.0),
    aceleracionY(0.4)      // ajusta para que caiga más rápido/lento
{
    // Cargar sprite
    QPixmap px(":/Imagenes/bloque2.png");   // o el nombre que estés usando
    if (px.isNull()) {
        qDebug() << "Error cargando :/Imagenes/bloque2.png";
        px = QPixmap(40, 40);
        px.fill(Qt::yellow);
    }
    setPixmap(px.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Configurar timer interno
    connect(&timerMovimiento, &QTimer::timeout,
            this, &recolectables::actualizarMovimiento);
    timerMovimiento.start(16);
}

void recolectables::resetear(const QPointF &posicionInicial)
{
    setPos(posicionInicial);
    velocidadY = 0.0;
}

void recolectables::iniciarCaida()
{
    if (!timerMovimiento.isActive())
        timerMovimiento.start(16);
}

void recolectables::detenerCaida()
{
    timerMovimiento.stop();
}

void recolectables::actualizarMovimiento()
{
    // Caída uniformemente acelerada
    velocidadY += aceleracionY;
    setY(y() + velocidadY);

    if (!scene())
        return;

    // Si se sale por abajo, avisar y/o parar
    if (y() > scene()->height()) {
        emit salioDePantalla(this);
        detenerCaida();
    }
}
