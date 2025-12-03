#include "espaniol.h"
#include "proyectil.h"
#include "nivelbase.h"
#include "indio.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

Espaniol::Espaniol(QObject *parent)
    : Personaje(300, parent),     // Español comienza con 300 de vida
    timerAtaque(nullptr),
    timerAnimacion(nullptr),
    frameActual(0),
    atacando(false),
    usandoCanon(false),
    reduccionDanio(0),
    canon(nullptr),
    disparandoCanon(false)
{
    // ----- SPRITE QUIETO SOLDADO -----
    spriteQuieto.load(":/Imagenes/soldadoquieto1.png");
    if (spriteQuieto.isNull()) {
        qDebug() << "Error cargando :/Imagenes/soldadoquieto1.png";
        spriteQuieto = QPixmap(100, 100);
        spriteQuieto.fill(Qt::yellow);
    }
    spriteQuieto = spriteQuieto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(spriteQuieto);

    // ----- FRAMES DISPARAR (SOLO FASE 1) -----
    for (int i = 1; i <= 6; ++i) {
        QString path = QString(":/Imagenes/soldadodisparando%1.png").arg(i);
        QPixmap pix(path);
        if (pix.isNull()) {
            qDebug() << "Error cargando" << path;
            pix = QPixmap(100, 100);
            pix.fill(Qt::red);
        }
        framesDisparar.append(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // ----- CAÑÓN (fase 2) -----
    spriteCanonApuntando.load(":/Imagenes/canonapuntando.png");
    if (spriteCanonApuntando.isNull()) {
        qDebug() << "Error cargando :/Imagenes/canonapuntando.png";
        spriteCanonApuntando = QPixmap(80, 80);
        spriteCanonApuntando.fill(Qt::darkGray);
    }
    spriteCanonApuntando = spriteCanonApuntando.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    spriteCanonDisparando.load(":/Imagenes/canondisparando.png");
    if (spriteCanonDisparando.isNull()) {
        qDebug() << "Error cargando :/Imagenes/canondisparando.png";
        spriteCanonDisparando = QPixmap(80, 80);
        spriteCanonDisparando.fill(Qt::lightGray);
    }
    spriteCanonDisparando = spriteCanonDisparando.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    canon = new QGraphicsPixmapItem(this);
    canon->setPixmap(spriteCanonApuntando);
    canon->setPos(-10, 40);
    canon->setZValue(1);
    canon->setVisible(false);

    // ----- TIMER ANIMACIÓN -----
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Espaniol::actualizarAnimacion);
    timerAnimacion->start(150); // animación más lenta

    // ----- Implementacion de inteligencia -----
    timerAtaque = new QTimer(this);
    connect(timerAtaque, &QTimer::timeout, this, &Espaniol::disparar);
    timerAtaque->start(2000); // dispara cada 2 s aprox
}

// --------------------------------------------------
// ANIMACIÓN
// --------------------------------------------------
void Espaniol::actualizarAnimacion()
{
    if (!usandoCanon) {
        // FASE 1: usa animación de disparo del soldado
        if (atacando && !framesDisparar.isEmpty()) {
            frameActual = (frameActual + 1) % framesDisparar.size();
            setPixmap(framesDisparar.at(frameActual));
        } else {
            setPixmap(spriteQuieto);
        }
    } else {
        // FASE 2: el soldado siempre quieto, solo se anima el cañón
        setPixmap(spriteQuieto);
        // El cañón cambia de sprite en disparar() / finalizarDisparo()
    }
}

// --------------------------------------------------
// DISPARAR
// --------------------------------------------------
void Espaniol::disparar()
{
    if (!scene() || !estaVivo()) return;

    frameActual = 0;

    int offsetY = 40;

    if (!usandoCanon) {
        // --------- FASE 1 ---------
        atacando = true;

        Proyectil *bala = new Proyectil(x(), y() + offsetY,180,12,3);
        scene()->addItem(bala);

        QTimer::singleShot(500, this, &Espaniol::finalizarDisparo);
    }
    else {
        // --------- FASE 2 ---------
        Indio *objetivo = nullptr;
        for (QGraphicsItem *item : scene()->items()) {
            objetivo = dynamic_cast<Indio*>(item);
            if (objetivo) break;
        }

        if (!objetivo) {
            qDebug() << "No se encontró al Indio para disparo de cañón.";
            return;
        }

        double xIni = x();
        double yIni = y() + offsetY;

        double objetivoX = objetivo->x();
        double objetivoY = objetivo->y();

        double dx = objetivoX - xIni;
        double dy = objetivoY - yIni;

        double g = 1.0;     // misma gravedad que Proyectil (tipo 2)
        double N = 40.0;    // nº de frames hasta impactar (ajustable)

        double vx  = dx / N;
        double vy0 = (dy - g * N * (N + 1) / 2.0) / N;

        double velocidad = qSqrt(vx * vx + vy0 * vy0);
        double rad       = qAtan2(-vy0, vx);
        double anguloDeg = qRadiansToDegrees(rad);

        // Mostrar animación de cañón disparando
        disparandoCanon = true;
        if (canon) {
            canon->setPixmap(spriteCanonDisparando);
        }

        Proyectil *balaCanon = new Proyectil(xIni,yIni,anguloDeg,velocidad,2);
        scene()->addItem(balaCanon);

        QTimer::singleShot(500, this, &Espaniol::finalizarDisparo);
    }
}

// --------------------------------------------------
// FINALIZAR DISPARO
// --------------------------------------------------
void Espaniol::finalizarDisparo()
{
    if (!usandoCanon) {
        atacando = false;
        frameActual = 0;
    } else {
        disparandoCanon = false;
        if (canon) {
            canon->setPixmap(spriteCanonApuntando);
        }
    }
}

// --------------------------------------------------
// RECIBIR DAÑO CON CAMBIO DE FASE Y REDUCCIÓN
// --------------------------------------------------
void Espaniol::recibirDanio(int danio)
{
    int danoFinal = danio;

    // Si ya está en fase 2, reducimos el daño recibido
    if (usandoCanon) {
        danoFinal -= reduccionDanio;
        if (danoFinal < 0) danoFinal = 0;
    }

    Personaje::recibirDanio(danoFinal);
    qDebug() << "Español recibió daño. Vida actual:" << vida;

    // Cambio a fase 2 cuando llegue a 120 o menos
    if (!usandoCanon && vida <= 120) {
        usandoCanon = true;
        reduccionDanio = 10; //hace que el enemigo reciba menos daño por cada golpe
        qDebug() << "Español cambia a fase 2. Reducción de daño =" << reduccionDanio;

        if (canon) {
            canon->setVisible(true);
            canon->setPixmap(spriteCanonApuntando);
        }

        atacando = false;
        frameActual = 0;
    }

    if (!estaVivo()) {
        qDebug() << "El Español ha muerto";

        NivelBase *nivel = dynamic_cast<NivelBase*>(scene());
        if (nivel) {
            nivel->onEnemigoMuere();
        }
    }
}
