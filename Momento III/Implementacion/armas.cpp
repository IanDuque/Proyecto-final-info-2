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
void armas:: tiroparabolico(short int velocidadx, short int velocidady, short int gravedad, short int tiempo){
    //aqui va el metodo de qt que cambia pos en x de manera lineal.
    float posiciony= (velocidady*tiempo)- (gravedad*tiempo*tiempo*0.5); //formula de posicion en y usando tiro parabolico (Vy*t-(0.5*g/t^2)).
    //aqui va el metodo que cambia pos en y basado en la formula de tiro parabolico.
}

