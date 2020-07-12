#include "stdio.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QGraphicsItem>


//arreglar lo de los disparos entre enemigos

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer= new QTimer;
    tiempo_enemigos = new QTimer;

    ui->graphicsView->setScene(Scene);
    Scene->setSceneRect(0,0,810,520);
    Scene->addRect(Scene->sceneRect());

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    srand(time(NULL));
    connect(timer,SIGNAL(timeout()),this,SLOT(Mover()));
    connect(tiempo_enemigos,SIGNAL(timeout()),this,SLOT(add_enemigo()));

    ui->gameover->hide();


    //connect(cuerpo->getSalud(), SIGNAL(valueChange(int)), ui->progressBar, SLOT(setVAlue(int)) );
}


MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}


void MainWindow::on_pushButton_clicked()
{
    timer->start(15);
    tiempo_enemigos->start(2000);

    ui->pushButton_2->hide();
    ui->pushButton->hide();
}


//  FUNCION PARA ACTUALIZAR POSICIONES
void MainWindow::Mover()
{
    //  MOVIMIENTO DE PERSONAJE 1
    cuerpo->iteracion();
    cuerpo->calculo();
    ui->lcdNumber->display(cuerpo->getPuntuacion());

    //  PARA CONFIRMAR BAJA ALTURA Y LADOS
    if ( cuerpo->baja_altura() == true )
        cuerpo->salto();

    //  PARA ANUNCIAR GAMEOVER
    if ( cuerpo->getPerdida() == 1 )
        juego_terminado();

    ui->progressBar->setValue(cuerpo->getSalud());

    //      MOVIMIENTO DE LAS BALAS
    for( int i=0; i<balas.size();i++ ){
        if ( balas.at(i)->getPuntos() == 1 )                // para confirmar si murio por impacto
            marcador += 20;

        if ( balas.at(i)->getColision() == true ){          // para confirma si salio de escena o impacto con algo
            Scene->removeItem(balas.at(i));                 // para remover de la escena
            balas.removeOne(balas.at(i));                  //para eliminar de la lista
        }
        else
            balas.at(i)->movimiento();
    }

    //      MOVIMIENTO DE LOS ENEMIGOS
    for( int i=0; i<enemy.size();i++ ){
        if ( int(-enemy.at(i)->getPosy_enemigo()) == int (cuerpo->getPosy()) ){
            //balas.append(new ataques( enemy.at(i)->getPosx_enemigo(), -enemy.at(i)->getPosy_enemigo(), 1));

            balas.append(new ataques( enemy.at(i)->getPosx_enemigo()-40, enemy.at(i)->getPosy_enemigo(), 1));
            Scene->addItem( balas.back() );

            std::cout<<cuerpo->getPosx()<<" "<<-cuerpo->getPosy()<<std::endl;
            std::cout<<enemy.at(i)->getPosx_enemigo()<<" "<<enemy.at(i)->getPosy_enemigo()<<std::endl;
        }

        if ( enemy.at(i)->getColision() == true ){
            Scene->removeItem(enemy.at(i));                 // para remover de la escena
            enemy.removeOne(enemy.at(i));                  //para eliminar de la lista
        }

        else
            enemy.at(i)->movimiento();
    }

    ui->lcdNumber->display(marcador);
}


void MainWindow::add_enemigo()
{
    enemy.append( new enemigo() );
    Scene->addItem(enemy.back());
}

void MainWindow::juego_terminado()
{
    ui->gameover->show();
    timer->stop();
    tiempo_enemigos->stop();
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
        balas.append(new ataques(cuerpo->getPosx()+20, -cuerpo->getPosy(), 0));
        Scene->addItem(balas.back());
    }
    if( evento->key()==Qt::Key_P ){
        timer->stop();
        tiempo_enemigos->stop();

        ui->pushButton_2->show();
        ui->pushButton->show();
    }
}



void MainWindow::on_pushButton_2_clicked()
{
    //ui->pushButton_2->hide();
    timer->stop();
    tiempo_enemigos->stop();
}
