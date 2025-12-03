
#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

// 외부 전역 변수 선언
extern int coin;
extern int playerAttack;
extern int playerHp;
extern int petCount;

class Shop {
public:
    void drawShop();
    void enterShop();
    void buyAttack();
    void buyHp();
    void buyPet();
};