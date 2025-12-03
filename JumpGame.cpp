#include "JumpGame.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

JumpGame::JumpGame()
    : WIDTH(30), HEIGHT(10), MAX_LEVEL(5), MAX_JUMP(3)
{
    playerChar = '@';
    playerX = 1;
    playerY = HEIGHT - 2;
    jumping = false;
    jumpHeight = 0;
    currentLevel = 0;
    gameWin = false;
    restartLevel = false;
    doubleJumpAvailable = false;
    Heart = 3;
    resetcount = 3;

    // maps 배열 초기화 (기존 코드)
    string temp[5][10] = {
        {   // Level 1
            "##############################",
            "#                            #",
            "#     C             E        #",
            "#     C    X       ###       #",
            "#     C          X           #",
            "#             ####           #",
            "#         ##                 #",
            "#      X                     #",
            "#C                       J   #",
            "##############################"
        },
        {   // Level 2
            "##############################",
            "#    H   X      C   X        #",
            "#   ###        ##    E       #",
            "# X           X    ###       #",
            "#  X                        X#",
            "#    X        ####           #",
            "#       ####   X             #",
            "#   X                        #",
            "#                            #",
            "##############################"
        },
        {   // Level 3
            "##############################",
            "# X C           X       E X #",
            "#   ###   ###   ###   ###   #",
            "#   X   X   X       X   X   #",
            "# #### #### #### #### ####  #",
            "#      X     X     X     X  #",
            "#   ###   ###   ###   ###   #",
            "# X                        X#",
            "#                           #",
            "##############################"
        },
        {   // Level 4
            "##############################",
            "#   C X       X       X J   #",
            "#  ### ### ### ### ### ###  #",
            "# X   X   X  X    X   X   X #",
            "#   ######     ######       #",
            "#X   H    X X       X X     #",
            "#   ###     ###     ###     #",
            "# X      X       X       X  #",
            "#        X         X      E #",
            "#############################"
        },
        {   // Level 5
            "##############################",
            "#                         E  #",
            "#                          X #",
            "#                        X   #",
            "#                      X     #",
            "#                  XX        #",
            "#               XX           #",
            "#            XX              #",
            "#         XX               C #",
            "##############################"
        }
    };

    for (int l = 0; l < MAX_LEVEL; ++l)
        for (int y = 0; y < HEIGHT; ++y)
            maps[l][y] = temp[l][y];
}

void JumpGame::gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool JumpGame::isOnGround() {
    return map[playerY + 1][playerX] == '#';
}

void JumpGame::loadLevel(int level) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            map[y][x] = maps[level][y][x];
        }
        map[y][WIDTH] = '\0';
    }
    playerX = 1;
    playerY = HEIGHT - 2;
    jumping = false;
    jumpHeight = 0;
}

void JumpGame::draw() {
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) cout << playerChar;
            else cout << map[y][x];
        }
        cout << '\n';
    }
    cout << "체력: " << Heart << "   레벨: " << currentLevel + 1 << "/" << MAX_LEVEL << "\n";
    cout << "현재 코인: " << coin << "        다시하기 가능 기회: " << resetcount << endl;
    cout << "게임 중 R을 눌러 재시작 가능\n";
}

void JumpGame::input() {
    if (GetAsyncKeyState('A') & 0x8000) {
        if (map[playerY][playerX - 1] != '#') playerX--;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        if (map[playerY][playerX + 1] != '#') playerX++;
    }
    if ((GetAsyncKeyState(' ') & 0x8000) && !jumping && isOnGround()) {
        jumping = true;
        jumpHeight = MAX_JUMP;
    }
    if (GetAsyncKeyState('R') & 0x8000 && resetcount > 0) {
        restartLevel = true;
        resetcount--;
    }
}

void JumpGame::jump() {
    if (jumping) {
        if (jumpHeight > 0 && map[playerY - 1][playerX] != '#') {
            playerY--;
            jumpHeight--;
        }
        else {
            jumping = false;
        }
    }
}

void JumpGame::update() {
    jump();

    if (map[playerY + 1][playerX] == 'X') {
        map[playerY + 1][playerX] = ' ';
        jumping = true;
        jumpHeight = MAX_JUMP;
    }
    else if (map[playerY][playerX] == 'X') {
        Heart--;
        map[playerY][playerX] = ' ';
    }
    if (!jumping && !isOnGround()) playerY++;

    if (map[playerY][playerX] == 'E') {
        currentLevel++;
        if (currentLevel >= MAX_LEVEL) gameWin = true;
        else loadLevel(currentLevel);
    }
    if (map[playerY][playerX] == 'H') {
        Heart++;
        map[playerY][playerX] = ' ';
    }
    if (map[playerY][playerX] == 'J') {
        doubleJumpAvailable = true;
        map[playerY][playerX] = ' ';
    }
    if (map[playerY][playerX] == 'C') {
        coin++;
        map[playerY][playerX] = ' ';
    }
}

void JumpGame::clearScreen() { system("cls"); }

void JumpGame::showEndMessage() {
    clearScreen();
    if (gameWin) {
        cout << "모든 레벨 클리어!\n";
        cout << "추가코인 10개 증정!!" << endl;
        coin += 10;
        Sleep(3000);
    }
    else {
        cout << "게임 오버!\n";
        Sleep(3000);
    }
}

void JumpGame::chooseCharacter() {
    clearScreen();
    cout << "캐릭터를 선택하세요:\n";
    cout << "현재코인: " << coin << endl;
    cout << "1. @ 능력없음\n2. & 벽점프 가능 (5코인)\n3. # 기본체력 4칸 (1코인)\n";
    char choice;
    while (true) {
        choice = _getch();
        if (choice == '1') { playerChar = '@'; break; }
        else if (choice == '2' && coin >= 5) { playerChar = '&'; coin -= 5; break; }
        else if (choice == '3' && coin >= 1) { playerChar = '#'; coin -= 1; Heart = 4; break; }
    }
}

void JumpGame::start() {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    while (true) {
        currentLevel = 0; Heart = 3; resetcount = 3; gameWin = false; restartLevel = false;

        clearScreen();
        cout << "==============================\n";
        cout << "        Game Start!\n";
        cout << "  아무 키나 누르면 시작합니다.\n";
        cout << "==============================\n";
        system("pause > nul");

        chooseCharacter();
        loadLevel(currentLevel);

        while (true) {
            draw();
            input();
            update();
            Sleep(50);

            if (Heart <= 0 || gameWin) break;
            if (restartLevel) { loadLevel(currentLevel); restartLevel = false; }
        }

        showEndMessage();
        cout << "\nR을 눌러 다시 시작, 아무 키나 누르면 종료합니다.\n";
        char ch = _getch();
        if (ch != 'r' && ch != 'R') break;
    }
}