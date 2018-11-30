#include "campsite.h"
#include <QtWidgets>


Campsite::Campsite(QObject *parent) : QObject(parent)
{

}


Campsite::Campsite(int x, int y, Player* owner)
{
    owner_ = owner;
    this->color_ = QColor(255,255,255);
    x_ = x * 150;
    y_ = y * 150;
}

QRectF Campsite::boundingRect() const
{
    return QRectF(x_, y_, x_ + 20, y_+ 20);
}

QPainterPath Campsite::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, x_ + 20, y_ + 20);
    return path;
}

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

void Campsite::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        emit Click(this);
    }

}
