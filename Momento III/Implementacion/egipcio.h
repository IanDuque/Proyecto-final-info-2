#ifndef EGIPCIO_H
#define EGIPCIO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QPixmap>

class nivel2;
class Bloque1;
class Bloque2;

class Egipcio : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Egipcio(nivel2 *nivel = nullptr, QObject *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void detenerAcciones();

private slots:
    void actualizarAnimacion();
    void actualizarMovimiento();

private:
    enum Direccion {
        Frente,
        Espalda,
        Izquierda,
        Derecha
    };

    nivel2 *m_nivel;

    // Animación
    QTimer *timerAnimacion;
    QTimer *timerMovimiento;

    QPixmap spriteQuietoFrente;

    QList<QPixmap> framesFrente;
    QList<QPixmap> framesEspalda;
    QList<QPixmap> framesIzquierda;
    QList<QPixmap> framesDerecha;

    int frameActual;
    Direccion direccionActual;
    bool enMovimiento;

    // Estado de teclas
    bool teclaArriba;
    bool teclaAbajo;
    bool teclaIzquierda;
    bool teclaDerecha;

    int velocidad;

    // Métodos auxiliares
    bool hayMovimiento() const;
};

#endif // EGIPCIO_H
