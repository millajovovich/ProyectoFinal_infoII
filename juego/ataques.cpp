#include "ataques.h"
#include "mainwindow.h"

ataques::~ataques()
{   }

bool ataques::getColision() const
{
    return colision;
}

ataques::ataques()
{   }

double ataques::getPosx_bala() const
{
    return posx_bala;
}

ataques::ataques(double x, double y)
{
    posx_bala = x;
    posy_bala = y;
    setPos(posx_bala, posy_bala);
}

QRectF ataques::boundingRect() const
{
    return QRectF(-5,-5,20,5);
}

void ataques::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());
}

void ataques::movimiento()
{
    posx_bala+=20*0.2;                  // movimiento de la bala
    setPos(posx_bala, posy_bala);
    if ( posx_bala > 700 ){
        colision = true;
    }

    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(enemigo)){
            colision = true;
        }
    }
}
