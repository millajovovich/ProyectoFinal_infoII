#include "enemigo.h"
#include "ataques.h"
#include "mainwindow.h"
#include "personaje.h"

bool enemigo::getColision() const
{
    return colision;
}

enemigo::enemigo()
{
    //srand (time(NULL));
    posx_enemigo = 800;
    posy_enemigo = rand()%410+80;
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
        posx_enemigo += -10*0.1;
        posy_enemigo += vy*0.1;

        if ( posy_enemigo <= 70 || posy_enemigo >= 510 )        // para hacer el efecto de rebote
            vy=-vy;

    }
    setPos( posx_enemigo , posy_enemigo );

    if ( posx_enemigo < 20 ){
        colision = true;
    }

    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo o con jugador
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if ( typeid(*(colliding_items[i])) == typeid(ataques) || typeid(*(colliding_items[i])) == typeid(personaje)
             || typeid(*(colliding_items[i])) == typeid(personaje2)){
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
