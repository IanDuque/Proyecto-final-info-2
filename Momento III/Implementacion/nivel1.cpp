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

    connect(enemigo, &Personaje::vidaCambiada,
            this, &NivelBase::actualizarVidaEspaniol);

    // Detenemos spawn de obstáculos del nivel anterior porque este es de pelea 1v1
    if (timerSpawn) timerSpawn->stop();
    if (timerLoop) timerLoop->stop(); // No necesitamos loop de scroll
}

void nivel1::inicializarJugador()
{
    Indio *player = new Indio();

    jugador = player;

    connect(player, &Indio::vidaCambiada, this, [this](int nuevaVida){
    emit actualizarHUD(tiempoRestante, 0, nuevaVida);
    });

    jugador->setPos(50, 430);

    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    player->setControlEnabled(true);

    addItem(jugador);
    jugador->setFocus();
    connect(player, &Personaje::vidaCambiada,
            this, &NivelBase::actualizarVidaIndio);
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
void nivel1::onJugadorMuere()
{
    if (juegoTerminado) return;
    juegoTerminado = true;

    // Detener timers por seguridad
    if (timerLoop)   timerLoop->stop();
    if (timerSpawn)  timerSpawn->stop();
    if (timerSecond) timerSecond->stop();

    // Texto principal: derrota
    QGraphicsTextItem *loseText = new QGraphicsTextItem(QString::fromUtf8("¡Has sido derrotado!"));
    QFont fontLose("Arial", 30, QFont::Bold);
    loseText->setFont(fontLose);
    loseText->setDefaultTextColor(Qt::red);
    loseText->setPos(160, 250);
    loseText->setZValue(20);
    addItem(loseText);

    // Texto secundario: volver al menú
    QGraphicsTextItem *mensajeextra = new QGraphicsTextItem(QString::fromUtf8("Presiona Esc para volver al menú principal."));
    QFont fontMensaje("Arial", 20, QFont::Bold);
    mensajeextra->setFont(fontMensaje);
    mensajeextra->setDefaultTextColor(Qt::black);
    mensajeextra->setPos(100, 320);
    mensajeextra->setZValue(20);
    addItem(mensajeextra);
}

void nivel1::onEnemigoMuere()
{
    if (juegoTerminado) return;
    juegoTerminado = true;

    if (timerLoop)   timerLoop->stop();
    if (timerSpawn)  timerSpawn->stop();
    if (timerSecond) timerSecond->stop();

    // Texto principal: victoria
    QGraphicsTextItem *winText = new QGraphicsTextItem(QString::fromUtf8("¡Has derrotado al enemigo!"));
    QFont fontWin("Arial", 30, QFont::Bold);
    winText->setFont(fontWin);
    winText->setDefaultTextColor(Qt::green);
    winText->setPos(120, 250);
    winText->setZValue(20);
    addItem(winText);

    // Texto secundario
    QGraphicsTextItem *mensajeextra = new QGraphicsTextItem(QString::fromUtf8("Presiona Esc para volver al menú principal."));
    QFont fontMensaje("Arial", 20, QFont::Bold);
    mensajeextra->setFont(fontMensaje);
    mensajeextra->setDefaultTextColor(Qt::black);
    mensajeextra->setPos(100, 320);
    mensajeextra->setZValue(20);
    addItem(mensajeextra);
}
