#include "stdio.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;

#include <QGraphicsItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

    ui->graphicsView->setScene(Scene);
    Scene->setSceneRect(0,0,818,528);
    Scene->addRect(Scene->sceneRect());

    // SUELO PARA AMBIENTAR
    entorno = new ambiente(1);

    //              RELOJES
    connect(timer,          SIGNAL(timeout()),  this,SLOT(Mover()));
    connect(tiempo_enemigos,SIGNAL(timeout()),  this,SLOT(add_enemigo()));
    connect(tiempo_ata_jefe,SIGNAL(timeout()),  this,SLOT(ataque_boss()));
    connect(tiempo_nubes,   SIGNAL(timeout()),  this,SLOT(add_nube()));
    connect(tiempo_amb,     SIGNAL(timeout()),  this,SLOT(add_ambiente()));
    connect(tiempo_agujero, SIGNAL(timeout()),  this,SLOT(add_agujero()));

    // ocultar los botones para que no aparezcan todos cuando empiece la app
    ui->gameover->      hide();
    ui->score->         hide();
    ui->completado->    hide();
    ui->lcdNumber->     hide();
    ui->nombre_1->      hide();
    ui->nombre_2->      hide();
    ui->progressBar_1-> hide();
    ui->progressBar_2-> hide();
    ui->continuar->     hide();
    ui->salir_menu->    hide();
    ui->solo_jugador->  hide();
    ui->multijugador->  hide();
    ui->espacio->       hide();
    ui->cargar->        hide();
    ui->guardar->       hide();
    ui->reiniciar->     hide();
    ui->ganador->       hide();
    ui->tutorial->      hide();
    ui->volver->        hide();

    ui->label_6->       hide();
    ui->label_7->       hide();

    //   SONIDOS
    intro->setMedia(QUrl("qrc:/sounds/sonidos/intro.mp3"));
    lose->setMedia(QUrl("qrc:/sounds/sonidos/lose.mp3"));
    ata_bos->setMedia(QUrl("qrc:/sounds/sonidos/ataque_b.mp3"));
    icon->setMedia(QUrl("qrc:/sounds/sonidos/icon.mp3"));
    win->setMedia(QUrl("qrc:/sounds/sonidos/win.mp3"));
    destruc->setMedia(QUrl("qrc:/sounds/sonidos/destruccion.mp3"));

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo_intro.png")));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

//  FUNCION PARA ACTUALIZAR POSICIONES
void MainWindow::Mover( )
{
    //      MOVIMIENTO DE PERSONAJE 1
    cuerpo->iteracion();
    if ( verificador_hole == 1){
            for (int i = 0; i < negro.size(); ++i) {
            cuerpo->calculo(negro.at(i)->getPosx(), -negro.at(i)->getPosy(), negro.at(i)->getMasa() );
        }
    }

    //      MOVIMIENTO DE PERSONAJE 2
    if ( multijugador == true ) {
        cuerpo2->iteracion();
        if ( verificador_hole == 1){
                for (int i = 0; i < negro.size(); ++i) {
                cuerpo2->calculo(negro.at(i)->getPosx(), -negro.at(i)->getPosy(), negro.at(i)->getMasa() );
            }
        }
    }

    //      MOVIMIENTO Y SALUD DE jefe
    if ( verificador == 1 ){
        boss->movimiento();
        boss->show();
        if ( boss->getDestruccion() == 1 ){
            Scene->removeItem(boss);
            delete boss;
            verificador = 0;
            marcador +=1000;
            nivel += 1;
            tiempo_ata_jefe->stop();

            destruc->play();
            tiempo_enemigos->start(5000 - 1000*nivel);      //  para subir la dificultad tra

            //actualizar valores de check point
            vida = cuerpo->getSalud();
            marc = marcador;
            lvl = nivel;

            //para juego completado
            if ( nivel == 4 ){
                juego_completado();
            }
        }
    }

    //      MOVIMIENTO DE LAS BALAS DE PERSONAJES
    for( int i=0; i<balas.size();i++ ){
        if ( balas.at(i)->getPuntos() == 1 )                // para confirmar si murio por impacto
            marcador += 20;

        if ( balas.at(i)->getColision() == true ){          // para confirma si salio de escena o impacto con algo
            Scene->removeItem(balas.at(i));                 // para remover de la escena
            delete balas.at(i);
            balas.removeOne(balas.at(i));                   // para eliminar de la lista
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
            delete balas_ene.at(i);
            balas_ene.removeOne(balas_ene.at(i));                  //para eliminar de la lista
        }
        else
            balas_ene.at(i)->movimiento();
    }

    //      MOVIMIENTO DE LOS ENEMIGOS
    for( int i=0; i<enemy.size();i++ ){
        if ( int(-enemy.at(i)->getPosy_enemigo()) == int (cuerpo->getPosy()) ){

            //          CREACION DE LAS BALAS ENEMIGAS EN POSICION DEL PERSONAJE
            balas_ene.append(new ataques_enemigos( enemy.at(i)->getPosx_enemigo(), enemy.at(i)->getPosy_enemigo(),0,0, 0));
            Scene->addItem( balas_ene.back() );
        }

        //      VERIFICADOR DE DESTRUCCION DE ENEMIGO
        if ( enemy.at(i)->getColision() == true ){
            Scene->removeItem(enemy.at(i));                 // para remover de la escena
            delete enemy.at(i);
            enemy.removeOne(enemy.at(i));                   //para eliminar de la lista

            // para llevar la cuenta de en cuanto aparece el boss o aumenta dificultad.
            cuenta_enemigos += 1;

            //      CUENTA PARA APARICION DE LOS JEFES SOLO EN JUGADOR SOLO
            if ( multijugador == 0 && cuenta_enemigos%(20*nivel) == 0 && verificador == 0){
                add_boss();
            }
            //      CUENTA PARA AUMENTO DE DIFICULTAD ENEMIGOS
            else if ( multijugador == 1 && cuenta_enemigos%(20*nivel) == 0 && verificador == 0 && nivel <= 3){
                tiempo_enemigos->start(4000-(1000*nivel));
                nivel += 1;
            }
        }

        else
            enemy.at(i)->movimiento();
    }

    //      MOVIMIENTO DE NUBES
    for ( int i=0; i<nube.size();i++ ){
        nube.at(i)->movimiento();

        if ( nube.at(i)->getColision() == true ){
            if ( nube.at(i)->getColision_perso() == 1 ){            // para confirmas si personaje tomo PowerUp
                cuerpo->setTipo_powerup( nube.at(i)->getTipo() );
            }
            Scene->removeItem( nube.at(i) );
            delete nube.at(i);
            nube.removeOne( nube.at(i) );
        }
    }


    //      MOVIMIENTO DE AGUJERO NEGRO
    for ( int i=0; i<negro.size();i++ ){
        negro.at(i)->iteracion();

        if ( negro.at(i)->getDestrucion() == true ){
            Scene->removeItem( negro.at(i) );
            delete negro.at(i);
            negro.removeOne( negro.at(i) );
            verificador_hole = 0;
        }
    }

    //  MOVIMIENTO DEL ENTORNO ( NUBES FONDO )
    for ( int i=0; i<entor.size(); i++ ){
        entor.at(i)->movimiento();

        if ( entor.at(i)->getDestrucion() == true ){
            Scene->removeItem( entor.at(i) );
            delete entor.at(i);
            entor.removeOne( entor.at(i) );
        }
    }

    //  PARA ACTUALIZAR VIDAS Y MARCADOR
    ui->progressBar_1->setValue(cuerpo->getSalud());
    ui->progressBar_2->setValue(cuerpo2->getSalud());
    ui->lcdNumber->display(marcador);

    //  PARA ANUNCIAR GAMEOVER
    if ( cuerpo->getPerdida() == 1 )
        juego_terminado();

    //  PARA PERDIDA DE P2 EN MULTIJUGADOR
    if ( multijugador == 1 && cuerpo2->getPerdida() == 1 ){
        juego_terminado();
    }

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
    boss = new jefes(820 , 70, nivel );
    Scene->addItem(boss);
    tiempo_ata_jefe->start(5000 - 1000*nivel);
}

void MainWindow::add_nube()
{
    nube.append( new nubes );
    Scene->addItem( nube.back() );
}

void MainWindow::add_ambiente()
{
    Scene->removeItem( entorno );
    Scene->removeItem( cuerpo );

    entor.append ( new ambiente(0) );
    Scene->addItem( entor.back() );

    Scene->addItem( entorno );
    Scene->addItem( cuerpo );
}

void MainWindow::add_agujero()
{
    negro.append( new agujero_negro() );
    Scene->addItem( negro.back() );
    verificador_hole = 1;
}

void MainWindow::ataque_boss()
{
    Scene->removeItem(boss);
    if (nivel == 1){
        balas_ene.append(new ataques_enemigos(0, 0, 0, 0, 1));
        Scene->addItem(balas_ene.back());
    }
    else if( nivel == 2){
        balas_ene.append(new ataques_enemigos(720, 160, -15, 0, 2));
        Scene->addItem(balas_ene.back());

        balas_ene.append(new ataques_enemigos(720, 160, -13, 5, 2));
        Scene->addItem(balas_ene.back());
        balas_ene.back()->setRotation(-20);

        balas_ene.append(new ataques_enemigos(720, 160, -10, 10, 2));
        Scene->addItem(balas_ene.back());
        balas_ene.back()->setRotation(-40);
    }
    else if ( nivel == 3){
        if ( ataque_jefe == 0 ){
            balas_ene.append(new ataques_enemigos(0, 0, 0, 0, 1));
            Scene->addItem(balas_ene.back());
        }
        else {
            balas_ene.append(new ataques_enemigos(720, 160, -15, 0, 2));
            Scene->addItem(balas_ene.back());

            balas_ene.append(new ataques_enemigos(720, 160, -13, 5, 2));
            Scene->addItem(balas_ene.back());
            balas_ene.back()->setRotation(-20);

            balas_ene.append(new ataques_enemigos(720, 160, -10, 10, 2));
            Scene->addItem(balas_ene.back());
            balas_ene.back()->setRotation(-40);
        }
        ataque_jefe = !ataque_jefe;
    }
    ata_bos->stop();
    ata_bos->play();

    Scene->addItem(boss);
}

void MainWindow::juego_terminado()
{
    lose->play();
    if ( multijugador == 0 ){
        ui->score->setText("PUNTAJE: " + QString::number(marcador));
        ui->gameover->  show();
        ui->score->     show();
    }

    else{
        if ( cuerpo->getSalud() <= 0 ){
            ui->ganador->setText("EL GANADOR ES: "+ ui->nombre_2->text());
        }
        else{
            ui->ganador->setText("EL GANADOR ES: "+ ui->nombre_1->text());
        }
        ui->ganador->   show();
    }

    ui->salir_menu->show();
    ui->reiniciar-> show();

    juego_on = false;

    timer->             stop();
    tiempo_enemigos->   stop();
    tiempo_nubes->      stop();
    tiempo_ata_jefe->   stop();
    tiempo_agujero->    stop();
    tiempo_amb->        stop();
}

void MainWindow::juego_completado()
{
    destruc->stop();
    win->play();

    ui->completado->show();
    ui->score->setText("PUNTAJE: " + QString::number(marcador+20));
    ui->score->  show();
    ui->salir_menu->show();

    juego_on = false;

    timer->             stop();
    tiempo_enemigos->   stop();
    tiempo_nubes->      stop();
    tiempo_ata_jefe->   stop();
    tiempo_agujero->    stop();

}


//                                              TECLAS DE MOVIMIENTO Y PAUSE
void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    //                                  MOVIMIENTO PERSONAJE 1
    if ( juego_on == true ){
        if( evento->key()==Qt::Key_W ){
            cuerpo->salto();
        }
        if( evento->key()==Qt::Key_D ){
            cuerpo->setVx(8);
        }
        if( evento->key()==Qt::Key_A ){
            cuerpo->setVx(-8);
        }
        if( evento->key()==Qt::Key_E || evento->key()==Qt::Key_V){
            balas.append(new ataques(cuerpo->getPosx()+20, -cuerpo->getPosy(), 0));
            Scene->addItem(balas.back());
        }
    }

    //                                  MOVIMIENTO PERSONAJE 2
    if ( multijugador == true && juego_on == true ){
        if( evento->key()==Qt::Key_I ){
            cuerpo2->salto();
        }
        if( evento->key()==Qt::Key_L ){
            cuerpo2->setVx(5);
        }
        if( evento->key()==Qt::Key_J ){
            cuerpo2->setVx(-5);
        }
        if( evento->key()==Qt::Key_0 || evento->key()==Qt::Key_O){
            balas.append(new ataques(cuerpo2->getPosx()+20, -cuerpo2->getPosy(), 0));
            Scene->addItem(balas.back());
        }
    }

    //                                  TECLAS DE EVENTOS
    if( evento->key()==Qt::Key_P && juego_on == 1){
        timer->          stop();
        tiempo_enemigos->stop();
        tiempo_nubes->   stop();
        tiempo_ata_jefe->stop();
        tiempo_agujero-> stop();
        tiempo_amb->     stop();

        ui->salir_menu->show();
        ui->continuar-> show();
        ui->reiniciar-> show();
        if ( multijugador == false )            // multijugador no contiene guardado
            ui->guardar->   show();
    }

    if ( evento->key()==Qt::Key_Space && juego_on == 0  && verificador_esp == 1 ){           //para evitar errores de tiempo en medio del juego

        juego_on = true;
        verificador_esp = false;

        timer->start(15);
        tiempo_enemigos->start(4000-(1000*nivel));
        tiempo_nubes->start(30000);
        tiempo_agujero->start(35000);
        tiempo_amb->start(20000);

        ui->espacio->hide();
    }
}


//                                              BOTONES USADOS
void MainWindow::on_solo_jugador_clicked()
{
    icon->play();

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    verificador = 0;
    verificador_esp = 1;

    ui->lcdNumber->display(marcador);

    ui->lcdNumber->     show();
    ui->nombre_1->      show();
    ui->progressBar_1-> show();
    ui->espacio->       show();

    Scene->addItem( entorno );

    ui->solo_jugador->  hide();
    ui->multijugador->  hide();
    ui->cargar->        hide();
    ui->tutorial->      hide();

    intro->play();

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo1.png")));
}

void MainWindow::on_multijugador_clicked()
{
    icon->play();

    multijugador = 1;
    verificador_esp = 1;

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    cuerpo2 = new personaje2();
    Scene->addItem(cuerpo2);

    ui->nombre_1->      show();
    ui->nombre_2->      show();
    ui->progressBar_1-> show();
    ui->progressBar_2-> show();
    ui->espacio->       show();

    Scene->addItem( entorno );

    ui->solo_jugador->hide();
    ui->multijugador->hide();
    ui->cargar->      hide();
    ui->tutorial->    hide();

    intro->play();

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo1.png")));
}

void MainWindow::on_continuar_clicked()
{
    icon->play();

    timer->          start();
    tiempo_enemigos->start();
    tiempo_nubes->   start();
    tiempo_agujero-> start();
    tiempo_amb->     start();

    if ( verificador == 1 ){
        tiempo_ata_jefe->start();
    }


    ui->continuar-> hide();
    ui->salir_menu->hide();
    ui->guardar->   hide();
    ui->reiniciar-> hide();
    ui->espacio->   hide();

}

void MainWindow::on_salir_menu_clicked()
{
    icon->play();

    if ( multijugador == 1 ){
        delete cuerpo2;
    }

    Scene->removeItem( entorno );

    //              PARAR LOS RELOJES
    timer->             stop();
    tiempo_enemigos->   stop();
    tiempo_nubes->      stop();
    tiempo_ata_jefe->   stop();
    tiempo_agujero->    stop();
    tiempo_amb->        stop();

    //              PARA LIMPIAR LISTAS
    balas.clear();
    balas_ene.clear();
    enemy.clear();
    nube.clear();
    negro.clear();
    entor.clear();
    Scene->clear();

    //              MOSTRAR BOTONES NECESARIOS
    ui->solo_jugador->  show();
    ui->multijugador->  show();
    ui->cargar->        show();
    ui->tutorial->      show();

    ui->completado->    hide();
    ui->score->         hide();
    ui->lcdNumber->     hide();
    ui->nombre_1->      hide();
    ui->nombre_2->      hide();
    ui->progressBar_1-> hide();
    ui->progressBar_2-> hide();
    ui->continuar->     hide();
    ui->salir_menu->    hide();
    ui->gameover->      hide();
    ui->guardar->       hide();
    ui->reiniciar->     hide();
    ui->ganador->       hide();

    juego_on = false;

    multijugador     = 0;
    verificador      = 0;
    verificador_hole = 0;
    verificador_esp  = 0;
    cuenta_enemigos  = 1;
    nivel            = 1;
    marcador         = 0;

    ui->lcdNumber->display(marcador);
    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo_intro.png")));
}

void MainWindow::on_registrar_clicked()
{
    icon->play();

    if ( datos->registro( ui->nombre_reg->text().toStdString(), ui->contra_reg->text().toStdString() ) ){  //verificar ingreso correcto
        ui->ingreso->hide();
        ui->registro->hide();

        ui->solo_jugador->show();
        ui->multijugador->show();
        ui->cargar->      show();
        ui->tutorial->    show();

        ui->nombre_1->setText( ui->nombre_reg->text() );          //  colocar nombre en juego
    }
    else{
        ui->label_6->show();
    }

}

void MainWindow::on_ingresar_clicked()
{
    icon->play();

    if ( datos->ingreso( ui->nombre_ing->text().toStdString(), ui->contra_ing->text().toStdString() ) == true ){
        ui->ingreso->hide();
        ui->registro->hide();

        ui->solo_jugador->show();
        ui->multijugador->show();
        ui->cargar->      show();
        ui->tutorial->    show();

        ui->nombre_1->setText( ui->nombre_ing->text() );          //  colocar nombre en juego
    }
    else{
        ui->label_7->show();
    }
}

void MainWindow::on_guardar_clicked()
{
    icon->play();

    datos->guardado_datos(to_string(vida), to_string(marc), to_string(lvl));

    juego_on = false;

    on_salir_menu_clicked();
}

void MainWindow::on_cargar_clicked()
{
    icon->play();

    cuerpo = new personaje();
    Scene->addItem(cuerpo);

    //      carga de datos de la ultima vez
    datos->obt_datos( &vida, &marc, &lvl );
    cuerpo->setSalud(vida);
    marcador = marc;
    nivel = lvl;

    verificador_esp = 1;

    ui->lcdNumber->     show();
    ui->nombre_1->      show();
    ui->progressBar_1-> show();
    ui->espacio->       show();

    ui->lcdNumber->display(marcador);
    Scene->addItem( entorno );

    ui->solo_jugador->  hide();
    ui->multijugador->  hide();
    ui->cargar->        hide();
    ui->tutorial->      hide();

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo1.png")));

}

void MainWindow::on_reiniciar_clicked()
{
    icon->play();

    if ( multijugador == 0 ){
        on_salir_menu_clicked();
        on_solo_jugador_clicked();
    }
    else{
        on_salir_menu_clicked();
        on_multijugador_clicked();
    }

    marcador = 0;
    verificador_esp = 1;
    ui->lcdNumber->display(marcador);
    ui->progressBar_1->setValue(100);
    ui->progressBar_2->setValue(100);
}

void MainWindow::on_volver_clicked()
{
    icon->play();

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/fondo_intro.png")));

    ui->solo_jugador->  show();
    ui->multijugador->  show();
    ui->cargar->        show();
    ui->tutorial->      show();

    ui->volver->hide();

}

void MainWindow::on_tutorial_clicked()
{
    icon->play();

    ui->solo_jugador->  hide();
    ui->multijugador->  hide();
    ui->cargar->        hide();
    ui->tutorial->      hide();
    ui->volver->        show();

    ui->graphicsView->setBackgroundBrush(QBrush(QImage(":/imag/tutorial.png")));
}
