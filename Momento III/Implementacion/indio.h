#ifndef INDIO_H
#define INDIO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>

class indio : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    short int vida;
    short int defensa;
    bool controlEnabled = false;
public:
    explicit indio(short int vida_inicial = 100, short int defensa_inicial = 100, QObject *parent = nullptr);

    // Getters.
    short int getdefensa() const;
    short int getvida() const;

    // Setters.
    void setvida(short int nuevavida);
    void setdefensa(short int cambiodefensa);

    // Metodos.
    void cargarSprite(const QString& rutaSprite);
    void setControlEnabled(bool enable);
};

#endif // INDIO_H
