#pragma once
#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <algorithm>
using namespace std;

extern int coin;

struct Config {
    int width = 55;
    int height = 31;
    int playerY = 30;
    int baseDelay = 80;
    int speedGainPerLevel = 5;
    int levelUpEvery = 150;
    double baseSpawnProb = 0.18;
    double spawnIncPerLevel = 0.03;
};

struct Poop {
    int x, y;
};

class PoopGame {
private:
    Config cfg;
    vector<Poop> poops;
    int playerX;
    int score;
    int level;
    int bestScore;
    bool running;
    bool paused;
    HANDLE hOut;

    int prevPlayerX;
    vector<Poop> prevPoops;

public:
    PoopGame();
    void run();

private:
    void setupConsole();
    void restoreCursor();
    void moveHome();
    void clearScreen();
    void initBoard();
    void handleInput();
    void maybeSpawnPoop();
    void tick();
    void draw();
    int delayForLevel();
    void showTitle();
    void showHelp();
    int showMenuAndPick();
    void runGame();
    void start();
};
