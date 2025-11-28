#ifndef NIVELBASE_H
#define NIVELBASE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "obstaculo.h"
#include "explosion.h"
#include <QGraphicsPixmapItem> // Clase base para jugador

class NivelBase : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit NivelBase(QObject *parent = nullptr);
    virtual ~NivelBase();

    // Jugador ahora es de la clase base QGraphicsPixmapItem para aceptar Carro o indio.
    QGraphicsPixmapItem *jugador;

    // Nuevo método: Inicia los timers SÓLO cuando el nivel está listo (evita el SIGSEGV).
    void iniciarTimers();
    void iniciarTimers(bool usarTiempo = true, bool usarSpawn = true);

signals:
    void nivelTerminado();
    void actualizarHUD(int tiempo, int velocidad, int vidas);

public slots:
    void gameLoop();

protected:
    QTimer *timerLoop;
    QTimer *timerSpawn;
    QTimer *timerSecond;

    QGraphicsPixmapItem *fondo1;
    QGraphicsPixmapItem *fondo2;

    QList<Obstaculo*> obstaculos;

    int tiempoRestante;
    int velocidadFondo;
    bool juegoTerminado;

    // Métodos virtuales puros (deben ser implementados por las clases hijas)
    virtual void configurarFondo() = 0;
    virtual QString getFondoPath() const = 0;
    virtual QString getObstaculoPath() const;
    virtual void inicializarJugador() = 0; // NUEVO: Obliga a cada nivel a crear su propio jugador.

    // Métodos internos
    virtual void spawnObstacle();
    virtual void updateTimer();

    int tiempoTotalInicial; // Para saber cuál es la mitad del tiempo
    int vidas;              // Contador de vidas
    bool modoSenoidalActivo; // Flag para saber si ya activamos el modo difícil
};

#endif // NIVELBASE_H
