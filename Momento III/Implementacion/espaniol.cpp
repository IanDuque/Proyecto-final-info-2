#include "espaniol.h"


short int espaniol:: getreddanio(){
    return reduccion_de_danio;
}
void espaniol:: setreddanio(short int nuevareddanio){
    reduccion_de_danio=nuevareddanio;
}

espaniol:: espaniol(short int vida, short int reduccion_de_danio){
    setvida(vida);
    setreddanio(reduccion_de_danio);
}
short int espaniol::  getvida(){
    return vida;
}
//setters.
void espaniol::  setvida(short int daniorecibido){
    vida-=(daniorecibido-reduccion_de_danio);
}
