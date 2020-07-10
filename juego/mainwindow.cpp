#include "stdio.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer= new QTimer;
    tiempo_enemigos = new QTimer;

    ui->graphicsView->setScene(Scene);
    Scene->setSceneRect(0,0,800,450);
    Scene->addRect(Scene->sceneRect());

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    srand(time(NULL));
    connect(timer,SIGNAL(timeout()),this,SLOT(Mover()));
    connect(tiempo_enemigos,SIGNAL(timeout()),this,SLOT(add_enemigo()));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}


void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    timer->start(15);
    tiempo_enemigos->start(1000);
}


//  FUNCION PARA ACTUALIZAR POSICIONES
void MainWindow::Mover()
{
    cuerpo->iteracion();
    cuerpo->calculo();

    //movimiento de las balas
    for( int i=0; i<balas.size();i++ ){
        if ( balas.at(i)->getColision() == true ){
            Scene->removeItem(balas.at(i));                 // para remover de la escena
            balas.removeOne(balas.at(i));                  //para eliminar de la lista
        }
        else
            balas.at(i)->movimiento();
    }

    for( int i=0; i<enemy.size();i++ ){
        if ( enemy.at(i)->getColision() == true ){
            Scene->removeItem(enemy.at(i));                 // para remover de la escena
            enemy.removeOne(enemy.at(i));                  //para eliminar de la lista
        }
        else
            enemy.at(i)->movimiento();
    }


    if ( cuerpo->baja_altura() == true )
        cuerpo->salto();
}

void MainWindow::add_enemigo()
{
    enemy.append( new enemigo() );
    Scene->addItem(enemy.back());
}


void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    if( evento->key()==Qt::Key_W ){
        cuerpo->salto();
    }
    if( evento->key()==Qt::Key_D ){
        cuerpo->setVx(5);
    }
    if( evento->key()==Qt::Key_A ){
        cuerpo->setVx(-5);
    }
    if( evento->key()==Qt::Key_S ){
        balas.append(new ataques(cuerpo->getPosx(), -cuerpo->getPosy()));
        Scene->addItem(balas.back());
    }
    if( evento->key()==Qt::Key_P ){
        activacion = !activacion;
        timer->stop();
        tiempo_enemigos->stop();
        if (activacion == true){
            ui->pushButton_2->show();
            ui->pushButton->show();
        }
        else{
            ui->pushButton_2->hide();
            ui->pushButton->hide();
        }
    }
}



void MainWindow::on_pushButton_2_clicked()
{
    //ui->pushButton_2->hide();
    timer->stop();
    tiempo_enemigos->stop();
}
