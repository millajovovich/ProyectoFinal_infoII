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

    ui->graphicsView->setScene(Scene);
    Scene->setSceneRect(0,0,810,520);
    Scene->addRect(Scene->sceneRect());
    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo.jpg")));

    srand(time(NULL));
    connect(timer,SIGNAL(timeout()),this,SLOT(Mover()));
    connect(tiempo_enemigos,SIGNAL(timeout()),this,SLOT(add_enemigo()));

    // ocultar los botones para que no aparezcan todos cuando empiece la app
    ui->gameover->hide();
    ui->lcdNumber->hide();
    ui->lcdNumber_2->hide();
    ui->nombre_1->hide();
    ui->nombre_2->hide();
    ui->progressBar_1->hide();
    ui->progressBar_2->hide();
    ui->continuar->hide();
    ui->salir_menu->hide();

}


MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

//  FUNCION PARA ACTUALIZAR POSICIONES
void MainWindow::Mover()
{
    //  MOVIMIENTO DE PERSONAJE 1
    cuerpo->iteracion();
    cuerpo->calculo();
    ui->lcdNumber->display(cuerpo->getPuntuacion());

    //  MOVIMIENTO DE PERSONAJE 2
    if ( multijugador == true ) {
        cuerpo2->iteracion();
        cuerpo2->calculo();
        ui->lcdNumber_2->display(cuerpo2->getPuntuacion());
    }

    //  MOVIMIENTO Y SALUD DE jefe
    if ( verificador == 1 ){
        boss->movimiento();
        if ( boss->getDestruccion() == 1 ){
            Scene->removeItem(boss);
            verificador = 0;
            marcador +=1000;
        }
    }

    //  PARA CONFIRMAR BAJA ALTURA Y LADOS P1
    if ( cuerpo->baja_altura() == true )
        cuerpo->salto();

    //  PARA CONFIRMAR BAJA ALTURA Y LADOS P2
    if ( cuerpo2->baja_altura() == true )
        cuerpo2->salto();

    //  PARA ANUNCIAR GAMEOVER
    if ( cuerpo->getPerdida() == 1 || cuerpo2->getPerdida() == 1 )
        juego_terminado();

    //      MOVIMIENTO DE LAS BALAS DE PERSONAJES
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

    //      MOVIMIENTO DE LAS BALAS DE ENEMIGOS
    for( int i=0; i<balas_ene.size();i++ ){
        if ( balas_ene.at(i)->getPuntos() == 1 )                // para confirmar si murio por impacto
            marcador += 20;

        if ( balas_ene.at(i)->getColision() == true ){          // para confirma si salio de escena o impacto con algo
            Scene->removeItem(balas_ene.at(i));                 // para remover de la escena
            balas_ene.removeOne(balas_ene.at(i));                  //para eliminar de la lista
        }
        else
            balas_ene.at(i)->movimiento();
    }

    //      MOVIMIENTO DE LOS ENEMIGOS
    for( int i=0; i<enemy.size();i++ ){
        if ( int(-enemy.at(i)->getPosy_enemigo()) == int (cuerpo->getPosy()) ){

            //          CREACION DE LAS BALAS ENEMIGAS EN POSICION DEL ENEMIGO
            balas_ene.append(new ataques_enemigos( enemy.at(i)->getPosx_enemigo(), enemy.at(i)->getPosy_enemigo(), 0));
            Scene->addItem( balas_ene.back() );
        }

        //      VERIFICADOR DE DESTRUCCION DE ENEMIGO
        if ( enemy.at(i)->getColision() == true ){
            Scene->removeItem(enemy.at(i));                 // para remover de la escena
            enemy.removeOne(enemy.at(i));                   //para eliminar de la lista

            //      CUENTA PARA APARICION DE LOS JEFES
            cuenta_enemigos += 1;                           // para llevar la cuenta de en cuanto aparece el boss
            if (cuenta_enemigos%20 == 0 && verificador == 0){  // condicional para llamar al boss
                add_boss();
            }
        }

        else
            enemy.at(i)->movimiento();
    }

    ui->progressBar_1->setValue(cuerpo->getSalud());
    ui->progressBar_2->setValue(cuerpo2->getSalud());
}



void MainWindow::add_enemigo()
{
    if (verificador == 0 ){                         // verificador de boss
        enemy.append( new enemigo() );
        Scene->addItem(enemy.back());
    }
}

void MainWindow::add_boss()
{
    verificador = 1;
    boss = new jefes(700 , 70);
    Scene->addItem(boss);
}



void MainWindow::juego_terminado()
{
    ui->gameover->show();
    timer->stop();
    tiempo_enemigos->stop();
}


void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    //                                  MOVIMIENTO PERSONAJE 1
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

    //                                  MOVIMIENTO PERSONAJE 2
    if ( multijugador == true ){
        if( evento->key()==Qt::Key_I ){
            cuerpo2->salto();
        }
        if( evento->key()==Qt::Key_L ){
            cuerpo2->setVx(5);
        }
        if( evento->key()==Qt::Key_J ){
            cuerpo2->setVx(-5);
        }
        if( evento->key()==Qt::Key_K ){
            balas.append(new ataques(cuerpo2->getPosx()+20, -cuerpo2->getPosy(), 0));
            Scene->addItem(balas.back());
        }
    }

    //                                  TECLAS DE EVENTOS
    if( evento->key()==Qt::Key_P ){
        timer->stop();
        tiempo_enemigos->stop();
        //ui->graphicsView->setBackgroundBrush(Qt::white);

        ui->salir_menu->show();
        ui->continuar->show();
    }
}


void MainWindow::on_Empezar_clicked()
{
    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    timer->start(15);
    tiempo_enemigos->start(2000);

    ui->lcdNumber->show();
    ui->nombre_1->show();
    ui->progressBar_1->show();

    ui->Empezar->hide();
    ui->multijugador->hide();
}


void MainWindow::on_continuar_clicked()
{
    timer->start(15);
    tiempo_enemigos->start(2000);

    ui->continuar->hide();
    ui->Empezar->hide();

}

void MainWindow::on_salir_menu_clicked()
{
    timer->stop();
    tiempo_enemigos->stop();
    Scene->clear();
    ui->Empezar->show();

}

void MainWindow::on_multijugador_clicked()
{
    multijugador = 1;

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    cuerpo2 = new personaje2();
    Scene->addItem(cuerpo2);

    timer->start(15);
    tiempo_enemigos->start(2000);

    ui->lcdNumber->show();
    ui->nombre_1->show();
    ui->nombre_2->show();
    ui->progressBar_1->show();
    ui->progressBar_2->show();

    ui->Empezar->hide();
    ui->multijugador->hide();
}
