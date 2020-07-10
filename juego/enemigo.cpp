#include "enemigo.h"
#include "ataques.h"
#include "mainwindow.h"
//#include "personaje.h"

bool enemigo::getColision() const
{
    return colision;
}

enemigo::enemigo()
{
    //srand (time(NULL));
    posx_enemigo = 750;
    posy_enemigo = rand()%410+21;
    setPos( posx_enemigo , posy_enemigo );
    tipo_enemigo = rand()%2;
}

enemigo::~enemigo()
{

}

double enemigo::getPosx_enemigo() const
{
    return posx_enemigo;
}

double enemigo::getPosy_enemigo() const
{
    return posy_enemigo;
}

void enemigo::setVx(double value)
{
    vx = value;
}

void enemigo::movimiento()
{
    if ( tipo_enemigo == 0 )
        posx_enemigo += vx*0.1;   //       posx = vel*tiempo

    else if ( tipo_enemigo == 1 ){
        posx_enemigo += -3*0.1;
        posy_enemigo += vy*0.1;

        if ( posy_enemigo <= 20 || posy_enemigo >= 430 )        // para hacer el efecto de rebote
            vy=-vy;

    }
    setPos( posx_enemigo , posy_enemigo );

    if ( posx_enemigo < 50 ){
        colision = true;
    }

    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(ataques)){
            colision = true;
        }
    }
}

QRectF enemigo::boundingRect() const
{
    return QRectF(-15,-15,30,30);
}

void enemigo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::blue);
    painter->drawRect(boundingRect());
}
