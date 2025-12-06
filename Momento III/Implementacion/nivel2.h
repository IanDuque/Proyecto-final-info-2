#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivelbase.h"
#include <QList>
#include <QVector>

class Egipcio;
class Bloques;
class recolectables;
class QGraphicsTextItem;

class nivel2 : public NivelBase
{
    Q_OBJECT
public:
    explicit nivel2(QObject *parent = nullptr);

    void incrementarRecolectados(recolectables *bloque);

protected:
    void configurarFondo() override;
    QString getFondoPath() const override;
    QString getObstaculoPath() const override;
    void inicializarJugador() override;
    void updateTimer() override;

private:
    Egipcio *egipcio;
    QList<Bloques*> paredes;
    QList<recolectables*> coleccionables;

    QGraphicsTextItem *textoRecolectados;
    int totalPorRecolectar;
    int recolectados;

    QVector<QPointF> posicionesrecolectables;  // coordenadas de los recoger (las irás afinando luego)

    void crearLaberinto();
    void crearColeccionables();
    void actualizarTextoRecolectados();
};

#endif // NIVEL2_H
