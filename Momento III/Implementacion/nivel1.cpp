#include "nivel1.h"
#include "indio.h"

nivel1::nivel1(QObject *parent)
    : NivelBase(parent)
{
    // Llama a la configuración específica de este nivel
    velocidadFondo = 0;
    configurarFondo();
    inicializarJugador();

    if (timerSpawn) {
        timerSpawn->stop();
    }
}

void nivel1::inicializarJugador()
{
    indio *player = new indio();
    jugador = player; // Asignamos el indio al puntero base

    jugador->setPos(10, 430);
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(jugador);
    jugador->setFocus();
}

QString nivel1::getFondoPath() const
{
    return ":/Imagenes/fondonivel1.png";
}

void nivel1::configurarFondo()
{
    QPixmap bg(getFondoPath());

    if (bg.isNull()) {
        qCritical() << "ERROR: Fondo Nivel 1 no cargado.";
        bg = QPixmap(800, 600);
        bg.fill(Qt::darkCyan);
    }

    bg = bg.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondo1 = new QGraphicsPixmapItem(bg);
    fondo1->setPos(0, 0);
    addItem(fondo1);
}
