#include "personaje.h"
#include "enemigo.h"
#include "mainwindow.h"

int personaje::getMasa() const
{
    return masa;
}

void personaje::setMasa(int value)
{
    masa = value;
}

void personaje::setVx(double value)
{
    vx = value;
}

int personaje::getSalud() const
{
    return salud;
}

int personaje::getPerdida() const
{
    return perdida;
}

void personaje::setPuntuacion(int value)
{
    puntuacion += value;
}

int personaje::getPuntuacion() const
{
    return puntuacion;
}

personaje::personaje()
{
    posx=400;
    posy=-200;
    vel=0;
    setPos(posx,-posy);
}

QRectF personaje::boundingRect() const
{
    return QRectF(-20,-35,35,70);
}

void personaje::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QPixmap pixmap;
    pixmap.load(":/imag/personaje0.png");
    painter->drawPixmap(boundingRect(),pixmap,pixmap.rect());
    //painter->setBrush(Qt::cyan);
    //painter->drawEllipse(boundingRect());
}

int personaje::getPosx() const
{
    return posx;
}

void personaje::setPosx(int value)
{
    posx = value;
}

int personaje::getPosy() const
{
    return posy;
}

void personaje::setPosy(int value)
{
    posy += value;
    setPos(posx,-posy);
}

void personaje::calculo(/*double posx2, double posy2, double masa2*/)
{
    /*angulo= atan2((posy2-posy), (posx2-posx));
    dist = pow(posx2-posx,2) + pow(posy2-posy,2);
    Ax += (((G*masa2)/dist) * cos(angulo));
    Ay += (((G*masa2)/dist) * sin(angulo));*/
    vy=vy-g*delta;
    vel=sqrt(vy*vy+vx*vx);
    angulo=atan2(vy,vx);

}

void personaje::salto()
{
    vy=40;
    angulo=90;
}

bool personaje::baja_altura()
{
    if ( posy < -500 )
        return true;
    else if (posx < 10  ||  posx > 810 )
        vx = -vx;
    else
        return false;
}

void personaje::iteracion()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(enemigo) || typeid(*(colliding_items[i])) == typeid(ataques)){
            salud -= 10;
        }
    }

    posx+=vx*delta;
    posy+=vy*delta-0.5*g*delta*delta;
    setPos(posx,-posy);

    if ( salud <= 0 )
        perdida = 1;
}




