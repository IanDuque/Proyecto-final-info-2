#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    // Señales que la MainWindow conectará para iniciar los niveles
    void nivelSeleccionado(int nivel);

private slots:
    // Slots internos para manejar los clicks de los botones
    void on_botonNivel1_clicked();
    void on_botonNivel2_clicked();
    void on_botonNivel3_clicked();

private:
    // Componentes del menú
    QPushButton *botonNivel1;
    QPushButton *botonNivel2;
    QPushButton *botonNivel3;
};

#endif // MENUWIDGET_H
