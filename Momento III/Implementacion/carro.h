#ifndef CARRO_H
#define CARRO_H
#include "movimientos.h"

class carro : public movimientos{
private:
private:
    float velx;
    float vely;
public:
    //constructor.
    carro(float velx, float vely);
    //metodos.
    void aceleracion(short int metros, short int segundos);
};

#endif // CARRO_H
