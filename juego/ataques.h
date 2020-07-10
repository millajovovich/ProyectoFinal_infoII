#ifndef ATAQUES_H
#define ATAQUES_H
#include <QGraphicsItem>
#include <QPainter>


class ataques: public QGraphicsItem
{
    bool colision = false;
    double posx_bala;
    double posy_bala;

public:
    ataques();
    ~ataques();
    ataques(double x, double y);                  //para crear la bala desde la posicion de personaje

    QRectF boundingRect() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void movimiento();
    double getPosx_bala() const;

    bool getColision() const;
};

#endif // ATAQUES_H
