#include "personaje.h"

Personaje::Personaje(int vidaInicial, QObject *parent)
    : QObject(parent), vida(vidaInicial), vidaMax(vidaInicial)
{
}

int Personaje::getVida() const { return vida; }

void Personaje::recibirDanio(int danio) {
    vida -= danio;
    if (vida < 0) vida = 0;
}

bool Personaje::estaVivo() const { return vida > 0; }
