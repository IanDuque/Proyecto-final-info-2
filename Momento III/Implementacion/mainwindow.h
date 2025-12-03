#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include "nivelbase.h"
#include "menu.h" // Incluir el nuevo widget de menú

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel(int nivel);
    void actualizarHUD(int tiempo, int velocidad, int vidas);
    void cambiarANivelSiguiente();
    void mostrarMenu();

private:
    Ui::MainWindow *ui;
    NivelBase *nivelActual;
    int numNivelActual;

    QGraphicsTextItem *textoTiempo;
    QGraphicsTextItem *textoVelocidad;
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoVidaIndio;
    QGraphicsTextItem *textoVidaEspaniol;

    MenuWidget *menuWidget; // Variable para el widget del menú

    void limpiarNivelActual();
    void configurarHUD();
};

#endif // MAINWINDOW_H
