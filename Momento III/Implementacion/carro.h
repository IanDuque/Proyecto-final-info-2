#ifndef CARRO_H
#define CARRO_H
#include "movimientos.h"

class carro : public movimientos{
private:
private:
    float velx;
    float vely;
    float accely;
public:
    //getters.
    float getaccely();
    //setters.
    void setaccely(float cambioaccely);
    //constructor.
    carro(float velx, float vely, float accely);
};

#endif // CARRO_H
