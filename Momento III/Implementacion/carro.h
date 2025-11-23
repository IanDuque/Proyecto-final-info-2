#ifndef CARRO_H
#define CARRO_H

<<<<<<< HEAD
class carro : public movimientos{
private:
    float velx;
    float vely;
public:
    //constructor.
    carro(float velx, float vely);
    //metodos.
    void aceleracion(short int metros, short int segundos);
    void setvelx(float nuevavelx){
        velx= nuevavelx;
    }
    void setvely(float nuevavely){
        vely= nuevavely;
    }
=======
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
>>>>>>> origin/Angel
};

#endif // CARRO_H
