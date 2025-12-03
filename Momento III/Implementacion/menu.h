#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QPixmap>
#include <QPaintEvent>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void nivelSeleccionado(int nivel);

private slots:
    void on_botonNivel1_clicked();
    void on_botonNivel2_clicked();
    void on_botonNivel3_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *botonNivel1;
    QPushButton *botonNivel2;
    QPushButton *botonNivel3;

    QPixmap fondomenu;
};

#endif // MENUWIDGET_H
