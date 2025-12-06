#include "nivel2.h"
#include "egipcio.h"
#include "bloques.h"
#include "recolectables.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QDebug>

nivel2::nivel2(QObject *parent)
    : NivelBase(parent),
    egipcio(nullptr),
    textoRecolectados(nullptr),
    totalPorRecolectar(6),
    recolectados(0)
{
    tiempoRestante     = 15; //el nivel tendra una duracion de 90 segundos pero usamos 15 a efectos de prueba
    tiempoTotalInicial = tiempoRestante;

    velocidadFondo = 0; // fondo estático

    configurarFondo();
    inicializarJugador();

    crearLaberinto();
    crearColeccionables();

    // Texto "Recolectados: x/6"
    textoRecolectados = new QGraphicsTextItem();
    textoRecolectados->setFont(QFont("Arial", 16));
    textoRecolectados->setDefaultTextColor(Qt::white);
    textoRecolectados->setPos(10, 30);
    textoRecolectados->setZValue(10);
    addItem(textoRecolectados);

    actualizarTextoRecolectados();
}

QString nivel2::getFondoPath() const
{
    return ":/Imagenes/fondonivel2.jpg";
}

void nivel2::configurarFondo()
{
    QPixmap bg(getFondoPath());
    if (bg.isNull()) {
        qCritical() << "ERROR: Fondo Nivel 2 no cargado.";
        bg = QPixmap(800, 600);
        bg.fill(Qt::darkBlue);
    }

    bg = bg.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fondo1 = new QGraphicsPixmapItem(bg);
    fondo1->setPos(0, 0);
    addItem(fondo1);
}

QString nivel2::getObstaculoPath() const
{
    // No usamos obstáculos automáticos en el nivel 2
    return QString();
}

void nivel2::inicializarJugador()
{
    Egipcio *player = new Egipcio(this);
    egipcio = player;
    jugador = player; // puntero heredado de NivelBase

    jugador->setPos(50, 430);  // posición inicial dentro del laberinto (ajusta según el diseño)
    jugador->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(jugador);
    jugador->setFocus();
}

// ---------------- LABERINTO (paredes usando "bloques") ----------------
void nivel2::crearLaberinto()
{
    const int anchoEscena = 800;
    const int altoEscena  = 600;
    const int size        = 40;

    // --- BORDES (Tu código original intacto) ---
    // Bordes horizontales (arriba y abajo)
    for (int x = 0; x < anchoEscena; x += size) {
        Bloques *top = new Bloques();
        top->setPos(x-15, -10);
        addItem(top);
        paredes.append(top);

        Bloques *bottom = new Bloques();
        bottom->setPos(x-15, altoEscena - size);
        addItem(bottom);
        paredes.append(bottom);
    }

    // Bordes verticales (izquierda y derecha)
    for (int y = 0; y < altoEscena; y += size) {
        Bloques *left = new Bloques();
        left->setPos(-15, y-10);
        addItem(left);
        paredes.append(left);

        Bloques *right = new Bloques();
        right->setPos(anchoEscena - size -15, y-10);
        addItem(right);
        paredes.append(right);
    }

    // --- OBSTÁCULOS INTERNOS (Configurados según la imagen) ---

    // 1. Barra Horizontal del MEDIO (Larga)
    // Coordenada Y visual: aprox 280. X: va desde la columna izquierda hasta la derecha.
    for (int x = 100; x <= 500; x += size) {
        Bloques *muro = new Bloques();
        muro->setPos(x, 200);
        addItem(muro);
        paredes.append(muro);
    }

    // 2. Barra Horizontal de ABAJO (Larga)
    for (int x = 80; x <= 610; x += size) {
        Bloques *muro = new Bloques();
        muro->setPos(x, 428);
        addItem(muro);
        paredes.append(muro);
    }
    //Vertical grande
    for (int y = 80; y <= 410; y += size) {
        Bloques *muro = new Bloques();
        muro->setPos(500, y);
        addItem(muro);
        paredes.append(muro);
    }

    for (int y = 348; y <= 500; y += size) {
        Bloques *muro = new Bloques();
        muro->setPos(350, y);
        addItem(muro);
        paredes.append(muro);
    }

    for (int y = 80; y <= 350; y += size) {
        Bloques *muro = new Bloques();
        muro->setPos(200, y);
        addItem(muro);
        paredes.append(muro);
    }

    for (int y = 25; y <= 100; y += size) {
        Bloques *muro = new Bloques();
        muro->setPos(330, y);
        addItem(muro);
        paredes.append(muro);
    }

    for (int y = 25; y <= 100; y += size) {
        Bloques *muro = new Bloques();
        muro->setPos(620, y);
        addItem(muro);
        paredes.append(muro);
    }







}

// ---------------- COLECCIONABLES (recolectables) ----------------
void nivel2::crearColeccionables()
{
    // Coordenadas ejemplo (luego las ajustas al laberinto real)
    posicionesrecolectables.clear();
    posicionesrecolectables << QPointF(90, 90)
                      << QPointF(200, 500) //no es este jajaaj
                      << QPointF(440, 150)
                      << QPointF(440, 370)//370
                      << QPointF(680, 500)
                      << QPointF(700, 70);

    totalPorRecolectar = posicionesrecolectables.size();
    recolectados       = 0;

    for (const QPointF &p : posicionesrecolectables) {
        recolectables *b = new recolectables();
        b->setPos(p);
        addItem(b);
        coleccionables.append(b);
    }
}

// ---------------- RECOLECCIÓN ----------------
void nivel2::incrementarRecolectados(recolectables *bloque)
{
    if (!bloque || juegoTerminado) return;

    if (coleccionables.contains(bloque)) {
        coleccionables.removeOne(bloque);
    }

    removeItem(bloque);
    delete bloque;

    recolectados++;
    actualizarTextoRecolectados();

    //verificar si ya se tienen todos los bloques.
    if (recolectados >= totalPorRecolectar) {

        juegoTerminado = true;

        // Detener timers
        if (timerLoop)   timerLoop->stop();
        if (timerSpawn)  timerSpawn->stop();
        if (timerSecond) timerSecond->stop();

        // Pantalla de victoria del nivel 2
        QGraphicsTextItem *winText = new QGraphicsTextItem(QString::fromUtf8("¡Conseguiste todos los bloques a tiempo!"));
        winText->setFont(QFont("Arial", 26, QFont::Bold));
        winText->setDefaultTextColor(Qt::green);
        winText->setPos(70, 250);
        winText->setZValue(20);
        addItem(winText);

        //para regresar automaticamente al menu tras 3 segundos de acabar el nivel.
        QTimer::singleShot(3000, this, [this](){ emit nivelTerminado(); });
    }
}

void nivel2::actualizarTextoRecolectados()
{
    if (!textoRecolectados) return;

    textoRecolectados->setPlainText(QString("Recolectados: %1/%2").arg(recolectados).arg(totalPorRecolectar));
}

void nivel2::updateTimer()
{
    if (juegoTerminado) return;

    // Restar 1 segundo
    tiempoRestante--;

    // Actualizar HUD (tiempo visible, vidas no nos interesan aquí)
    emit actualizarHUD(tiempoRestante, 0, 0);

    // Si todavía queda tiempo, no hacemos nada más
    if (tiempoRestante > 0)
        return;

    // Aquí se acabó el tiempo
    juegoTerminado = true;

    // Detener timers
    if (timerLoop)   timerLoop->stop();
    if (timerSpawn)  timerSpawn->stop();
    if (timerSecond) timerSecond->stop();

    if (recolectados >= totalPorRecolectar) {
        QGraphicsTextItem *winText =
            new QGraphicsTextItem(QString::fromUtf8("¡Conseguiste todos los bloques a tiempo!"));
        winText->setFont(QFont("Arial", 26, QFont::Bold));
        winText->setDefaultTextColor(Qt::green);
        winText->setPos(70, 250);
        winText->setZValue(20);
        addItem(winText);

        //para regresar automaticamente al menu tras 3 segundos de acabar el nivel.
        QTimer::singleShot(3000, this, [this](){ emit nivelTerminado(); });

    } else {
        QGraphicsTextItem *loseText = new QGraphicsTextItem(QString::fromUtf8("¡No conseguiste los bloques necesarios"));
        loseText->setFont(QFont("Arial", 22, QFont::Bold));
        loseText->setDefaultTextColor(Qt::red);
        loseText->setPos(120, 250);
        loseText->setZValue(20);
        addItem(loseText);

        QGraphicsTextItem *loseText2 = new QGraphicsTextItem(QString::fromUtf8("para construir la pirámide!"));
        loseText2->setFont(QFont("Arial", 22, QFont::Bold));
        loseText2->setDefaultTextColor(Qt::red);
        loseText2->setPos(150, 280);
        loseText2->setZValue(20);
        addItem(loseText2);

        //para regresar automaticamente al menu tras 3 segundos de acabar el nivel.
        QTimer::singleShot(3000, this, [this](){ emit nivelTerminado(); });
    }
}
