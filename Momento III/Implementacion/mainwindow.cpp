#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFont>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Configuración de la Ventana (Tamaño fijo del juego)
    this->setFixedSize(400,600);
    // Aseguramos que el QGraphicsView ocupe toda la ventana
    ui->graphicsView->setGeometry(0, 0, 400, 600);

    // 2. Inicializar variables
    nivelActual = nullptr;
    numNivelActual = 0;

    // 3. Configurar la interfaz HUD (los QGraphicsTextItem)
    configurarHUD();

    // 4. Iniciar el primer nivel (Nivel 3 como prueba)
    // Esto llamará a iniciarNivel(3)
    iniciarNivel(3);
}

MainWindow::~MainWindow()
{
    limpiarNivelActual();
    delete ui;
}

void MainWindow::configurarHUD()
{
    // Crear los textos de HUD (que se añadirán a la escena cuando el nivel se cargue)
    textoTiempo = new QGraphicsTextItem("Tiempo: 120");
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setFont(QFont("Arial", 16, QFont::Bold));
    textoTiempo->setPos(10, 10); // Posición en la esquina superior izquierda

    textoVelocidad = new QGraphicsTextItem("Vel: 100%");
    textoVelocidad->setDefaultTextColor(Qt::yellow);
    textoVelocidad->setFont(QFont("Arial", 16, QFont::Bold));
    textoVelocidad->setPos(280, 10); // Posición en la esquina superior derecha
}

void MainWindow::limpiarNivelActual()
{
    if (nivelActual) {
        // Desconectar cualquier señal pendiente
        disconnect(nivelActual, &NivelBase::nivelTerminado, this, &MainWindow::cambiarANivelSiguiente);
        disconnect(nivelActual, &NivelBase::actualizarHUD, this, &MainWindow::actualizarHUD);

        // Limpiar y destruir la escena del nivel anterior
        delete nivelActual;
        nivelActual = nullptr;
    }
}

void MainWindow::iniciarNivel(int nivel)
{
    limpiarNivelActual(); // Limpia el nivel anterior si existe

    numNivelActual = nivel;
    NivelBase *nuevoNivel = nullptr;

    switch (nivel) {
    case 3:
        nuevoNivel = new Nivel3(this); // Nivel 3 (Desierto)
        break;
    default:
        // Manejo de otros niveles o fin del juego
        QMessageBox::information(this, "Fin del Juego", "¡Has completado todos los niveles!");
        return;
    }

    if (nuevoNivel) {
        nivelActual = nuevoNivel;
        ui->graphicsView->setScene(nivelActual);

        // ** CONFIGURACIÓN DE VISTA **
        // Asegurar que la vista muestre toda la escena (400x600) sin barras
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        // Mostrar el área completa de la escena (0,0) a (400,600)
        ui->graphicsView->setSceneRect(0, 0, 400, 600);
        // ** NO usar centerOn(), ya que el movimiento es simulado por el fondo **

        // 1. Añadir HUD (los textos) a la nueva escena y darles alta prioridad (Z=10)
        nivelActual->addItem(textoTiempo);
        nivelActual->addItem(textoVelocidad);
        textoTiempo->setZValue(10);
        textoVelocidad->setZValue(10);

        // 2. Conectar señales del nivel al controlador (MainWindow)
        connect(nivelActual, &NivelBase::nivelTerminado, this, &MainWindow::cambiarANivelSiguiente);
        connect(nivelActual, &NivelBase::actualizarHUD, this, &MainWindow::actualizarHUD);

        // 3. Asegurar que el carro tenga el foco para recibir input
        if (nivelActual->jugador) {
            nivelActual->jugador->setFlag(QGraphicsItem::ItemIsFocusable);
            nivelActual->jugador->setFocus();
        }

        qDebug() << "Iniciado Nivel" << nivel;
    }
}

void MainWindow::cambiarANivelSiguiente()
{
    QMessageBox::information(this, "Nivel Superado", QString("¡Nivel %1 Superado!").arg(numNivelActual));
    // inicia el siguiente nivel, por ejemplo, el 4
    iniciarNivel(numNivelActual + 1);
}

void MainWindow::actualizarHUD(int tiempo, int velocidad)
{
    textoTiempo->setPlainText("Tiempo: " + QString::number(tiempo));
    textoVelocidad->setPlainText("Vel: " + QString::number(velocidad) + "%");
}
// NOTA: No necesitamos keyPressEvent en MainWindow, ya que el foco se da al jugador (Carro).
// Si el foco se pierde, el jugador no se moverá. Asegúrate de que no haya otros widgets
// recibiendo foco.
