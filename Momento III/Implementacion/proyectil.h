#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Proyectil : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // tipo: 1 = Lanza (Indio), 2 = Bola Cañon (Español)
    Proyectil(double xIni, double yIni, double angulo, double velocidad, int tipo, QObject *parent = nullptr);

private slots:
    void actualizarPosicion();

private:
    QTimer *timer;
    double x, y;          // Posición actual
    double vx, vy;        // Velocidades
    double gravedad;      // Gravedad (ej. 9.8 o ajustada a pixeles)
    int tipoProyectil;    // Para saber a quién daña
};

#endif // PROYECTIL_H
