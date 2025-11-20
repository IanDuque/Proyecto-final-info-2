#include "carro.h"

//constructor.
carro:: carro(float velx, float vely){
    setvelx(velx);
    setvely(vely);
}
//metodos.
void carro:: aceleracion(short int metros, short int segundos){
    vely=metros*(segundos*segundos); //formula de aceleracion uniforme (ms^2).
}
