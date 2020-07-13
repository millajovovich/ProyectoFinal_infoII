#ifndef JEFES_H
#define JEFES_H

#include <QGraphicsItem>
#include <QPainter>

class jefes: public QGraphicsItem
{
    int posx;
    int posy;
    int ancho;
    int alto;
    int salud           = 100;
    bool destruccion    = 0;                   // para confirmar la salud del boss
public:
    jefes();
    jefes (int pos_x , int pos_y );
    void movimiento();

    bool getDestruccion() const;

private:
    QRectF boundingRect() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};

#endif // JEFES_H
