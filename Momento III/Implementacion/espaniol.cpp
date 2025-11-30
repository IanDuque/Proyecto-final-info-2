#include "espaniol.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

Espaniol::Espaniol(QObject *parent) : Personaje(150, parent) // 150 vida (es mas fuerte)
{
    spriteSheetQuieto.load(":/Imagenes/soldadoquieto.png");
    spriteSheetDisparando.load(":/Imagenes/soldadodisparando.png");

    numFrames = 10; // Asumimos misma cantidad de frames (1280px total)
    altoFrame = 583;
    anchoFrame = 1280 / numFrames;

    spriteActual = &spriteSheetQuieto;
    atacando = false;
    frameActual = 0;

    // Timer Animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Espaniol::actualizarAnimacion);
    timerAnimacion->start(50);

    // IA Disparo
    timerAtaque = new QTimer(this);
    connect(timerAtaque, &QTimer::timeout, this, &Espaniol::disparar);
    timerAtaque->start(2000); // Dispara cada 2 segundos
}
void Espaniol::actualizarAnimacion()
{
    frameActual = (frameActual + 1) % numFrames;
    QPixmap recorte = spriteActual->copy(frameActual * anchoFrame, 0, anchoFrame, altoFrame);

    // El soldado mira a la izquierda, así que usamos transform para espejar si es necesario
    // Pero si el sprite ya mira a la izquierda, déjalo así.
    // Si mira a la derecha y quieres voltearlo: recorte = recorte.transformed(QTransform().scale(-1, 1));

    setPixmap(recorte.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Espaniol::disparar()
{
    if (!scene() || !estaVivo()) return;

    // 1. Cambiar animación
    atacando = true;
    spriteActual = &spriteSheetDisparando;
    frameActual = 0;

    // 2. Crear bala (ajustar coordenadas para que salga del arma)
    // Usamos Tipo 2 (Granada/Bala)
    // Ángulo 135 para ir arriba-izquierda
    Proyectil *bala = new Proyectil(x(), y() + 20, 135, 22, 2);
    scene()->addItem(bala);

    // 3. Volver a estado quieto después de 500ms (tiempo que dura la animación de disparo aprox)
    QTimer::singleShot(500, this, &Espaniol::finalizarDisparo);
}
void Espaniol::finalizarDisparo()
{
    atacando = false;
    spriteActual = &spriteSheetQuieto;
    frameActual = 0;
}
