#include "nivelbase.h"
#include <QDebug>
#include <QFont>
#include "carro.h" // Incluir aquí para la colisión

NivelBase::NivelBase(QObject *parent)
    : QGraphicsScene(parent), jugador(nullptr) // Inicializa jugador a nullptr (seguridad)
{
    setSceneRect(0, 0, 800, 600);

    velocidadFondo = 5;
    tiempoRestante = 15; //el tiempo que usaremos sera 120 pero a efectos de prueba sera 15.
    juegoTerminado = false;

    // Configurar Timers, PERO NO INICIARLOS AQUÍ (iniciarlos en MainWindow)
    timerLoop = new QTimer(this);
    connect(timerLoop, &QTimer::timeout, this, &NivelBase::gameLoop);

    timerSpawn = new QTimer(this);
    connect(timerSpawn, &QTimer::timeout, this, &NivelBase::spawnObstacle);

    timerSecond = new QTimer(this);
    connect(timerSecond, &QTimer::timeout, this, &NivelBase::updateTimer);
}

// Nuevo método para iniciar los timers una vez que el nivel esté listo
void NivelBase::iniciarTimers()
{
    timerLoop->start(16);
    timerSpawn->start(1500);
    timerSecond->start(1000);
}

NivelBase::~NivelBase()
{
    // Detener los Timers
    if (timerLoop) {
        timerLoop->stop();
        delete timerLoop;
        timerLoop = nullptr;
    }
    if (timerSpawn) {
        timerSpawn->stop();
        delete timerSpawn;
        timerSpawn = nullptr;
    }
    if (timerSecond) {
        timerSecond->stop();
        delete timerSecond;
        timerSecond = nullptr;
    }

    // Eliminar el jugador
    if (jugador) {
        delete jugador;
        jugador = nullptr;
    }

    // Eliminar fondos
    if (fondo1) {
        delete fondo1;
        fondo1 = nullptr;
    }
    if (fondo2) {
        delete fondo2;
        fondo2 = nullptr;
    }

    // Eliminar Obstáculos
    for (Obstaculo *obs : obstaculos) {
        if (obs) {
            delete obs;
        }
    }
    obstaculos.clear();
}

void NivelBase::gameLoop()
{
    if (juegoTerminado) return;

    // CRÍTICO: Protección contra el puntero nulo del jugador (Evita SIGSEGV)
    if (!jugador) {
        qWarning() << "Error: jugador es nullptr en gameLoop.";
        return;
    }

    // A. MOVER FONDO (SCROLLING)
    if (fondo1) {
        fondo1->setPos(0, fondo1->y() + velocidadFondo);
        if (fondo1->y() >= 600) fondo1->setPos(0, -600);
    }

    // El fondo 2 SOLO se mueve si existe (solo en Nivel3)
    if (fondo2) {
        fondo2->setPos(0, fondo2->y() + velocidadFondo);
    }

    // Resetear posición (solo si el fondo 2 existe)
    if (fondo1 && fondo1->y() >= 600) fondo1->setPos(0, -600);

    if (fondo2 && fondo2->y() >= 600) {
        fondo2->setPos(0, -600);
    }
    // B. MANEJAR OBSTÁCULOS Y COLISIONES
    for (int i = 0; i < obstaculos.size(); ++i) {
        Obstaculo *obs = obstaculos[i];
        if (!obs) {
            // Si por alguna razón el puntero es nulo, lo eliminamos de la lista.
            obstaculos.removeAt(i);
            i--;
            continue;
        }
        obs->mover(velocidadFondo);

        // Se necesita hacer un cast a QGraphicsItem* para usar collidesWithItem
        if (jugador->collidesWithItem(obs)) {
            // ... (resto de la lógica de colisión es correcta)
            // 1. Penalización
            if (velocidadFondo > 2) {
                velocidadFondo -= 1;
            }
            emit actualizarHUD(tiempoRestante, velocidadFondo * 20);

            // 2. Crear Explosión
            Explosion *boom = new Explosion();
            boom->setPos(obs->pos());
            addItem(boom);

            // 3. Eliminar obstáculo
            removeItem(obs);
            delete obs;
            obstaculos.removeAt(i);
            i--;
            continue;
        }

        // 4. Eliminar si sale de pantalla
        if (obs->y() > 600) {
            removeItem(obs);
            delete obs;
            obstaculos.removeAt(i);
            i--;
        }
    }
}

QString NivelBase::getObstaculoPath() const
{
    // Devuelve una cadena vacía o una ruta de "obstáculo nulo"
    return QString();
}

void NivelBase::spawnObstacle()
{
    if (juegoTerminado) return;

    QString path = getObstaculoPath();

    // **VERIFICACIÓN DE SEGURIDAD:** Solo crea un obstáculo si la ruta no está vacía.
    if (path.isEmpty()) {
        return; // Salir si el nivel no tiene obstáculos (como nivel1)
    }

    Obstaculo *obs = new Obstaculo(path);
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
        winText->setPos(240, 250);
        winText->setZValue(20);
        addItem(winText);

        // Señal para que MainWindow cambie de nivel
        QTimer::singleShot(2000, this, [this](){ emit nivelTerminado(); });
    }
}

void NivelBase::iniciarTimers(bool usarTiempo, bool usarSpawn)
{
    // timerLoop (Bucle de juego) siempre se inicia para manejar el jugador y colisiones
    timerLoop->start(16);

    // Spawn de Obstáculos (solo si se pide)
    if (usarSpawn) {
        timerSpawn->start(1500);
    } else {
        // Aseguramos que no se intente aparecer un obstáculo
        timerSpawn->stop();
    }

    // Timer de Tiempo (solo si se pide)
    if (usarTiempo) {
        timerSecond->start(1000);
    } else {
        // Aseguramos que no se emita la señal de tiempo
        timerSecond->stop();
    }
}
