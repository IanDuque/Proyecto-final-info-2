#include "armas.h"

//getters.
short int armas:: getdanio(){
    return danio;
}
short int armas:: getvelataq(){
    return velataq;
}
//setters.
void armas:: setdanio(short int nuevodanio){
    danio=nuevodanio;
}
void armas:: setvelataq(short int nuevavelataq){
    velataq=nuevavelataq;
}
//constructor.
armas:: armas(short int danio, short int velataq){
    setdanio(danio);
    setvelataq(velataq);
}
