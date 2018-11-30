#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <map>
#include <QColor>

enum class ResourceType { BlackHole, Metal, Stone, Oxygen};

class Player
{
public:
    Player(int, QColor);
    void increaseResource(ResourceType, int);
    QColor getColor() { return color_; }
    int getId() { return id_; }
    int getMetal() { return metal_; }
    int getStone() { return stone_; }
    int getOxygen() { return oxygen_; }

private:
    QColor color_;
    int id_;
    int points_;
    int metal_;
    int stone_;
    int oxygen_;
    int totalUsed_;
};

#endif // PLAYER_H
