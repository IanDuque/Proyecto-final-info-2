#ifndef RECOLECTABLES_H
#define RECOLECTABLES_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class recolectables : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit recolectables(QObject *parent = nullptr);

    // Se llama desde nivel2 para “reiniciar” el bloque en una nueva posición
    void resetear(const QPointF &posicionInicial);

    // Opcional: por si quieres arrancar/parar el movimiento desde fuera
    void iniciarCaida();
    void detenerCaida();

signals:
    // Por si quieres avisar al nivel cuando el bloque se salga de la pantalla
    void salioDePantalla(recolectables *self);

public slots:
    void actualizarMovimiento();   // <-- ESTE ES EL SLOT

private:
    QTimer timerMovimiento;
    qreal  velocidadY;             // velocidad vertical
    qreal  aceleracionY;           // “gravedad”
};

#endif // RECOLECTABLES_H
