#include "indio.h"
#include "proyectil.h"
#include "nivelbase.h"
#include <QGraphicsScene>
#include <QDebug>

Indio::Indio(int vida_inicial, QObject *parent)
    : Personaje(vida_inicial, parent),
    controlEnabled(false),
    timerAnimacion(nullptr),
    frameActual(0),
    arriba(false),
    abajo(false),
    izquierda(false),
    derecha(false)
{
    //nos dice que no hay lanzas en pantalla ahora mismo.
    lanzaEnAire = false;

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

    // Timer de animación (más lento: 150 ms)
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Indio::actualizarAnimacion);
    timerAnimacion->start(150);

    setFlag(QGraphicsItem::ItemIsFocusable);
}

// Animación: si se está moviendo, recorre framesCorrer, si no, muestra spriteQuieto
void Indio::actualizarAnimacion()
{
    bool enMovimiento = (arriba || abajo || izquierda || derecha);

    if (enMovimiento && !framesCorrer.isEmpty()) {
        frameActual = (frameActual + 1) % framesCorrer.size();
        setPixmap(framesCorrer.at(frameActual));
    } else {
        setPixmap(spriteQuieto);
    }
}

void Indio::setControlEnabled(bool enable)
{
    controlEnabled = enable;

    if (!enable) {
        arriba = abajo = izquierda = derecha = false;
        frameActual = 0;
        setPixmap(spriteQuieto);
    }
}

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

void Indio::disparar()
{
    if (!scene() || !estaVivo()) return;

    if (lanzaEnAire) return;
    lanzaEnAire = true;

    // Posicion del español
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
    Proyectil *lanza = new Proyectil(xIni,yIni,anguloDeg,velocidad,1);
    scene()->addItem(lanza);
}

void Indio::setLanzaLibre()
{
    lanzaEnAire = false;
}

void Indio::keyPressEvent(QKeyEvent *event)
{
    if (!controlEnabled) return;

    if (event->key() == Qt::Key_W) {
        setPos(x(), y() - 10);
        arriba = true;
    }
    if (event->key() == Qt::Key_S) {
        setPos(x(), y() + 10);
        abajo = true;
    }
    if (event->key() == Qt::Key_A) {
        setPos(x() - 10, y());
        izquierda = true;
    }
    if (event->key() == Qt::Key_D) {
        setPos(x() + 10, y());
        derecha = true;
    }

    if (event->key() == Qt::Key_Space)
        disparar();
}

void Indio::keyReleaseEvent(QKeyEvent *event)
{
    if (!controlEnabled) return;

    if (event->key() == Qt::Key_W) arriba = false;
    if (event->key() == Qt::Key_S) abajo = false;
    if (event->key() == Qt::Key_A) izquierda = false;
    if (event->key() == Qt::Key_D) derecha = false;
}
