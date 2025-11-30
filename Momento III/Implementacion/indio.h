#ifndef INDIO_H
#define INDIO_H

#include "personaje.h" // <--- Importante: Incluimos al padre
#include <QKeyEvent>
#include <QTimer>

// Heredamos de Personaje.
// NOTA: Ya no hace falta poner QObject ni QGraphicsPixmapItem aquí
// porque Personaje ya los tiene.
class Indio : public Personaje
{
    Q_OBJECT

public:
    // Constructor
    explicit Indio(int vida_inicial = 100, int defensa_inicial = 100, QObject *parent = nullptr);

    // Getters y Setters propios de Indio
    int getdefensa() const;
    void setdefensa(int cambiodefensa);

    // Métodos específicos
    void actualizarAnimacion();
    void setControlEnabled(bool enable);

    // Sobrescribimos recibirDanio para añadir lógica extra si quieres (logs, etc)
    void recibirDanio(int valor);

    // Eventos
    void disparar();
    void keyPressEvent(QKeyEvent *event) override;

private:
    // 'vida' YA NO ESTÁ AQUÍ. Usamos la de Personaje.
    int defensa;
    bool controlEnabled;

    // Variables de Animación (Se mantienen igual)
    QTimer *timerAnimacion;
    QPixmap spriteSheetQuieto;
    QPixmap spriteSheetCorriendo;
    QPixmap *spriteActual;
    int frameActual;
    int numFrames;
    int anchoFrame;
    int altoFrame;
    bool isMoving;
};

#endif // INDIO_H
