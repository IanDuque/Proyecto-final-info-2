#ifndef ARMAS_H
#define ARMAS_H

class armas{
private:
    short int danio;
    short int velataq;
public:
    //getters.
    short int getdanio() const;
    short int getvelataq() const;
    //setters.
    void setdanio(short int nuevodanio);
    void setvelataq(short int nuevavelataq);
    //constructor.
    armas(short int danio, short int velataq);
    //metodos.
};

#endif // ARMAS_H
