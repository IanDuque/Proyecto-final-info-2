#include "carro.h"
#include <QDebug>
#include <algorithm> // Necesario para std::max y std::min

Carro::Carro(QObject *parent) : QObject(parent)
{
    QPixmap pixmap(":/Imagenes/carro.png");

    if (pixmap.isNull()) {
        qWarning() << "ERROR: No se pudo cargar la imagen del carro. Usando color de respaldo.";
        // Fallback: Si no carga, crea un cuadrado azul visible
        QPixmap error(70, 70);
        error.fill(Qt::blue); // Azul para que destaque sobre el fondo
        setPixmap(error);
    } else {
        // Escalar la imagen a un tamaño razonable para el juego (ej. 80x80)
        QPixmap scaledPixmap = pixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    // El carro debe estar en una capa superior (Z=1) al fondo (Z=0)
    setZValue(1);

    // Inicializar velocidad lateral
    velocidadLateral = 10; // Velocidad de movimiento lateral
}

void Carro::setControlEnabled(bool enable) {
    controlEnabled = enable;
}

void Carro::keyPressEvent(QKeyEvent *event)
{
    if (!controlEnabled) {
        event->ignore();
        return;
    }

    qreal newX = x();

    const qreal MIN_X = 290.0;
    const qreal MAX_X = 455.0;

    // Cambiamos flechas → A y D
    if (event->key() == Qt::Key_A) {
        newX = std::max(MIN_X, newX - velocidadLateral);
    }
    else if (event->key() == Qt::Key_D) {
        newX = std::min(MAX_X, newX + velocidadLateral);
    }

    setX(newX);
}
