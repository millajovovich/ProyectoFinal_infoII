#include "jefes.h"
#include "ataques.h"

bool jefes::getDestruccion() const
{
    return destruccion;
}

jefes::jefes()
{

}

jefes::jefes(int pos_x, int pos_y)
{
    posx = pos_x;
    posy = pos_y;
    setPos(posx, posy);
}

void jefes::movimiento()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();  // para la colision con disparo o con jugador
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if ( typeid(*(colliding_items[i])) == typeid(ataques)  ){
            salud -= 10;
        }
    }
    if ( salud <= 0 ){
        destruccion = true;
    }
}








QRectF jefes::boundingRect() const
{
    return QRectF(0,0,100,400);
}

void jefes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawRect(boundingRect());
}
