#include "campsite.h"
#include <QtWidgets>

/**
    Camsite Contructor
    @param QObject
    @return
*/
Campsite::Campsite(QObject *parent) : QObject(parent)
{

}

/**
    Campsite Constructor
    @param two int for coordinate, and a player pointer
    @return
*/
Campsite::Campsite(int x, int y, Player* owner)
{
    owner_ = owner;
    this->color_ = QColor(255,255,255);
    x_ = x * 150;
    y_ = y * 150;
}

/**
    Build bounding rectangle where the object will be drawn
    @param
    @return
*/
QRectF Campsite::boundingRect() const
{
    return QRectF(x_, y_, x_ + 20, y_+ 20);
}

/**
    Draw the shape of object
    @param
    @return
*/
QPainterPath Campsite::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, x_ + 20, y_ + 20);
    return path;
}

/**
    Paint objects
    @param QPainter, QStyleOptionGraphicsItem, QWidget
    @return
*/
void Campsite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(color_);
    QPoint p1 = QPoint(x_, y_);
    QPoint p2 = QPoint(x_+20, y_+20);

    painter->drawRect(QRect(p1, p2));

    painter->setBrush(b);
}

/**
    Emit signals when an object is clicked in different situations
    @param QGraphicsSceneMouseEvent
    @return
*/
void Campsite::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        emit Click(this);
    }
}

/**
    Build bounding rectangle where the object will be drawn
    @param
    @return
*/
QRectF SpaceStation::boundingRect() const
{
    return QRectF(x_, y_, x_ + 20, y_+ 20);
}

/**
    Draw the shape of object
    @param
    @return
*/
QPainterPath SpaceStation::shape() const
{
    QPainterPath path;
    path.addEllipse(x_, y_, x_ + 20, y_ + 20);
    return path;
}

/**
    Paint objects
    @param QPainter, QStyleOptionGraphicsItem, QWidget
    @return
*/
void SpaceStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(color_);
    QPoint p1 = QPoint(x_, y_);
    QPoint p2 = QPoint(x_+20, y_+20);

    painter->drawEllipse(QRect(p1, p2));

    painter->setBrush(b);
}
