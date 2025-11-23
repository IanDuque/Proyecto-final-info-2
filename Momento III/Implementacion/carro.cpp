#include "carro.h"
#include <QDebug>
#include <algorithm> // Necesario para std::max y std::min

Carro::Carro(QObject *parent) : QObject(parent)
{
    // RUTA A REVISAR: ¡Debe ser exacta!
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
    // CRÍTICO: Si el control no está habilitado (Nivel 1 y 2), ignora la tecla.
    if (!controlEnabled) {
        event->ignore();
        return;
    }
    // Manejo de teclas:
    // Aseguramos que el movimiento se mantenga dentro del área de la carretera (aprox 50 a 300 de X)
    qreal newX = x();
    // Definimos las constantes como qreal para evitar el error de tipos en std::max/min
    const qreal MIN_X = 290.0;  // Límite izquierdo de la carretera
    const qreal MAX_X = 455.0; // Límite derecho de la carretera (Ancho de la escena es 800, carro es 60)

    if (event->key() == Qt::Key_Left) {
        // Corrección: Usamos std::max con newX (qreal) y MIN_X (qreal)
        newX = std::max(MIN_X, newX - velocidadLateral);
    } else if (event->key() == Qt::Key_Right) {
        // Corrección: Usamos std::min con newX (qreal) y MAX_X (qreal)
        newX = std::min(MAX_X, newX + velocidadLateral);
    }

    setX(newX);
}
