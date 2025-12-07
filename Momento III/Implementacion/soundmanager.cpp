#include "soundmanager.h"

SoundManager::SoundManager(QObject *parent)
    : QObject(parent)
{
    // choque del carro
    carCrash.setSource(QUrl("qrc:/audios/choquecarro.wav"));
    carCrash.setVolume(0.9); // 90%

    // disparo del cañón
    canonShot.setSource(QUrl("qrc:/audios/disparocanon.wav"));
    canonShot.setVolume(0.6); // 60%

    // disparo de pistola
    pistolShot.setSource(QUrl("qrc:/audios/disparopistola.wav"));
    pistolShot.setVolume(0.4); // 40%

    // recolección
    collect.setSource(QUrl("qrc:/audios/recoleccion.wav"));
    collect.setVolume(1.0); // 100%
}

void SoundManager::playCarCrash() { carCrash.play(); }
void SoundManager::playCanonShot() { canonShot.play(); }
void SoundManager::playPistolShot() { pistolShot.play(); }
void SoundManager::playCollect() { collect.play(); }
