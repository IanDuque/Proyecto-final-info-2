#ifndef CARRO_H
#define CARRO_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>

class Carro : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Carro(QObject *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;

private:
    int velocidadLateral;
};

#endif // CARRO_H
