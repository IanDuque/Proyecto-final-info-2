#include "indio.h"
#include "proyectil.h"
#include "nivelbase.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

Indio::Indio(int vida_inicial, QObject *parent)
    : Personaje(vida_inicial, parent),
    controlEnabled(false),
    lanzaEnAire(false),
    timerAnimacion(nullptr),
    frameActual(0),
    izquierda(false),
    derecha(false),
    timerFisica(nullptr),
    vy(0.0),
    gravedad(0.8),
    velocidadSalto(15.0),
    ySuelo(430.0),
    saltando(false),
    enSuelo(true)
{
    // ----- CARGAR SPRITE QUIETO (1 solo) -----
    spriteQuieto.load(":/Imagenes/indioquieto1.png");
    if (spriteQuieto.isNull()) {
        qDebug() << "Error cargando :/Imagenes/indioquieto1.png";
        spriteQuieto = QPixmap(120, 120);
        spriteQuieto.fill(Qt::red);
    }
    spriteQuieto = spriteQuieto.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // ----- CARGAR SPRITES CORRER (varios) -----
    for (int i = 1; i <= 6; ++i) {
        QString path = QString(":/Imagenes/indiocorriendo%1.png").arg(i);
        QPixmap px(path);
        if (px.isNull()) {
            qDebug() << "Error cargando" << path;
            px = QPixmap(120, 120);
            px.fill(Qt::blue);
        }
        framesCorrer.append(px.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Estado inicial: quieto
    setPixmap(spriteQuieto);

    // Timer de ANIMACIÓN (correr / quieto)
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Indio::actualizarAnimacion);
    timerAnimacion->start(150); // más lento

    // Timer de FÍSICA (salto)
    timerFisica = new QTimer(this);
    connect(timerFisica, &QTimer::timeout, this, &Indio::actualizarFisica);
    timerFisica->start(16); // ~60 FPS

    setFlag(QGraphicsItem::ItemIsFocusable);
}

// ------------------------ ANIMACIÓN ------------------------
void Indio::actualizarAnimacion()
{
    // En movimiento si hay desplazamiento horizontal o está en el aire
    bool enMovimiento = izquierda || derecha || !enSuelo;

    if (enMovimiento && !framesCorrer.isEmpty()) {
        frameActual = (frameActual + 1) % framesCorrer.size();
        setPixmap(framesCorrer.at(frameActual));
    } else {
        setPixmap(spriteQuieto);
    }
}

// ------------------------ CONTROL ------------------------
void Indio::setControlEnabled(bool enable)
{
    controlEnabled = enable;

    if (!enable) {
        izquierda = derecha = false;
        frameActual = 0;
        setPixmap(spriteQuieto);
    }
}

// ------------------------ DAÑO ------------------------
void Indio::recibirDanio(int valor)
{
    Personaje::recibirDanio(valor);
    qDebug() << "Indio recibió daño. Vida:" << vida;

    if (!estaVivo()) {
        qDebug() << "El Indio ha muerto";

        // Avisar al nivel
        NivelBase *nivel = dynamic_cast<NivelBase*>(scene());
        if (nivel) {
            nivel->onJugadorMuere();
        }
    }
}

// ------------------------ DISPARAR LANZA ------------------------
void Indio::disparar()
{
    if (!scene() || !estaVivo()) return;

    if (lanzaEnAire) return;
    lanzaEnAire = true;

    // Posición del español (fija)
    double objetivoX = 650;
    double objetivoY = 430;

    // Punto desde donde sale la lanza
    double xIni = x() + 60;
    double yIni = y() + 20;

    double dx = objetivoX - xIni;
    double dy = objetivoY - yIni;

    // Parámetros físicos
    double g = 1.0;
    double N = 40.0;

    // Cálculo de vx y vy0
    double vx  = dx / N;
    double vy0 = (dy - g * N * (N + 1) / 2) / N;

    double velocidad = qSqrt(vx * vx + vy0 * vy0);
    double rad = qAtan2(-vy0, vx);
    double anguloDeg = qRadiansToDegrees(rad);

    // Crea la lanza
    Proyectil *lanza = new Proyectil(xIni, yIni, anguloDeg, velocidad, 1);
    scene()->addItem(lanza);
}

void Indio::setLanzaLibre()
{
    lanzaEnAire = false;
}

// ------------------------ FÍSICA DEL SALTO ------------------------
void Indio::actualizarFisica()
{
    if (!controlEnabled) return;

    // Si está en el suelo y no está saltando, no hay nada que actualizar
    if (enSuelo && !saltando) return;

    // Actualizar velocidad y posición vertical
    vy += gravedad;
    double nuevaY = y() + vy;

    if (nuevaY >= ySuelo) {
        // Ha tocado el suelo
        nuevaY = ySuelo;
        vy = 0.0;
        enSuelo = true;
        saltando = false;
    }

    setY(nuevaY);
}

// ------------------------ TECLAS ------------------------
void Indio::keyPressEvent(QKeyEvent *event)
{
    if (!controlEnabled) return;

    const qreal MIN_X = 50.0;   // límite izquierdo
    const qreal MAX_X = 350.0;  // límite derecho (ajusta a tu gusto)

    if (event->key() == Qt::Key_W) {
        // solo salta si está en el suelo
        if (enSuelo && !saltando) {
            saltando = true;
            enSuelo = false;
            vy = -velocidadSalto;
        }
    }

    if (event->key() == Qt::Key_A) {
        izquierda = true;
        derecha   = false;
        qreal nuevaX = x() - 10;
        if (nuevaX < MIN_X) nuevaX = MIN_X;
        setX(nuevaX);
    }

    if (event->key() == Qt::Key_D) {
        derecha   = true;
        izquierda = false;
        qreal nuevaX = x() + 10;
        if (nuevaX > MAX_X) nuevaX = MAX_X;
        setX(nuevaX);
    }

    if (event->key() == Qt::Key_Space) {
        disparar();
    }
}

void Indio::keyReleaseEvent(QKeyEvent *event)
{
    if (!controlEnabled) return;

    if (event->key() == Qt::Key_A) izquierda = false;
    if (event->key() == Qt::Key_D) derecha   = false;
}

// ------------------------ DETENER TODO ------------------------
void Indio::detenerAcciones()
{
    controlEnabled = false;
    izquierda = derecha = false;
    saltando = false;
    enSuelo  = true;
    vy = 0.0;

    if (timerAnimacion) timerAnimacion->stop();
    if (timerFisica)     timerFisica->stop();

    setPixmap(spriteQuieto);
}

