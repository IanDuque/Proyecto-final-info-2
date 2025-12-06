#ifndef INDIO_H
#define INDIO_H

#include "personaje.h"
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QList>

class Indio : public Personaje
{
    Q_OBJECT

public:
    explicit Indio(int vida_inicial = 100, QObject *parent = nullptr);

    void actualizarAnimacion();
    void setControlEnabled(bool enable);

    void recibirDanio(int valor);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void detenerAcciones();

    void disparar();
    void setLanzaLibre();

private slots:
    void actualizarFisica();   // para el salto

private:
    // Control general
    bool controlEnabled;
    bool lanzaEnAire;

    // Animación
    QTimer *timerAnimacion;
    QPixmap spriteQuieto;
    QList<QPixmap> framesCorrer;
    int frameActual;

    // Estado de movimiento horizontal (para animación)
    bool izquierda;
    bool derecha;

    // Física del salto
    QTimer *timerFisica;
    double vy;             // velocidad vertical
    double gravedad;       // aceleración hacia abajo
    double velocidadSalto; // impulso inicial del salto
    double ySuelo;         // altura del suelo (coincide con la posición inicial en Y)
    bool saltando;
    bool enSuelo;
};

#endif // INDIO_H
