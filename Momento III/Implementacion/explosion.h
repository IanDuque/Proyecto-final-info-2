#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QPixmap>

class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Explosion(QObject *parent = nullptr);

private slots:
    void siguienteFrame(); // Slot para cambiar la imagen

private:
    QList<QPixmap> sprites; // Almacena las 4 imágenes
    int currentFrame;       // Rastrea qué imagen se está mostrando
    QTimer *timerAnimacion; // Controla la velocidad de la animación
};

#endif // EXPLOSION_H
