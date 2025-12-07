#include "recolectables.h"
#include <QPixmap>
#include <QDebug>

recolectables::recolectables(const QPointF &destino, QObject *parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    m_destino(destino),
    m_velocidadY(0.0),
    m_aceleracion(0.35),
    m_timer(new QTimer(this))
{
    // Cargar sprite
    QPixmap img(":/Imagenes/bloque2.png"); // cambia al nombre real
    if (img.isNull()) {
        qDebug() << "Error cargando :/Imagenes/bloque.png";
        img = QPixmap(40, 40);
        img.fill(Qt::yellow);
    }
    setPixmap(img.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Posición inicial: misma X del destino, pero arriba de la pantalla
    setPos(m_destino.x(), -50);

    // Conectar timer de caída
    connect(m_timer, &QTimer::timeout, this, &recolectables::actualizarMovimiento);
    m_timer->start(16); // ~60 FPS
}

void recolectables::setDestino(const QPointF &destino)
{
    m_destino = destino;
}

void recolectables::actualizarMovimiento()
{
    // Movimiento vertical uniformemente acelerado
    m_velocidadY += m_aceleracion;
    qreal nuevaY = y() + m_velocidadY;

    // Si ya alcanzó o pasó la altura del destino, lo fijamos allí
    if (nuevaY >= m_destino.y()) {
        setPos(m_destino.x(), m_destino.y());
        m_timer->stop(); // ya no sigue cayendo
        return;
    }

    setPos(m_destino.x(), nuevaY);
}
