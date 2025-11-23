#include "nivelbase.h"
#include <QDebug>

NivelBase::NivelBase(QObject *parent)
    : QGraphicsScene(parent)
{
    // Tamaño de escena fijo para todos los niveles
    setSceneRect(0, 0, 400, 600);

    // Inicialización universal
    velocidadFondo = 5;
    tiempoRestante = 120; // 2 minutos por defecto
    juegoTerminado = false;

    // 1. Configurar Carro
    jugador = new Carro();
    jugador->setPos(175, 500); // Posición correcta (cerca del fondo de la pantalla)
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(jugador);

    // 2. Configurar el fondo (¡Llamada a la clase hija!)
    // Esta llamada debe inicializar fondo1 y fondo2, y añadirlos a la escena.
    // **IMPORTANTE: Si no llamas a configurarFondo() aquí o en el constructor de Nivel3, el fondo nunca se cargará.**
    //configurarFondo();

    // 3. Configurar Timers
    timerLoop = new QTimer(this);
    connect(timerLoop, &QTimer::timeout, this, &NivelBase::gameLoop);
    timerLoop->start(16);

    timerSpawn = new QTimer(this);
    connect(timerSpawn, &QTimer::timeout, this, &NivelBase::spawnObstacle);
    timerSpawn->start(1500);

    timerSecond = new QTimer(this);
    connect(timerSecond, &QTimer::timeout, this, &NivelBase::updateTimer);
    timerSecond->start(1000);

    // Los fondos (fondo1 y fondo2) deben tener Z-Value bajo.
    if (fondo1) fondo1->setZValue(-1);
    if (fondo2) fondo2->setZValue(-1);

}
NivelBase::~NivelBase()
{
    // Detener timers para evitar problemas de destrucción
    timerLoop->stop();
    timerSpawn->stop();
    timerSecond->stop();
}

void NivelBase::gameLoop()
{
    if (juegoTerminado) return;

    // A. MOVER FONDO (SCROLLING)
    fondo1->setPos(0, fondo1->y() + velocidadFondo);
    fondo2->setPos(0, fondo2->y() + velocidadFondo);

    // Resetear posición
    if (fondo1->y() >= 600) fondo1->setPos(0, -600);
    if (fondo2->y() >= 600) fondo2->setPos(0, -600);

    // B. MANEJAR OBSTÁCULOS Y COLISIONES
    for (int i = 0; i < obstaculos.size(); ++i) {
        Obstaculo *obs = obstaculos[i];
        obs->mover(velocidadFondo);

        if (jugador->collidesWithItem(obs)) {
            // 1. Penalización
            if (velocidadFondo > 2) {
                velocidadFondo -= 1;
            }
            // Emitir señal para que MainWindow actualice el HUD
            emit actualizarHUD(tiempoRestante, velocidadFondo * 20);

            // 2. Crear Explosión
            Explosion *boom = new Explosion();
            boom->setPos(obs->pos());
            addItem(boom);

            // 3. Eliminar obstáculo
            removeItem(obs);
            obstaculos.removeAt(i);
            delete obs;
            i--;
            continue;
        }

        // 4. Eliminar si sale de pantalla
        if (obs->y() > 600) {
            removeItem(obs);
            obstaculos.removeAt(i);
            delete obs;
            i--;
        }
    }
}

void NivelBase::spawnObstacle()
{
    if (juegoTerminado) return;

    // Usar la ruta de obstáculo específica de la clase hija
    Obstaculo *obs = new Obstaculo(getObstaculoPath());
    addItem(obs);
    obstaculos.append(obs);
}

void NivelBase::updateTimer()
{
    if (juegoTerminado) return;

    tiempoRestante--;
    emit actualizarHUD(tiempoRestante, velocidadFondo * 20);

    if (tiempoRestante <= 0) {
        juegoTerminado = true;
        timerLoop->stop();
        timerSpawn->stop();
        timerSecond->stop();

        QGraphicsTextItem *winText = new QGraphicsTextItem("¡NIVEL COMPLETADO!");
        winText->setFont(QFont("Arial", 24, QFont::Bold));
        winText->setDefaultTextColor(Qt::green);
        winText->setPos(50, 250);
        winText->setZValue(20);
        addItem(winText);

        // Señal para que MainWindow cambie de nivel
        QTimer::singleShot(2000, this, [this](){ emit nivelTerminado(); });
    }
}
