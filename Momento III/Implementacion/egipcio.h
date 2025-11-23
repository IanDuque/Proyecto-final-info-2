#ifndef EGIPCIO_H
#define EGIPCIO_H
#include "movimientos.h"

class egipcio : public movimientos {
private:
    float velx;
    float vely;
public:
    //constructor.
    egipcio(float velx, float vely);
};

#endif // EGIPCIO_H
