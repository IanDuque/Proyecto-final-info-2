#include "explosion.h"
#include <QTimer>
#include <QPixmap>

Explosion::Explosion(QObject *parent) : QObject(parent)
{
    // 1. Cargar la imagen gigante
    QPixmap pixmap(":/Imagenes/explosion.png"); // Asegúrate que el nombre coincida en tu .qrc

    // 2. ESCALAR: De 1024x1024 a 80x80
    // Usamos KeepAspectRatio para que no se deforme si no es perfectamente cuadrada
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Fallback: Cuadro rojo si no encuentra la imagen
        QPixmap error(80, 80);
        error.fill(Qt::red);
        setPixmap(error);
    }

    // 3. AUTODESTRUCCIÓN
    // Después de 500 milisegundos (0.5 segundos), este objeto se elimina solo.
    // Esto borra la imagen de la pantalla y libera la memoria.
    QTimer::singleShot(500, this, &QObject::deleteLater);
}
