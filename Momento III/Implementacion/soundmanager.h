#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    static SoundManager& instance() {
        static SoundManager inst;
        return inst;
    }

    void playCarCrash();
    void playCanonShot();
    void playPistolShot();
    void playCollect();

private:
    explicit SoundManager(QObject *parent = nullptr);

    QSoundEffect carCrash;
    QSoundEffect canonShot;
    QSoundEffect pistolShot;
    QSoundEffect collect;
};

#endif // SOUNDMANAGER_H
