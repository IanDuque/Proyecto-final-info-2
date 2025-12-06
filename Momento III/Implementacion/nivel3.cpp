#include "nivel3.h"
#include "carro.h"

Nivel3::Nivel3(QObject *parent)
    : NivelBase(parent)
{
    configurarFondo();
    inicializarJugador();
}

void Nivel3::inicializarJugador()
{
    Carro *player = new Carro();
    jugador = player; // Asignamos el Carro al puntero base
    player->setControlEnabled(true);

    jugador->setPos(360, 500);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(jugador);
    jugador->setFocus();
}

QString Nivel3::getFondoPath() const
{
    return ":/Imagenes/fondoNivel3.png";
}

QString Nivel3::getObstaculoPath() const
{
    return ":/Imagenes/obstaculo";
}

void Nivel3::configurarFondo()
{
    QPixmap bg(getFondoPath());

    if (bg.isNull()) {
        qCritical() << "ERROR: Fondo Nivel 3 no cargado.";
        // Fallback
        bg = QPixmap(800, 600);
        bg.fill(Qt::darkYellow);
    }

    bg = bg.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondo1 = new QGraphicsPixmapItem(bg);
    fondo1->setPos(0, 0);
    addItem(fondo1);

    // Fondo para scrolling (se asume que el fondo es de 600px de alto)
    fondo2 = new QGraphicsPixmapItem(bg);
    fondo2->setPos(0, -600);
    addItem(fondo2);
}
