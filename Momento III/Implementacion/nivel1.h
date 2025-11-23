#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivelbase.h"

class nivel1 : public NivelBase
{
    Q_OBJECT
public:
    explicit nivel1(QObject *parent = nullptr);

protected:
    void configurarFondo() override;
    QString getFondoPath() const override;
    void inicializarJugador() override; // Implementación del Indio
};

#endif // NIVEL1_H
