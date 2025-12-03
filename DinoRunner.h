#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

class DinoRunner {
public:
    struct Config {
        int width = 70;
        int height = 26;
        int groundY = 22;
        int baseDelay = 18;
        double baseSpeed = 1.1;
        double gravity = 0.9;
        double jumpV = -4.5;
        int playerX = 10;
        double cactusSpawnProb = 0.10;
        double birdSpawnProb = 0.06;
        int birdWidth = 3;
        int minGapTiles = 22;
        int maxGapTiles = 34;
        int cactusHeight = 2;
        int scorePerCoin = 400;
    };

    enum class ObType { CACTUS_LOW, BIRD_HIGH };

    struct Obstacle {
        ObType type;
        double x;
        int h;
    };

    struct State {
        double y = 0.0;
        double vy = 0.0;
        bool onGround = true;
        bool duck = false;
        vector<Obstacle> obs;
        int score = 0;
        int bestScore = 0;
        int coins = 0;
        bool running = true;
        bool paused = false;
        double gapRemain = 0.0;
    };

public:
    DinoRunner();
    void run();

private:
    Config cfg;
    State st;
    HANDLE hOut;

    void setupConsole();
    void moveHome();
    void clearScreen();
    void restoreCursor();
    void initGame();
    void showTitle();
    int showMenuAndPick();
    void showHelp();
    void handleInput();
    void maybeSpawnObstacle();
    void tick();
    void draw();
    void startGame();
};