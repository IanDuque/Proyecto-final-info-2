#include "indio.h"

indio::indio(short int vida_inicial, short int defensa_inicial, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), vida(vida_inicial), defensa(defensa_inicial)
{
    cargarSprite(":/Imagenes/indio1.png");
}

// Getters y setters.

short int indio::getdefensa() const
{
    return defensa;
}

void indio::setdefensa(short int cambiodefensa)
{
    defensa = cambiodefensa;
}

short int indio::getvida() const
{
    return vida;
}

void indio::setvida(short int daniorecibido)
{
    // Restar el daño recibido
    vida -= daniorecibido;

    // Asegurar que la vida no sea negativa
    if (vida < 0) {
        vida = 0;
    }
}

void indio::cargarSprite(const QString& rutaSprite)
{
    QPixmap sprite(rutaSprite);

    if (sprite.isNull()) {
        qCritical() << "ERROR: No se pudo cargar el sprite del Indio en:" << rutaSprite;
    } else {
        // Establece el sprite y lo escala.
        setPixmap(sprite.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Permite la detección de colisiones si es necesario
        setShapeMode(BoundingRectShape);
    }
}
void indio::setControlEnabled(bool enable) {
    controlEnabled = enable;
}
