#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <map>
#include <QColor>
#include <vector>

enum class ResourceType { BlackHole, Metal, Stone, Oxygen};

class Player
{
public:
    Player(int, QColor, bool);
    void increaseResource(ResourceType, int);
    void increasePoints();
    void increaseUsed(int);
    QColor getColor() { return color_; }
    int getId() { return id_; }
    int getMetal() { return metal_; }
    int getStone() { return stone_; }
    int getOxygen() { return oxygen_; }
    int getPoints() { return points_; }
    int getUsed() { return totalUsed_; }
    bool isHuman() { return human_; }
    std::vector<int> getPointsOverTime() { return pointsOverTime_; }
    void pushToPoints(int x) { pointsOverTime_.push_back(x);}

private:
    QColor color_;
    int id_;
    int points_;
    int metal_;
    int stone_;
    int oxygen_;
    int totalUsed_;
    std::vector<int> pointsOverTime_;
    bool human_;
};

#endif // PLAYER_H
