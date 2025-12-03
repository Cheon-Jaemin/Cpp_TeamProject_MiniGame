#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

// 외부 전역 변수
extern int playerHp;
extern int playerAttack;
extern int petCount;


class BossBattle {
private:
    int bossHP = 1000;
    int bossAttack = 50;
    int nowHp;
    clock_t lastBossAttack;
    clock_t lastPetAttack;

public:
    BossBattle();
    void clearScreens();
    void drawHPBar(const string& name, int hp, int maxHP, int barWidth = 30);
    void drawBattle();
    void startBattle();
}; 
