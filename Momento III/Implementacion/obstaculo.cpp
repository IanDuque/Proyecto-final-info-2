#include "obstaculo.h"
#include <QRandomGenerator>
#include <QtMath>

Obstaculo::Obstaculo(const QString &basePath, QObject *parent)
    : QObject(parent),
    velocidadPropia(0),
    esSenoidal(false),
    xOriginal(0),
    soloDecoracion(false)
{
    Q_UNUSED(basePath);

    // Rutas de sprites
    QString nombreObstaculoCalle = ":/Imagenes/obstaculo3.png"; // obstáculo carretera
    QString nombreEdificio1      = ":/Imagenes/obstaculo1.png"; // edificio 1
    QString nombreEdificio2      = ":/Imagenes/obstaculo2.png"; // edificio 2

    QPixmap pixmap;

    // Queremos que sigan saliendo bien los obstáculos de carretera,
    // así que les damos más probabilidad.
    // rand 0..9 → 0-5: carretera (60%), 6-7: edificio izq, 8-9: edificio der
    int r = QRandomGenerator::global()->bounded(0, 10);
    int tipo = 0;
    if (r <= 5)      tipo = 0; // carretera
    else if (r <= 7) tipo = 1; // edificio izquierdo
    else             tipo = 2; // edificio derecho

    int randomX = 0;

    if (tipo == 0) {
        // ❖ Obstáculo sobre la carretera
        soloDecoracion = false;

        pixmap.load(nombreObstaculoCalle);
        if (pixmap.isNull()) {
            pixmap = QPixmap(60, 60);
            pixmap.fill(QColor("#5d4037"));
        }

        pixmap = pixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);

        // X dentro de la carretera
        randomX = QRandomGenerator::global()->bounded(290, 456); // [290,455]

        // Velocidad extra además del fondo
        velocidadPropia = QRandomGenerator::global()->bounded(2, 5);
    } else {
        // Edificios a izquierda o derecha
        soloDecoracion = true; // MUY IMPORTANTE

        bool ladoIzquierdo = (tipo == 1);
        QString rutaEdificio = ladoIzquierdo ? nombreEdificio1 : nombreEdificio2;

        pixmap.load(rutaEdificio);
        if (pixmap.isNull()) {
            pixmap = QPixmap(80, 80);
            pixmap.fill(Qt::darkGray);
        }

        pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);

        if (ladoIzquierdo) {
            // Lado izquierdo de la pantalla
            randomX = QRandomGenerator::global()->bounded(0, 250);
        } else {
            // Lado derecho de la pantalla
            randomX = QRandomGenerator::global()->bounded(550, 800);
        }

        // Edificios: solo se mueven a la velocidad del fondo
        velocidadPropia = 0;
        esSenoidal = false; // por si acaso
    }

    setPos(randomX, -100);
    xOriginal = randomX;
}

void Obstaculo::setMovimientoSenoidal(bool activar)
{
    // Solo los obstáculos de carretera pueden ser senoidales
    if (soloDecoracion) {
        esSenoidal = false;
        return;
    }
    esSenoidal = activar;
}

bool Obstaculo::esSoloDecoracion() const
{
    return soloDecoracion;
}

void Obstaculo::mover(int velocidadEscenario)
{
    int nuevaY = y() + velocidadEscenario + velocidadPropia;
    int nuevaX = x();

    if (esSenoidal && !soloDecoracion) {
        nuevaX = xOriginal + 40 * qSin(nuevaY * 0.02);
    }

    setPos(nuevaX, nuevaY);
}
