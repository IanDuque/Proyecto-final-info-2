#include "indio.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QDebug>

// 1. En el constructor, llamamos a Personaje(vida_inicial)
Indio::Indio(int vida_inicial, int defensa_inicial, QObject *parent)
    : Personaje(vida_inicial, parent), // <--- Inicializamos la clase base
    defensa(defensa_inicial)
{
    controlEnabled = false;
    isMoving = false;
    frameActual = 0;

    // --- CONFIGURACIÓN DE SPRITES (Igual que antes) ---
    numFrames = 10;
    spriteSheetQuieto.load(":/Imagenes/indioquieto.png");
    spriteSheetCorriendo.load(":/Imagenes/indiocorriendo.png");

    if(spriteSheetQuieto.isNull()) qDebug() << "Error cargando indioquieto.png";
    if(spriteSheetCorriendo.isNull()) qDebug() << "Error cargando indiocorriendo.png";

    altoFrame = 583;
    anchoFrame = 1280 / numFrames;
    spriteActual = &spriteSheetQuieto;

    // Timer Animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Indio::actualizarAnimacion);
    timerAnimacion->start(50);

    // Habilitar foco para teclado
    setFlag(QGraphicsItem::ItemIsFocusable);

    // Dibujar primer frame
    actualizarAnimacion();
}

void Indio::actualizarAnimacion()
{
    frameActual = (frameActual + 1) % numFrames;
    QPixmap recorte = spriteActual->copy(frameActual * anchoFrame, 0, anchoFrame, altoFrame);
    setPixmap(recorte.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Getters y Setters
int Indio::getdefensa() const { return defensa; }
void Indio::setdefensa(int cambiodefensa) { defensa = cambiodefensa; }

// NOTA: getVida() ya existe en Personaje, no hace falta reescribirlo.
// Pero si tenias setVida especifico, puedes reusar la variable 'vida' de Personaje
// (ya que es protected).

void Indio::setControlEnabled(bool enable) {
    controlEnabled = enable;
}

void Indio::keyPressEvent(QKeyEvent *event)
{
    if (!controlEnabled) return;

    bool teclaMovimiento = false;

    if (event->key() == Qt::Key_W) {
        setPos(x(), y() - 5);
        teclaMovimiento = true;
    }
    if (event->key() == Qt::Key_S) {
        setPos(x(), y() + 5);
        teclaMovimiento = true;
    }
    if (event->key() == Qt::Key_A) {
        setPos(x() - 5, y());
        teclaMovimiento = true;
    }
    if (event->key() == Qt::Key_D) {
        setPos(x() + 5, y());
        teclaMovimiento = true;
    }

    if (event->key() == Qt::Key_Space) {
        disparar();
    }

    if (teclaMovimiento && !isMoving) {
        isMoving = true;
        spriteActual = &spriteSheetCorriendo;
        frameActual = 0;
    }
    // Opcional: Volver a estado quieto al soltar teclas (requeriría keyReleaseEvent)
}

void Indio::recibirDanio(int valor)
{
    // Llamamos a la lógica base para restar la vida matemática
    Personaje::recibirDanio(valor);

    qDebug() << "Indio recibió daño. Vida restante:" << vida; // 'vida' viene de Personaje

    // Lógica extra de muerte visual si es necesaria
    if (!estaVivo()) { // estaVivo() viene de Personaje
        qDebug() << "El Indio ha muerto";
        // Aquí podrías cambiar el sprite a uno de muerte o emitir señal game over
    }
}

void Indio::disparar()
{
    // Ajuste de coordenadas para que salga más cerca del "arma"
    Proyectil *lanza = new Proyectil(x() + 60, y() + 20, 0, 25, 1);
    scene()->addItem(lanza);
}
