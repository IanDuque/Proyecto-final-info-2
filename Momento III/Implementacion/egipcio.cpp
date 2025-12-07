#include "egipcio.h"
#include "nivel2.h"
#include "bloques.h"
#include "recolectables.h"

#include <QGraphicsScene>
#include <QDebug>

Egipcio::Egipcio(nivel2 *nivel, QObject *parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    m_nivel(nivel),
    timerAnimacion(nullptr),
    timerMovimiento(nullptr),
    frameActual(0),
    direccionActual(Frente),
    enMovimiento(false),
    teclaArriba(false),
    teclaAbajo(false),
    teclaIzquierda(false),
    teclaDerecha(false),
    velocidad(2)
{
    // ----- SPRITE QUIETO (FRENTE) -----
    spriteQuietoFrente.load(":/Imagenes/egipcioquietofrente.png");
    if (spriteQuietoFrente.isNull()) {
        qDebug() << "Error cargando :/Imagenes/egipcioquietofrente.png";
        spriteQuietoFrente = QPixmap(50, 50);
        spriteQuietoFrente.fill(Qt::yellow);
    }
    spriteQuietoFrente = spriteQuietoFrente.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(spriteQuietoFrente);

    // ----- FRAMES FRENTE (S) -----
    {
        QString path1(":/Imagenes/egipciocaminafrente1.png");
        QString path2(":/Imagenes/egipciocaminafrente2.png");

        QPixmap p1(path1), p2(path2);
        if (p1.isNull()) { qDebug() << "Error cargando" << path1; p1 = QPixmap(60,60); p1.fill(Qt::blue); }
        if (p2.isNull()) { qDebug() << "Error cargando" << path2; p2 = QPixmap(60,60); p2.fill(Qt::blue); }

        framesFrente.append(p1.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        framesFrente.append(p2.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    // ----- FRAMES ESPALDA (W) -----
    {
        QString path1(":/Imagenes/egipciocaminaespalda1.png");
        QString path2(":/Imagenes/egipciocaminaespalda2.png");

        QPixmap p1(path1), p2(path2);
        if (p1.isNull()) { qDebug() << "Error cargando" << path1; p1 = QPixmap(60,60); p1.fill(Qt::red); }
        if (p2.isNull()) { qDebug() << "Error cargando" << path2; p2 = QPixmap(60,60); p2.fill(Qt::red); }

        framesEspalda.append(p1.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        framesEspalda.append(p2.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    // ----- FRAMES IZQUIERDA (A) -----
    {
        QString path1(":/Imagenes/egipciocaminaizquierda1.png");
        QString path2(":/Imagenes/egipciocaminaizquierda2.png");

        QPixmap p1(path1), p2(path2);
        if (p1.isNull()) { qDebug() << "Error cargando" << path1; p1 = QPixmap(60,60); p1.fill(Qt::green); }
        if (p2.isNull()) { qDebug() << "Error cargando" << path2; p2 = QPixmap(60,60); p2.fill(Qt::green); }

        framesIzquierda.append(p1.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        framesIzquierda.append(p2.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    // ----- FRAMES DERECHA (D) -----
    {
        QString path1(":/Imagenes/egipciocaminaderecha1.png");
        QString path2(":/Imagenes/egipciocaminaderecha2.png");

        QPixmap p1(path1), p2(path2);
        if (p1.isNull()) { qDebug() << "Error cargando" << path1; p1 = QPixmap(60,60); p1.fill(Qt::magenta); }
        if (p2.isNull()) { qDebug() << "Error cargando" << path2; p2 = QPixmap(60,60); p2.fill(Qt::magenta); }

        framesDerecha.append(p1.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        framesDerecha.append(p2.scaled(60,60,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    // ----- TIMER DE ANIMACIÓN -----
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Egipcio::actualizarAnimacion);
    timerAnimacion->start(150); // velocidad de cambio de frame

    // ----- TIMER DE MOVIMIENTO (para movimiento continuo + colisiones) -----
    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, & Egipcio::actualizarMovimiento);
    timerMovimiento->start(16); // ~60 FPS

    setFlag(QGraphicsItem::ItemIsFocusable);
}

// Devuelve true si alguna tecla de movimiento está activa
bool Egipcio::hayMovimiento() const
{
    return teclaArriba || teclaAbajo || teclaIzquierda || teclaDerecha;
}

void Egipcio::actualizarAnimacion()
{
    enMovimiento = hayMovimiento();

    if (!enMovimiento) {
        // Ninguna tecla: sprite quieto de frente
        setPixmap(spriteQuietoFrente);
        return;
    }

    const QList<QPixmap> *frames = nullptr;

    switch (direccionActual) {
    case Frente:
        frames = &framesFrente;
        break;
    case Espalda:
        frames = &framesEspalda;
        break;
    case Izquierda:
        frames = &framesIzquierda;
        break;
    case Derecha:
        frames = &framesDerecha;
        break;
    }

    if (!frames || frames->isEmpty()) {
        setPixmap(spriteQuietoFrente);
        return;
    }

    frameActual = (frameActual + 1) % frames->size();
    setPixmap(frames->at(frameActual));
}

// 🔹 Movimiento continuo + colisiones con Bloques y Recolectables
void Egipcio::actualizarMovimiento()
{
    if (!hayMovimiento())
        return;

    QPointF oldPos = pos();
    QPointF newPos = oldPos;

    if (teclaArriba)    newPos.setY(newPos.y() - velocidad);
    if (teclaAbajo)     newPos.setY(newPos.y() + velocidad);
    if (teclaIzquierda) newPos.setX(newPos.x() - velocidad);
    if (teclaDerecha)   newPos.setX(newPos.x() + velocidad);

    // Mover provisionalmente
    setPos(newPos);

    // Revisar colisiones
    QList<QGraphicsItem*> col = collidingItems();
    for (QGraphicsItem *it : col) {

        // --- PAREDES ---
        if (dynamic_cast<Bloques*>(it)) {
            // Revertir movimiento si choca contra una pared
            setPos(oldPos);
            return;
        }

        // --- RECOLECTABLES ---
        if (auto r = dynamic_cast<recolectables*>(it)) {
            if (m_nivel) {
                m_nivel->incrementarRecolectados(r);
            }
        }
    }
}

void Egipcio::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_W) {
        teclaArriba = true;
        direccionActual = Espalda;
    }
    if (event->key() == Qt::Key_S) {
        teclaAbajo = true;
        direccionActual = Frente;
    }
    if (event->key() == Qt::Key_A) {
        teclaIzquierda = true;
        direccionActual = Izquierda;
    }
    if (event->key() == Qt::Key_D) {
        teclaDerecha = true;
        direccionActual = Derecha;
    }
}

void Egipcio::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_W) teclaArriba    = false;
    if (event->key() == Qt::Key_S) teclaAbajo     = false;
    if (event->key() == Qt::Key_A) teclaIzquierda = false;
    if (event->key() == Qt::Key_D) teclaDerecha   = false;

    enMovimiento = hayMovimiento();
}

void Egipcio::detenerAcciones()
{
    teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
    enMovimiento = false;
    frameActual = 0;

    if (timerAnimacion)
        timerAnimacion->stop();
    if (timerMovimiento)
        timerMovimiento->stop();

    setPixmap(spriteQuietoFrente);
}

