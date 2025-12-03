#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Proyectil : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // 1 = Lanza
    // 2 = Bala de cañón
    // 3 = Bala de pistola
    Proyectil(double xIni, double yIni, double angulo, double velocidad, int tipo, QObject *parent = nullptr);

private slots:
    void actualizarPosicion();

private:
    QTimer *timer;

    // Posición actual
    double x;
    double y;

    // Velocidades
    double vx;
    double vy;

    // Física
    double gravedad;

    // Tipo de proyectil
    int tipoProyectil;
};

#endif // PROYECTIL_H
