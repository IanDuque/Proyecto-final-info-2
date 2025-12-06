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

    void setMovimientoSenoidal(bool activar);

    // 🔹 Nuevo: para saber si es solo decorativo (edificio fuera de la carretera)
    bool esSoloDecoracion() const;

private:
    int velocidadPropia;     // Velocidad extra individual de cada obstáculo

    // Variables para el movimiento senoidal
    bool esSenoidal;
    int xOriginal;

    // 🔹 Nuevo: bandera para edificios (no colisionan, no senoidal)
    bool soloDecoracion;
};

#endif // OBSTACULO_H
