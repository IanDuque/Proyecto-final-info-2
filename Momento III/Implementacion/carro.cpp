#include "carro.h"

//getters.
float carro:: getaccely(){
    return accely;
}
//setters.
void carro:: setaccely(float cambioaccely){
    accely=cambioaccely;
}
//constructor.
carro:: carro(float velx, float vely, float accely){
    setvelx(velx);
    setvely(vely);
    setaccely(accely);
}
