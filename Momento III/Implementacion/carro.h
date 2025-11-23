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
    void setControlEnabled(bool enable);

private:
    int velocidadLateral;
    bool controlEnabled = false;
};

#endif // CARRO_H
