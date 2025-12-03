#pragma once

class Player {
private:
    int x, y;
public:

    Player(int startX, int startY);
    int getX() const;
    int getY() const;
    void move(char direction);
};