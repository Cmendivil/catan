#include "player.h"

Player::Player(int id, QColor color)
{
    id_ = id;
    color_ = color;
    metal_ = 0;
    stone_ = 0;
    oxygen_ = 0;
    totalUsed_ = 0;
}

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
