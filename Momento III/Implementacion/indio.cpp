#include "indio.h"

//getters y setters de la clase indio.
short int indio:: getdefensa(){
    return defensa;
}
void indio:: setdefensa(short int cambiodefensa){
    defensa=cambiodefensa;
}
//constructor de objeto indio.
indio::indio(short int vida,short int defensa){
    setvida(vida);
    setdefensa(defensa);
}
short int indio:: getvida(){
    return vida;
}

//setters.
void indio:: setvida(short int cambiovida){
    vida-=cambiovida;
}
