#include "Player.h"

Player::Player(int startX, int startY) 
{ 
    x = startX; 
    y = startY; 
}
int Player::getX() const 
{ 
    return x; 
}
int Player::getY() const 
{ 
    return y; 
}
void Player::move(char direction)
{
    switch (direction) 
    {
    case 'w': x--; break;
    case 's': x++; break;
    case 'a': y--; break;
    case 'd': y++; break;
    }

}