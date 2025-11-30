#include "nivel1.h"
#include "indio.h"
#include"espaniol.h"

nivel1::nivel1(QObject *parent)
    : NivelBase(parent)
{
    velocidadFondo = 0; // Nivel estático (sin scroll)
    configurarFondo();

    // Como NivelBase tiene un puntero 'jugador', lo usamos
    inicializarJugador();

    // Crear al enemigo (El español)
    Espaniol *enemigo = new Espaniol();
    enemigo->setPos(650, 430); // A la derecha de la pantalla
    addItem(enemigo);

    // Detenemos spawn de obstáculos del nivel anterior porque este es de pelea 1v1
    if (timerSpawn) timerSpawn->stop();
    if (timerLoop) timerLoop->stop(); // No necesitamos loop de scroll
}

void nivel1::inicializarJugador()
{
    // 1. Crear instancia usando 'indio' (minúscula, como tu clase)
    Indio *player = new Indio();

    // 2. Asignar al puntero 'jugador' heredado de NivelBase
    jugador = player;

    // 3. Posición inicial (X=50 para dar margen izquierdo, Y=430 altura del suelo)
    jugador->setPos(50, 430);

    // 4. CRÍTICO: Permitir que el objeto reciba eventos de teclado de Qt
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    // 5. CRÍTICO: Activar tu lógica interna de movimiento (variable bool controlEnabled)
    player->setControlEnabled(true);

    // 6. Añadir a la escena y forzar el foco inmediatamente
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
