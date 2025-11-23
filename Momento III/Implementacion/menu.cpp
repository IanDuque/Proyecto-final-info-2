#include "menu.h"
#include <QGridLayout>
#include <QDebug>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    // Usamos QGridLayout para centrar los botones
    QGridLayout *layout = new QGridLayout(this);

    // Crear botones
    botonNivel1 = new QPushButton("Nivel 1");
    botonNivel2 = new QPushButton("Nivel 2");
    botonNivel3 = new QPushButton("Nivel 3");

    // Estilo básico
    botonNivel1->setFixedSize(150, 40);
    botonNivel2->setFixedSize(150, 40);
    botonNivel3->setFixedSize(150, 40);

    // Añadir al layout (centrados)
    layout->addWidget(botonNivel1, 0, 0, Qt::AlignCenter);
    layout->addWidget(botonNivel2, 1, 0, Qt::AlignCenter);
    layout->addWidget(botonNivel3, 2, 0, Qt::AlignCenter);

    // Conectar los botones a los slots internos
    connect(botonNivel1, &QPushButton::clicked, this, &MenuWidget::on_botonNivel1_clicked);
    connect(botonNivel2, &QPushButton::clicked, this, &MenuWidget::on_botonNivel2_clicked);
    connect(botonNivel3, &QPushButton::clicked, this, &MenuWidget::on_botonNivel3_clicked);
}

// Implementación de los slots: emitir la señal con el número de nivel
void MenuWidget::on_botonNivel1_clicked()
{
    emit nivelSeleccionado(1);
}

void MenuWidget::on_botonNivel2_clicked()
{
    emit nivelSeleccionado(2);
}

void MenuWidget::on_botonNivel3_clicked()
{
    emit nivelSeleccionado(3);
}
