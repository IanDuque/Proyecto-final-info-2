#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QString>

class Obstaculo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // El constructor ahora recibe una ruta base (ej: ":/imagenes/obstaculo")
    explicit Obstaculo(const QString &basePath, QObject *parent = nullptr);

    // Mueve el obstáculo hacia abajo
    void mover(int velocidadEscenario);

private:
    int velocidadPropia; // Velocidad extra individual de cada obstáculo
};

#endif // OBSTACULO_H
