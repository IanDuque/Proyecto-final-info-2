#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QGraphicsTextItem>
#include "carro.h"
#include "obstaculo.h"
#include "nivelbase.h" // Importante: Clase Base para todos los niveles
#include "nivel3.h"    // Importante: Clase del Nivel 3 (Desierto)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // Slot para iniciar un nivel específico
    void iniciarNivel(int nivel);
    // Slot llamado por la señal 'nivelTerminado'
    void cambiarANivelSiguiente();
    // Slot llamado por la señal 'actualizarHUD' del nivel
    void actualizarHUD(int tiempo, int velocidad);

private:
    Ui::MainWindow *ui;

    NivelBase *nivelActual; // Puntero al nivel que se está ejecutando
    int numNivelActual;

    // UI en pantalla (Ahora propiedad de MainWindow, no del Nivel)
    QGraphicsTextItem *textoTiempo;
    QGraphicsTextItem *textoVelocidad;

    void configurarHUD();
    void limpiarNivelActual(); // Gestiona la memoria del nivel anterior
};
#endif // MAINWINDOW_H
