#ifndef CAMPSITE_H
#define CAMPSITE_H
#include "player.h"
#include <QObject>
#include <QGraphicsItem>
#include <QLabel>

class Campsite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Campsite(int, int, Player*);
    explicit Campsite(QObject *parent = nullptr);
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void setColor(QColor color){
        color_=color;
        update();
    }
    void setOwner(Player* player){
        owner_ = player;
    }
    int getX() { return x_; }
    int getY() { return y_; }
    virtual Player* getOwner() { return owner_; }
    virtual int getReward() { return 1; }


signals:
    void Click(Campsite*);


public slots:

protected:
     Player* owner_;
     QColor color_;
     int x_;
     int y_;
};


class SpaceStation : public Campsite
{
public:
    SpaceStation(int x, int y, Player* player, QColor color):
        Campsite(x,y,player)
        { color_ = color;}

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    int getReward() { return 2; }

//private:
//     Player* owner_;
//     QColor color_;
//     int x_;
//     int y_;
};

#endif // CAMPSITE_H
