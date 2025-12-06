#ifndef NIVEL1_H
#define NIVEL1_H
#include "nivelbase.h"
#include "espaniol.h"
#include "indio.h"

class nivel1 : public NivelBase
{
   Q_OBJECT
public:
    explicit nivel1(QObject *parent = nullptr);

protected:
    void configurarFondo() override;
    QString getFondoPath() const override;
    void inicializarJugador() override; // Implementación del Indio
    void onJugadorMuere() override;
    void onEnemigoMuere() override;

private slots:
    void onPersonajeMuerto(Personaje *p);

private:

    bool batallaTerminada = false;
    Indio    *indio   = nullptr;
    Espaniol *espanol = nullptr;
    QGraphicsPixmapItem *spriteEspaniolMuerto = nullptr;
};

#endif // NIVEL1_H
