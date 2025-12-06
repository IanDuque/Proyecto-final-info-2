#include "recolectables.h"
#include <QPixmap>
#include <QDebug>

recolectables::recolectables(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    QPixmap px(":/Imagenes/bloque2.png"); // sprite del objeto a recolectar
    if (px.isNull()) {
        qDebug() << "Error cargando :/Imagenes/bloque2.png";
        px = QPixmap(50, 50);
        px.fill(Qt::yellow);
    }
    setPixmap(px.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
