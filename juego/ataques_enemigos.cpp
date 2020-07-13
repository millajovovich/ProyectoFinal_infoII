#include "ataques_enemigos.h"
#include "mainwindow.h"

ataques_enemigos::ataques_enemigos()
{   }

bool ataques_enemigos::getColision() const
{
    return colision;
}

int ataques_enemigos::getPuntos() const
{
    return puntos;
}

double ataques_enemigos::getPosx_bala() const
{
    return posx_bala;
}

ataques_enemigos::ataques_enemigos(double x, double y, int tip)
{
    tipo = tip;
    posx_bala = x;
    posy_bala = y;
    setPos(posx_bala, posy_bala);
}

QRectF ataques_enemigos::boundingRect() const
{
    return QRectF(-5,-5,20,5);
}

void ataques_enemigos::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());
}

void ataques_enemigos::movimiento()
{
    if ( posx_bala < 10){
        colision = true;
    }

    if ( tipo == 0 ){
        posx_bala -= 20*0.2;                  // movimiento de la bala
        setPos(posx_bala, posy_bala);
    }

    /*else if ( tipo == 1 ){
        posx_bala -= 20*0.2;                  // movimiento de la bala
        setPos(posx_bala, posy_bala);
    }*/

    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if ( typeid(*(colliding_items[i])) == typeid(personaje) || typeid(*(colliding_items[i])) == typeid(personaje2) ){
            colision = true;
            puntos = 1;
        }
    }
}
