#ifndef RECOLECTABLES_H
#define RECOLECTABLES_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QTimer>

class recolectables : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // Constructor que recibe la posición destino
    explicit recolectables(const QPointF &destino, QObject *parent = nullptr);

    // Por si en algún momento se quiere cambiar el destino
    void setDestino(const QPointF &destino);

private slots:
    void actualizarMovimiento();

private:
    QPointF m_destino;     // punto donde debe quedarse
    double  m_velocidadY;  // velocidad vertical actual
    double  m_aceleracion; // aceleración (gravedad)
    QTimer *m_timer;
};

#endif // RECOLECTABLES_H
