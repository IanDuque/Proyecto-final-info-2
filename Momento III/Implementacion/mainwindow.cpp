#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QBrush>
#include <QDebug>
#include <QFont>

#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include "menu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    nivelActual(nullptr),
    numNivelActual(0),
    menuWidget(nullptr),
    musicPlayer(nullptr),
    audioOutput(nullptr)
{
    ui->setupUi(this);

    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    musicPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.4); // volumen 40%

    this->setFixedSize(820, 660);

    configurarHUD();

    // Crear menú y conectarlo
    menuWidget = new MenuWidget(this);
    connect(menuWidget, &MenuWidget::nivelSeleccionado, this, &MainWindow::iniciarNivel);

    mostrarMenu();
}

MainWindow::~MainWindow()
{
    limpiarNivelActual();
    delete ui;
}


// =====================================================
//                      HUD
// =====================================================

void MainWindow::configurarHUD()
{
    textoTiempo = new QGraphicsTextItem("Tiempo: ");
    textoTiempo->setFont(QFont("Arial", 16));
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(10, 10);

    textoVelocidad = new QGraphicsTextItem("Velocidad:");
    textoVelocidad->setFont(QFont("Arial", 16));
    textoVelocidad->setDefaultTextColor(Qt::white);
    textoVelocidad->setPos(10, 40);

    textoVidas = new QGraphicsTextItem("Vidas:");
    textoVidas->setFont(QFont("Arial", 16));
    textoVidas->setDefaultTextColor(Qt::red);
    textoVidas->setPos(10, 70);

    textoVidaIndio = new QGraphicsTextItem("Indio:");
    textoVidaIndio->setFont(QFont("Arial", 16));
    textoVidaIndio->setDefaultTextColor(Qt::black);
    textoVidaIndio->setPos(600, 10);

    textoVidaEspaniol = new QGraphicsTextItem("Español:");
    textoVidaEspaniol->setFont(QFont("Arial", 16));
    textoVidaEspaniol->setDefaultTextColor(Qt::red);
    textoVidaEspaniol->setPos(600, 40);
}


// =====================================================
//      ELIMINAR ESCENA PREVIA SIN CRASHEAR
// =====================================================

void MainWindow::limpiarNivelActual()
{
    if (!nivelActual)
        return;

    nivelActual->disconnect();

    // Retirar HUD si estaba en la escena
    for (QGraphicsTextItem *item :{textoTiempo, textoVelocidad, textoVidas, textoVidaIndio, textoVidaEspaniol})
    {
        if (item && item->scene())
            item->scene()->removeItem(item);
    }

    ui->graphicsView->setScene(nullptr);

    nivelActual->deleteLater();
    nivelActual = nullptr;
}


// =====================================================
//                      MENÚ PRINCIPAL
// =====================================================

void MainWindow::mostrarMenu()
{
    limpiarNivelActual();
    if (musicPlayer)
        musicPlayer->stop();

    ui->graphicsView->setScene(nullptr);

    // Fondo del menú
    QPixmap bg(":/Imagenes/fondomenu.jpg");
    if (!bg.isNull())
        ui->graphicsView->setBackgroundBrush(QBrush(bg.scaled(800, 600, Qt::IgnoreAspectRatio)));
    else
        ui->graphicsView->setBackgroundBrush(QBrush(Qt::darkBlue));

    menuWidget->setGeometry(0, 0, 800, 600);
    menuWidget->show();
}


// =====================================================
//                      INICIAR NIVEL
// =====================================================

void MainWindow::iniciarNivel(int nivel)
{
    menuWidget->hide();

    limpiarNivelActual();

    NivelBase *nuevoNivel = nullptr;

    switch (nivel)
    {
    case 1:
        nuevoNivel = new nivel1(this);
        break;
    case 2:
        nuevoNivel = new nivel2(this);
        break;
    case 3:
        nuevoNivel = new Nivel3(this);
        break;
    case 4:
        mostrarMenu();
        return;
    }

    if (!nuevoNivel)
        return;

    nivelActual = nuevoNivel;

    ui->graphicsView->setScene(nivelActual);
    ui->graphicsView->fitInView(0, 0, 800, 600, Qt::KeepAspectRatio);

    // Inicia la cancion correspondiente
    iniciarMusicaNivel(nivel);

    // Añadir HUD a la escena
    nivelActual->addItem(textoTiempo);
    nivelActual->addItem(textoVelocidad);
    nivelActual->addItem(textoVidas);
    nivelActual->addItem(textoVidaIndio);
    nivelActual->addItem(textoVidaEspaniol);

    textoTiempo->setZValue(10);
    textoVelocidad->setZValue(10);
    textoVidas->setZValue(10);
    textoVidaIndio->setZValue(10);
    textoVidaEspaniol->setZValue(10);

    // Control de jugador
    if (nivelActual->jugador)
    {
        nivelActual->jugador->setFlag(QGraphicsItem::ItemIsFocusable, true);
        nivelActual->jugador->setFocus();
    }

    // Conectar señales
    connect(nivelActual, &NivelBase::nivelTerminado, this, &MainWindow::cambiarANivelSiguiente);
    connect(nivelActual, &NivelBase::actualizarHUD, this, &MainWindow::actualizarHUD);
    connect(nivelActual, &NivelBase::actualizarVidaIndio, this, [this](int vida){ textoVidaIndio->setPlainText(QString("Indio: %1").arg(vida)); });

    connect(nivelActual, &NivelBase::actualizarVidaEspaniol, this, [this](int vida){ textoVidaEspaniol->setPlainText(QString("Español: %1").arg(vida)); });

    // Personalizar visibilidad del HUD por nivel
    if (nivel == 1)
    {
        textoTiempo->setVisible(false);
        textoVelocidad->setVisible(false);
        textoVidas->setVisible(false);
        textoVidaIndio->setVisible(true);
        textoVidaEspaniol->setVisible(true);
    }
    else if (nivel == 2)
    {
        textoTiempo->setVisible(true);
        textoVelocidad->setVisible(false);
        textoVidas->setVisible(false);
        textoVidaIndio->setVisible(false);
        textoVidaEspaniol->setVisible(false);
    }
    else if (nivel == 3)
    {
        textoTiempo->setVisible(true);
        textoVelocidad->setVisible(true);
        textoVidas->setVisible(true);
        textoVidaIndio->setVisible(false);
        textoVidaEspaniol->setVisible(false);
    }

    bool usarTiempo = (nivel != 1);
    bool usarSpawn = (nivel == 3);

    nivelActual->iniciarTimers(usarTiempo, usarSpawn);
}


// =====================================================
//               HUD AUTOMÁTICO DURANTE EL NIVEL
// =====================================================

void MainWindow::actualizarHUD(int tiempo, int velocidad, int vidas)
{
    textoTiempo->setPlainText(QString("Tiempo: %1 s").arg(tiempo));
    textoVelocidad->setPlainText(QString("Velocidad: %1 km/h").arg(velocidad));
    textoVidas->setPlainText(QString("Vidas: %1").arg(vidas));
}


// =====================================================
//        REGRESAR AL MENÚ CUANDO ACABA UN NIVEL
// =====================================================

void MainWindow::cambiarANivelSiguiente()
{
    detenerMusica();
    limpiarNivelActual();
    mostrarMenu();
}

// ==========================================================
//        REPRODUCE CANCION SEGUN EL NIVEL QUE ESTE ACTIVO
// ==========================================================

void MainWindow::iniciarMusicaNivel(int nivel)
{
    // Si ya había algo sonando, lo paramos
    detenerMusica();

    // Creamos los objetos si aún no existen
    if (!musicPlayer) {
        musicPlayer = new QMediaPlayer(this);
    }
    if (!audioOutput) {
        audioOutput = new QAudioOutput(this);
        musicPlayer->setAudioOutput(audioOutput);
        audioOutput->setVolume(0.5); // 50%
    }

    // Elegir la pista según el nivel
    switch (nivel) {
    case 1:
        musicPlayer->setSource(QUrl("qrc:/audios/audionivel1.wav"));
        break;
    case 2:
        musicPlayer->setSource(QUrl("qrc:/audios/audionivel2.wav"));
        break;
    case 3:
        musicPlayer->setSource(QUrl("qrc:/audios/audionivel3.wav"));
        break;
    default:
        // menú u otra cosa: sin música o una general si quieres
        musicPlayer->setSource(QUrl());
        return;
    }

    musicPlayer->setLoops(QMediaPlayer::Infinite);
    musicPlayer->play();
}

void MainWindow::detenerMusica()
{
    if (musicPlayer) {
        musicPlayer->stop();
    }
}
