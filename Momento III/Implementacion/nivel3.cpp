#include "nivel3.h"

Nivel3::Nivel3(QObject *parent)
    : NivelBase(parent)
{
    // Llama a la configuración específica de este nivel
    configurarFondo();
    // Asegura que el jugador tenga el foco al iniciar el nivel
    jugador->setFocus();
}

// 1. Define la ruta del fondo para este nivel (Desierto)
QString Nivel3::getFondoPath() const
{
    return ":/Imagenes/fondoNivel3.png";
}

// 2. Define la base de la ruta de los obstáculos (rocas)
QString Nivel3::getObstaculoPath() const
{
    // La clase Obstaculo tendrá que generar obstaculo1.png, 2.png, etc.
    return ":/imagenes/obstaculo";
}

// 3. Lógica específica de la creación del fondo
void Nivel3::configurarFondo()
{
    QPixmap bg(getFondoPath());
    bg = bg.scaled(400, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondo1 = new QGraphicsPixmapItem(bg);
    fondo1->setPos(0, 0);
    addItem(fondo1);

    fondo2 = new QGraphicsPixmapItem(bg);
    fondo2->setPos(0, -600);
    addItem(fondo2);
}
