#include "explosion.h"
#include <QDebug>

Explosion::Explosion(QObject *parent) : QObject(parent)
{
    // 1. Cargar los 4 sprites en la lista
    for (int i = 1; i <= 4; ++i) {
        // Construye la ruta dinámicamente: ":/Imagenes/explosion1.png", etc.
        QString path = QString(":/Imagenes/explosion%1.png").arg(i);
        QPixmap pix(path);

        if (pix.isNull()) {
            qWarning() << "Error cargando sprite explosión:" << path;
            // Fallback: cuadro rojo si falla alguna imagen
            pix = QPixmap(80, 80);
            pix.fill(Qt::red);
        } else {
            // Escalar a un tamaño visible (ej. 80x80)
            pix = pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        sprites.append(pix);
    }

    // 2. Configurar estado inicial
    currentFrame = 0;
    if (!sprites.isEmpty()) {
        setPixmap(sprites[0]); // Mostrar la primera imagen
    }

    // 3. Configurar Timer para la animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Explosion::siguienteFrame);

    // Iniciar timer: cambia de imagen cada 100ms (ajusta este valor para más rápido/lento)
    timerAnimacion->start(100);
}

void Explosion::siguienteFrame()
{
    currentFrame++; // Avanzar al siguiente frame

    if (currentFrame < sprites.size()) {
        // Si aún hay imágenes, mostrar la siguiente
        setPixmap(sprites[currentFrame]);
    } else {
        // Si ya mostramos las 4, detener y destruir el objeto
        timerAnimacion->stop();
        this->deleteLater(); // Eliminar de la escena y memoria
    }
}
