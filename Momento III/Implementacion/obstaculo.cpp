#include "obstaculo.h"
#include <QRandomGenerator>

// Recibe la ruta base (basePath, ej: ":/imagenes/obstaculo")
Obstaculo::Obstaculo(const QString &basePath, QObject *parent) : QObject(parent)
{
    // 1. Seleccionar imagen aleatoria (basePath + 1.png, 2.png, 3.png)
    //int tipoObstaculo = QRandomGenerator::global()->bounded(1, 4);
    // Combina la ruta base (ej: ":/imagenes/roca") con el número (ej: 1) y la extensión (.png)
    QString nombreImagen = ":/Imagenes/obstaculo3.png";

    QPixmap pixmap(nombreImagen);

    // Si la imagen no carga, crear un cuadro café de respaldo
    if (pixmap.isNull()) {
        pixmap = QPixmap(60, 60);
        pixmap.fill(QColor("#5d4037"));
    }

    // --- ESCALAR OBSTÁCULOS ---
    setPixmap(pixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 2. Posición Aleatoria
    int randomX = QRandomGenerator::global()->bounded(290, 455);
    setPos(randomX, -100);

    // GUARDAMOS LA X ORIGINAL
    xOriginal = randomX;
    esSenoidal = false; // Por defecto es recto

    // 3. Velocidad propia aleatoria
    velocidadPropia = QRandomGenerator::global()->bounded(2, 5);
}
void Obstaculo::setMovimientoSenoidal(bool activar)
{
    esSenoidal = activar;
    // Si ya bajó un poco y se activa el modo, actualizamos xOriginal
    // para que no salte bruscamente, o lo dejamos basado en el spawn.
    // Para simplificar, usamos el del spawn.
}

void Obstaculo::mover(int velocidadEscenario)
{
    int nuevaY = y() + velocidadEscenario + velocidadPropia;
    int nuevaX = x();

    if (esSenoidal) {
        // Fórmula: x = Centro + Amplitud * sin(Frecuencia * Y)
        // Amplitud 40: Se mueve 40px a la izquierda y 40px a la derecha
        // Frecuencia 0.02: Define qué tan cerradas son las curvas
        nuevaX = xOriginal + 40 * qSin(nuevaY * 0.02);
    }

    setPos(nuevaX, nuevaY);
}
