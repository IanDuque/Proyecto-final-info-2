#include "nivel1.h"
#include "indio.h"
#include "espaniol.h"

nivel1::nivel1(QObject *parent)
    : NivelBase(parent)
{
    velocidadFondo = 0; // Nivel estático (sin scroll)
    configurarFondo();

    // Inicializar jugador (Indio)
    inicializarJugador();

    // Crear al enemigo (El español)
    Espaniol *enemigo = new Espaniol();
    enemigo->setPos(650, 440);        // A la derecha de la pantalla
    addItem(enemigo);

    espanol = enemigo;

    // HUD: vida del español
    connect(espanol, &Personaje::vidaCambiada,
            this, &NivelBase::actualizarVidaEspaniol);

    // Creamos el sprite del español muerto, pero oculto
    {
        QPixmap espaniolderrotado(":/Imagenes/espaniolderrotado.png");
        if (espaniolderrotado.isNull()) {
            espaniolderrotado = QPixmap(120, 120);
            espaniolderrotado.fill(Qt::red);
        }
        espaniolderrotado = espaniolderrotado.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        spriteEspaniolMuerto = new QGraphicsPixmapItem(espaniolderrotado);
        spriteEspaniolMuerto->setVisible(false);
        spriteEspaniolMuerto->setZValue(1);

        // En esta posicion se va a mostrar el sprite del espanñol al ser derrotado
        spriteEspaniolMuerto->setPos(650, 500);
        addItem(spriteEspaniolMuerto);
    }

    // Detenemos spawn de obstáculos del nivel anterior porque este es de pelea 1v1
    if (timerSpawn) timerSpawn->stop();
    if (timerLoop)  timerLoop->stop(); // No necesitamos loop de scroll

    // Conexión al morir (Indio)
    if (indio) {
        connect(indio, &Personaje::murio, this, &nivel1::onPersonajeMuerto);
    }

    // Conexión al morir (Español)
    if (espanol) {
        connect(espanol, &Personaje::murio, this, &nivel1::onPersonajeMuerto);
    }
}

void nivel1::inicializarJugador()
{
    Indio *player = new Indio();

    indio   = player;
    jugador = player;

    connect(player, &Indio::vidaCambiada, this, [this](int nuevaVida){
    emit actualizarHUD(tiempoRestante, 0, nuevaVida);
    });

    jugador->setPos(50, 430);

    jugador->setFlag(QGraphicsItem::ItemIsFocusable);

    player->setControlEnabled(true);

    addItem(jugador);
    jugador->setFocus();
    connect(player, &Personaje::vidaCambiada, this, &NivelBase::actualizarVidaIndio);
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

    //para regresar automaticamente al menu tras 3 segundos de acabar el nivel.
    QTimer::singleShot(3000, this, [this](){ emit nivelTerminado(); });
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

    //para regresar automaticamente al menu tras 3 segundos de acabar el nivel.
    QTimer::singleShot(3000, this, [this](){ emit nivelTerminado(); });
}

void nivel1::onPersonajeMuerto(Personaje *p)
{
    if (batallaTerminada) return; // por si llega doble
    batallaTerminada = true;

    // Detener cualquier timer global por si acaso
    if (timerLoop)   timerLoop->stop();
    if (timerSpawn)  timerSpawn->stop();
    if (timerSecond) timerSecond->stop();

    // Detener acciones propias de cada personaje
    if (indio)   indio->detenerAcciones();
    if (espanol) espanol->detenerAcciones();

    // --- Si el que murió es el Indio ---
    if (auto jugador = qobject_cast<Indio*>(p)) {
        QPixmap indiomuerto(":/Imagenes/indioderrotado.png");
        if (!indiomuerto.isNull()) {
            jugador->setPixmap(
                indiomuerto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                );
        }

        onJugadorMuere();
    }
    // --- Si el que murió es el Español ---
    else if (qobject_cast<Espaniol*>(p)) {

        // Ocultar el sprite normal del español
        if (espanol)
            espanol->setVisible(false);

        // Mostrar el sprite ya preparado del español muerto
        if (spriteEspaniolMuerto)
            spriteEspaniolMuerto->setVisible(true);

        // Mostrar mensaje de victoria
        onEnemigoMuere();
    }
}
