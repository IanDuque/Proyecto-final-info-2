#ifndef ESPANIOL_H
#define ESPANIOL_H

class espaniol{
private:
    short int vida;
    short int reduccion_de_danio;
public:
    //getters y setters de la clase español.
    short int getreddanio();
    short int getvida();
    void setvida(short int cambiovida);
    void setreddanio(short int nuevareddanio);
    //constructor.
    espaniol(short int vida, short int reduccion_de_danio);
};

#endif // ESPANIOL_H
