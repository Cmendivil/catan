#include "player.h"

/**
    Player Contructor
    @param int for id, and a color
    @return
*/
Player::Player(int id, QColor color, bool human)
{
    id_ = id;
    color_ = color;
    metal_ = 0;
    stone_ = 0;
    oxygen_ = 0;
    totalUsed_ = 0;
    points_ = 0;
    human_ = human;
    pointsOverTime_.push_back(0);
}

/**
    Increase the amount of a specific material
    @param ResourceType of material being updated, and int of how much to change
    @return
*/
void Player::increaseResource(ResourceType type, int reward)
{
    switch (type) {
    case ResourceType::Metal:
        metal_ += reward;
        break;
    case ResourceType::Stone:
        stone_ += reward;
        break;
    case ResourceType::Oxygen:
        oxygen_ += reward;
        break;
    }
}


/**
    Increase points by one
    @param
    @return
*/
void Player::increasePoints(){
    points_++;
}

/**
    Add a number to the totalUSed_ variable
    @param an int
    @return
*/
void Player::increaseUsed(int number){
    totalUsed_ += number;
}
