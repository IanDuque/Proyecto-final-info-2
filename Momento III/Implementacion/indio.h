#ifndef INDIO_H
#define INDIO_H

class indio{
private:
    short int vida;
    short int defensa;
public:
    //constructor.
    indio(short int vida, short int defensa);
    //getters y setters.
    short int getdefensa();
    short int getvida();
    void setvida(short int nuevavida);
    void setdefensa(short int cambiodefensa);
};

#endif // INDIO_H
