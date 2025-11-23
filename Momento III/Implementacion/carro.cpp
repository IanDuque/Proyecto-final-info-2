#include "carro.h"
#include <QDebug>
#include <algorithm> // Necesario para std::max y std::min

<<<<<<< HEAD
//constructor.
carro:: carro(float velx, float vely){
    setvelx(velx);
    setvely(vely);
}
//metodos.
void carro:: aceleracion(short int metros, short int segundos){
    vely=metros*(segundos*segundos); //formula de aceleracion uniforme (ms^2).
=======
Carro::Carro(QObject *parent) : QObject(parent)
{
    // RUTA A REVISAR: ¡Debe ser exacta!
    QPixmap pixmap(":/Imagenes/carro.png");

    if (pixmap.isNull()) {
        qWarning() << "ERROR: No se pudo cargar la imagen del carro. Usando color de respaldo.";
        // Fallback: Si no carga, crea un cuadrado azul visible
        QPixmap error(50, 80);
        error.fill(Qt::blue); // Azul para que destaque sobre el fondo
        setPixmap(error);
    } else {
        // Escalar la imagen a un tamaño razonable para el juego (ej. 50x80)
        QPixmap scaledPixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    // El carro debe estar en una capa superior (Z=1) al fondo (Z=0)
    setZValue(1);

    // Inicializar velocidad lateral
    velocidadLateral = 5; // Velocidad de movimiento lateral
}

void Carro::keyPressEvent(QKeyEvent *event)
{
    // Manejo de teclas:
    // Aseguramos que el movimiento se mantenga dentro del área de la carretera (aprox 50 a 300 de X)
    qreal newX = x();
    // Definimos las constantes como qreal para evitar el error de tipos en std::max/min
    const qreal MIN_X = 50.0;  // Límite izquierdo de la carretera
    const qreal MAX_X = 300.0; // Límite derecho de la carretera (Ancho de la escena es 400, carro es 50)

    if (event->key() == Qt::Key_Left) {
        // Corrección: Usamos std::max con newX (qreal) y MIN_X (qreal)
        newX = std::max(MIN_X, newX - velocidadLateral);
    } else if (event->key() == Qt::Key_Right) {
        // Corrección: Usamos std::min con newX (qreal) y MAX_X (qreal)
        newX = std::min(MAX_X, newX + velocidadLateral);
    }

    setX(newX);
>>>>>>> origin/Angel
}
