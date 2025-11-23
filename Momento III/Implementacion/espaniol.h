#ifndef ESPANIOL_H
#define ESPANIOL_H

class espaniol{
private:
    short int vida;
    short int reduccion_de_danio;
public:
    //getters y setters.
    short int getreddanio() const;
    short int getvida() const;
    void setvida(short int daniorecibido);
    void setreddanio(short int nuevareddanio);
    //constructor.
    espaniol(short int vida, short int reduccion_de_danio);
};

#endif // ESPANIOL_H
