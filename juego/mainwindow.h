#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QKeyEvent>

#include "personaje.h"
#include "personaje2.h"
#include "ataques.h"
#include "enemigo.h"
#include "ataques_enemigos.h"
#include "jefes.h"

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
    int marcador            = 0;
    int verificador         = 0;                //  este es para saber si entra en escena un boss y no spawnear enemigos basicos
    int cuenta_enemigos     = 1;                //emmpieza en uno para que no aparezca jefe empezando
    int pantalla            = 0;
    bool multijugador       = 0;


private slots:
    void Mover();                       //          para actualizar las posiciones
    void add_enemigo();                 //          para añadir enemigo
    void add_boss();                 //          para añadir jefe
    void juego_terminado();             //          para anunciar game over

    //       BOTONES
    void on_Empezar_clicked();
    void on_continuar_clicked();
    void on_salir_menu_clicked();
    void on_multijugador_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *Scene = new QGraphicsScene;
    QGraphicsView  *view;

    //              TIEMPOS DE MOVIMIENTOS
    QTimer *timer           = new QTimer;
    QTimer *tiempo_enemigos = new QTimer;

    personaje   *cuerpo;
    personaje2  *cuerpo2;
    ataques     *bala;
    jefes       *boss;

    //              LISTAS PARA LOS MOVIMIENTOS Y CONFIMARCIONES DE COLISION
    QList<ataques*>             balas;
    QList<ataques_enemigos*>    balas_ene;
    QList<enemigo*>             enemy;

    void keyPressEvent(QKeyEvent *evento);
};
#endif // MAINWINDOW_H
