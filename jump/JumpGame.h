#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

extern int coin; 

class JumpGame {
public:
    JumpGame();
    void start();

private:
    // 상수, 상태, 맵 선언
    const int WIDTH;
    const int HEIGHT;
    const int MAX_LEVEL;
    const int MAX_JUMP;

    int Heart;
    int resetcount;
    int playerX;
    int playerY;
    char playerChar;
    bool jumping;
    int jumpHeight;
    int currentLevel;
    bool gameWin;
    bool restartLevel;
    bool doubleJumpAvailable;

    string maps[5][10];
    char map[10][31];

    void loadLevel(int level);
    void gotoxy(int x, int y);
    bool isOnGround();
    void draw();
    void input();
    void jump();
    void update();
    void clearScreen();
    void showEndMessage();
    void chooseCharacter();
};
