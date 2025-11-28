#include "nivelbase.h"
#include <QDebug>
#include <QFont>
#include "carro.h" // Incluir aquí para la colisión

NivelBase::NivelBase(QObject *parent)
    : QGraphicsScene(parent), jugador(nullptr),    fondo1(nullptr),  // <--- Se agrego esto
    fondo2(nullptr) // Inicializa jugador a nullptr (seguridad)
{

    setSceneRect(0, 0, 800, 600);

    velocidadFondo = 5;
    tiempoRestante = 30; //el tiempo que usaremos sera 120 pero a efectos de prueba sera 15.
    juegoTerminado = false;

    vidas = 5; // COMENZAMOS CON 5 VIDAS
    modoSenoidalActivo = false;
    juegoTerminado = false;
    tiempoTotalInicial = tiempoRestante;

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
    // 1. Detener y eliminar los Timers (Esto es correcto y necesario)
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

    // 2. IMPORTANTE: NO eliminar 'jugador', 'fondo1', 'fondo2' ni items de 'obstaculos'.
    // QGraphicsScene se encarga de borrarlos automáticamente al destruirse.
    // Si usas 'delete jugador' aquí, el juego CRASHEA.

    // Solo limpiamos la lista para no dejar referencias sueltas.
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

        // Seguridad por si el puntero es nulo
        if (!obs) {
            obstaculos.removeAt(i);
            i--;
            continue;
        }

        // 1. Mover el obstáculo
        // (La lógica senoidal ya está dentro de la clase Obstaculo, aquí solo llamamos a mover)
        obs->mover(velocidadFondo);

        // 2. Detectar Colisión con el Jugador
        // Usamos collidesWithItem para una detección precisa
        if (jugador->collidesWithItem(obs)) {

            // --- LÓGICA DE VIDAS ---
            vidas--;
            qDebug() << "¡Colisión! Vidas restantes:" << vidas;

            // Opcional: Reducir velocidad como penalización leve
            if (velocidadFondo > 2) {
                velocidadFondo -= 1;
            }

            // Actualizar HUD (Tiempo y velocidad)
            // Si agregaste el argumento 'vidas' a tu señal, agrégalo aquí: emit actualizarHUD(..., vidas);
            emit actualizarHUD(tiempoRestante, velocidadFondo * 20,vidas);

            // --- CREAR EXPLOSIÓN ---
            Explosion *boom = new Explosion();
            boom->setPos(obs->pos()); // La explosión aparece donde estaba el obstáculo
            addItem(boom);

            // --- ELIMINAR OBSTÁCULO ---
            removeItem(obs);       // Quitar de la escena
            delete obs;            // Liberar memoria
            obstaculos.removeAt(i);// Quitar de la lista
            i--;                   // Ajustar índice porque la lista se redujo

            // --- VERIFICAR DERROTA (GAME OVER) ---
            if (vidas <= 0) {
                juegoTerminado = true;

                // Detener todos los timers
                timerLoop->stop();
                timerSpawn->stop();
                timerSecond->stop();

                // Mostrar Mensaje de PERDEDOR
                QGraphicsTextItem *loseText = new QGraphicsTextItem("¡HAS PERDIDO!");
                QFont fontLose("Arial", 30, QFont::Bold); // Fuente grande y negrita
                loseText->setFont(fontLose);
                loseText->setDefaultTextColor(Qt::red);

                // Centrar texto (ajusta coordenadas si es necesario)
                loseText->setPos(250, 250);
                loseText->setZValue(20); // ZValue alto para que aparezca encima de todo
                addItem(loseText);

                return; // Salimos inmediatamente del loop
            }

            continue; // Pasamos al siguiente obstáculo del ciclo
        }

        // 3. Eliminar si sale de la pantalla (parte inferior)
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
    if (path.isEmpty() || path == "") {
        return; // Salir si el nivel no tiene obstáculos (como nivel1)
    }

    Obstaculo *obs = new Obstaculo(path);
    if (modoSenoidalActivo) {
        obs->setMovimientoSenoidal(true);
    }
    addItem(obs);
    obstaculos.append(obs);
}

void NivelBase::updateTimer()
{
    if (juegoTerminado) return;

    tiempoRestante--;
    if (tiempoRestante <= (tiempoTotalInicial / 2) && !modoSenoidalActivo) {
        modoSenoidalActivo = true;
        qDebug() << "¡Modo Senoidal Activado!";

        // Opcional: Cambiar los obstáculos que YA están en pantalla
        for (Obstaculo* obs : obstaculos) {
            obs->setMovimientoSenoidal(true);
        }
    }
    emit actualizarHUD(tiempoRestante, velocidadFondo * 20, vidas);

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
