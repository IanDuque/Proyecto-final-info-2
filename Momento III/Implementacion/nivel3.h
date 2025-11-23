#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivelbase.h"

class Nivel3 : public NivelBase
{
    Q_OBJECT
public:
    explicit Nivel3(QObject *parent = nullptr);

protected:
    void configurarFondo() override;
    QString getFondoPath() const override;
    QString getObstaculoPath() const override;
    void inicializarJugador() override; // Implementación del Carro
};

#endif // NIVEL3_H
