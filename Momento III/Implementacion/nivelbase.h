#ifndef NIVELBASE_H
#define NIVELBASE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "carro.h"
#include "obstaculo.h"
#include "explosion.h" // Se incluye la cabecera de la explosión para el manejo de colisiones

class NivelBase : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit NivelBase(QObject *parent = nullptr);
    virtual ~NivelBase(); // Importante: Destructor virtual

    // --- MIEMBROS PÚBLICOS ---
    // Hacemos 'jugador' público para que MainWindow pueda acceder a él y darle el foco.
    Carro *jugador;

signals:
    void nivelTerminado();
    void actualizarHUD(int tiempo, int velocidad);

public slots:
    void gameLoop(); // Implementación del bucle principal del juego.

protected:
    // --- MIEMBROS PROTEGIDOS ---
    QTimer *timerLoop;
    QTimer *timerSpawn;
    QTimer *timerSecond;

    QGraphicsPixmapItem *fondo1;
    QGraphicsPixmapItem *fondo2;

    QList<Obstaculo*> obstaculos; // Lista para gestionar los obstáculos

    int tiempoRestante; // 120 segundos
    int velocidadFondo; // Velocidad de movimiento del fondo y obstáculos
    bool juegoTerminado;

    // Métodos virtuales puros (deben ser implementados por las clases hijas)
    virtual void configurarFondo() = 0;
    virtual QString getFondoPath() const = 0;
    virtual QString getObstaculoPath() const = 0;

    // Métodos internos
    virtual void spawnObstacle();
    virtual void updateTimer();
};

#endif // NIVELBASE_H
