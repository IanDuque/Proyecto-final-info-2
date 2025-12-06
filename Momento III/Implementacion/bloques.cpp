#include "bloques.h"
#include <QPixmap>
#include <QDebug>

Bloques::Bloques(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    QPixmap px(":/Imagenes/bloque1.png"); // nombre del sprite de pared
    if (px.isNull()) {
        qDebug() << "Error cargando :/Imagenes/bloque1.png";
        px = QPixmap(60, 60);
        px.fill(Qt::darkGray);
    }
    setPixmap(px.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
