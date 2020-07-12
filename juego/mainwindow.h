#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>

#include "personaje.h"
#include "ataques.h"
#include "enemigo.h"

#include <QTimer>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool activacion = true;             // para confirmar, por ahora no esta en uso
    int marcador = 0;


private slots:
    void on_pushButton_clicked();
    void Mover();                       //          para actualizar las posiciones
    void add_enemigo();                 //          para añadir enemigo
    void juego_terminado();             //          para anunciar game over

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *Scene = new QGraphicsScene;
    QGraphicsView *view;

    QTimer *timer;
    QTimer *tiempo_enemigos;

    personaje *cuerpo;
    ataques *bala;

    QList<ataques*> balas;
    QList<enemigo*> enemy;

    void keyPressEvent(QKeyEvent *evento);
};
#endif // MAINWINDOW_H
