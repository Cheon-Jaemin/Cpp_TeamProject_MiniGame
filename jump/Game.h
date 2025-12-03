#pragma once
#include "Maze.h"
#include "bPlayer.h"
#include <ctime>

class Game
{
private:
    Maze maze;
    bPlayer player;
    time_t startTime;
    int timeLimit;

public:
    Game(int rows, int cols, int timeLimit);
    void start();
    bool isTimeOver() const;
};