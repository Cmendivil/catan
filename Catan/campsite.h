#ifndef CAMPSITE_H
#define CAMPSITE_H
#include "player.h"
#include <QObject>
#include <QGraphicsItem>

class Campsite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Campsite(int, int, Player*);
    explicit Campsite(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void setColor(QColor color){
        color_=color;
        update();
    }
    void setOwner(Player* player){
        owner_ = player;
    }

    Player* getOwner() { return owner_; }
    int getReward() { return 1; }


signals:
    void Click(Campsite*);

public slots:

private:
     Player* owner_;
     QColor color_;
     int x_;
     int y_;
};

class SpaceStation : public Campsite
{
public:
    int getReward() { return 2; }
private:
};

#endif // CAMPSITE_H
