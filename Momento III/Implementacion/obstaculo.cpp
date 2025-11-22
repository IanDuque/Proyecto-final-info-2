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
    setPixmap(pixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 2. Posición Aleatoria
    int randomX = QRandomGenerator::global()->bounded(0, 340);
    setPos(randomX, -100);

    // 3. Velocidad propia aleatoria
    velocidadPropia = QRandomGenerator::global()->bounded(2, 5);
}

void Obstaculo::mover(int velocidadEscenario)
{
    setPos(x(), y() + velocidadEscenario + velocidadPropia);
}
