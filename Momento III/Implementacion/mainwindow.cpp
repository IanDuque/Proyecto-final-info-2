#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QDebug>
#include <QMessageBox>
#include <QFont>
#include "nivel1.h"
#include "nivel3.h"
#include "nivel2.h"
#include "carro.h"
#include "indio.h"
#include "espaniol.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), nivelActual(nullptr), numNivelActual(0)
{
    ui->setupUi(this);
    this->setFixedSize(800 + 20, 600 + 60);

    configurarHUD();

    // CREACIÓN DEL WIDGET DEL MENÚ
    menuWidget = new MenuWidget(ui->graphicsView);
    connect(menuWidget, &MenuWidget::nivelSeleccionado, this, &MainWindow::iniciarNivel);

    QGraphicsScene tempScene;
    ui->graphicsView->setScene(&tempScene);
    tempScene.setSceneRect(0, 0, 800, 600);

    // Aplicar el ajuste de tamaño
    ui->graphicsView->setFixedSize(800, 600); // Fija el tamaño del widget QGraphicsView
    this->adjustSize();

    // Revertir a la escena correcta
    ui->graphicsView->setScene(nullptr);

    mostrarMenu();

    ui->graphicsView->fitInView(0, 0, 800, 600, Qt::KeepAspectRatio);

    this->setFixedSize(this->size());

    ui->graphicsView->fitInView(0, 0, 800, 600, Qt::KeepAspectRatio);

    // Conectar la señal del MenuWidget a la lógica de la MainWindow
    connect(menuWidget, &MenuWidget::nivelSeleccionado, this, &MainWindow::iniciarNivel);

    // MOSTRAR EL MENÚ AL INICIAR EL JUEGO
    mostrarMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configurarHUD()
{
    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setFont(QFont("Arial", 16));
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(10, 10);

    textoVelocidad = new QGraphicsTextItem();
    textoVelocidad->setFont(QFont("Arial", 16));
    textoVelocidad->setDefaultTextColor(Qt::white);
    textoVelocidad->setPos(10, 40);

    textoVidas = new QGraphicsTextItem();
    textoVidas->setFont(QFont("Arial", 16));
    textoVidas->setDefaultTextColor(Qt::red); // Rojo para que destaque
    textoVidas->setPos(10, 70); // Debajo de la velocidad

    actualizarHUD(120,0,5);
}

void MainWindow::limpiarNivelActual()
{
    if (nivelActual) {
        // 1. Salvar el HUD: Quitamos los textos de la escena antes de destruirla
        // Si no hacemos esto, la escena borra el texto y el siguiente nivel falla.
        if (textoTiempo && textoTiempo->scene() == nivelActual) {
            nivelActual->removeItem(textoTiempo);
        }
        if (textoVelocidad && textoVelocidad->scene() == nivelActual) {
            nivelActual->removeItem(textoVelocidad);
        }
        if (textoVidas && textoVidas->scene() == nivelActual)
            nivelActual->removeItem(textoVidas);
        // 2. Desconectar señales y borrar el nivel de forma segura
        nivelActual->disconnect();

        // Usamos deleteLater() en lugar de delete para evitar conflictos de eventos pendientes
        nivelActual->deleteLater();

        nivelActual = nullptr;
    }
}

void MainWindow::mostrarMenu()
{
    ui->graphicsView->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFixedSize(800 + 20, 600 + 60);

    // 1. Configurar el fondo de la QGraphicsView con fondomenu.png
    QPixmap bg(":/Imagenes/fondomenu.jpg");
    if (!bg.isNull()) {
        bg = bg.scaled(ui->graphicsView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QBrush brush(bg);
        ui->graphicsView->setBackgroundBrush(brush);
    } else {
        ui->graphicsView->setBackgroundBrush(QBrush(Qt::darkBlue));
    }

    // 2. Ajustar el tamaño y mostrar el widget del menú
    menuWidget->resize(ui->graphicsView->size());
    menuWidget->show();

    // **Ajuste preventivo también en el menú**
    ui->graphicsView->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->fitInView(0, 0, 800, 600, Qt::KeepAspectRatio);
}

void MainWindow::iniciarNivel(int nivel)
{
    if (menuWidget) {
        menuWidget->hide();
    }

    // Limpieza visual
    ui->graphicsView->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFixedSize(800 + 20, 600 + 60);

    limpiarNivelActual();

    numNivelActual = nivel;
    NivelBase *nuevoNivel = nullptr;

    switch (nivel) {
    case 1:
        nuevoNivel = new nivel1(this);
        break;
    case 2:
        // nuevoNivel = new nivel2(this);
        break;
    case 3:
        nuevoNivel = new Nivel3(this);
        break;
    default:
        QMessageBox::information(this, "Fin del Juego", "¡Has completado el último nivel!");
        return;
    }

    if (nuevoNivel) {
        nivelActual = nuevoNivel;
        ui->graphicsView->setScene(nivelActual);
        ui->graphicsView->fitInView(0, 0, 800, 600, Qt::KeepAspectRatio);

        // Agregar HUD
        nivelActual->addItem(textoTiempo);
        nivelActual->addItem(textoVelocidad);
        nivelActual->addItem(textoVidas);
        // Si ya implementaste el textoVidas, agrégalo aquí también
        // nivelActual->addItem(textoVidas);

        textoTiempo->setZValue(10);
        textoVelocidad->setZValue(10);
        textoVidas->setZValue(10);

        connect(nivelActual, &NivelBase::nivelTerminado, this, &MainWindow::cambiarANivelSiguiente);
        // Asegúrate que la señal coincida con tu slot (si agregaste vidas, ajusta aquí)
        connect(nivelActual, &NivelBase::actualizarHUD, this, &MainWindow::actualizarHUD);

        // --- CORRECCIÓN LÓGICA DE CONTROL ---

        // Queremos controlar al personaje en Nivel 1 (Indio) Y en Nivel 3 (Carro)
        bool controlHabilitado = (nivel == 1 || nivel == 3);

        bool usarTiempo = (nivel != 1); // El nivel 1 es batalla, quizás no usa cuenta regresiva
        bool usarSpawn = (nivel != 1);  // El nivel 1 tiene al Español, no obstaculos cayendo

        textoTiempo->setVisible(usarTiempo);
        textoVelocidad->setVisible(nivel == 3); // Solo mostrar velocidad en el carro

        if (nivelActual->jugador) {

            // CORRECCIÓN 1: Usar 'indio' en minúscula si así se llama tu clase
            Carro *carroPtr = dynamic_cast<Carro*>(nivelActual->jugador);
            Indio *indioPtr = dynamic_cast<Indio*>(nivelActual->jugador);

            if (carroPtr) {
                carroPtr->setControlEnabled(controlHabilitado);
            }
            else if (indioPtr) {
                // CORRECCIÓN 2: Pasamos 'controlHabilitado' (true) en lugar de 'controlManualCarro' (false)
                indioPtr->setControlEnabled(controlHabilitado);
            }

            // Foco y Movimiento
            nivelActual->jugador->setFlag(QGraphicsItem::ItemIsFocusable, controlHabilitado);

            if (controlHabilitado) {
                nivelActual->jugador->setFocus();
            } else {
                this->setFocus();
            }
        }

        nivelActual->iniciarTimers(usarTiempo, usarSpawn);
    }
}

void MainWindow::actualizarHUD(int tiempo, int velocidad, int vidas)
{
    textoTiempo->setPlainText(QString("Tiempo: %1 s").arg(tiempo));
    textoVelocidad->setPlainText(QString("Velocidad: %1 km/h").arg(velocidad));
    textoVidas->setPlainText(QString("Vidas: %1").arg(vidas));
}

void MainWindow::cambiarANivelSiguiente()
{
    iniciarNivel(numNivelActual + 1);
}
