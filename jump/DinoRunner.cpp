#include "DinoRunner.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <conio.h>
#include <windows.h>
using namespace std;

// ==================== 생성자 ====================
DinoRunner::DinoRunner() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned)time(nullptr));
    if (cfg.groundY < 2) cfg.groundY = 2;
    if (cfg.groundY > cfg.height - 2) cfg.groundY = cfg.height - 2;
    setupConsole();
}

// ==================== 콘솔 초기화 ====================
void DinoRunner::setupConsole() {
    COORD bufSize = { (SHORT)(cfg.width + 4), (SHORT)(cfg.height + 8) };
    SetConsoleScreenBufferSize(hOut, bufSize);
    SMALL_RECT win = { 0,0, bufSize.X - 1, min<SHORT>(bufSize.Y - 1, 60) };
    SetConsoleWindowInfo(hOut, TRUE, &win);
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &ci);
}

void DinoRunner::moveHome() { SetConsoleCursorPosition(hOut, { 0,0 }); }

void DinoRunner::clearScreen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacterA(hOut, ' ', size, { 0,0 }, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
    moveHome();
}

void DinoRunner::restoreCursor() {
    CONSOLE_CURSOR_INFO ci = { 20, TRUE };
    SetConsoleCursorInfo(hOut, &ci);
}

// ==================== 게임 초기화 ====================
void DinoRunner::initGame() {
    st.y = cfg.groundY;
    st.vy = 0.0;
    st.onGround = true;
    st.duck = false;
    st.obs.clear();
    st.score = 0;
    st.coins = 0;
    st.running = true;
    st.paused = false;
    st.gapRemain = 0.0;
}

// ==================== 메뉴 & 타이틀 ====================
void DinoRunner::showTitle() {
    cout << "         ====================================================\n";
    cout << "         ★★★★★★★  D I N O   R U N N E R ★★★★★★★\n";
    cout << "         ====================================================\n\n";
}

int DinoRunner::showMenuAndPick() {
    clearScreen();
    showTitle();
    cout << " \t\t\t 1) 시작\n";
    cout << " \t\t\t 2) 조작법 보기\n";
    cout << " \t\t\t 3) 종료\n\n";
    cout << "번호를 선택하세요: ";

    while (true) {
        int ch = _getch();
        if (ch == '1') return 1;
        if (ch == '2') return 2;
        if (ch == '3' || ch == 27) return 3;
    }
}

void DinoRunner::showHelp() {
    clearScreen();
    cout << "\n\n[조작법]\n";
    cout << " - 점프: Space 또는 ↑ (선인장 넘기)\n";
    cout << " - 웅크리기(홀드): S 또는 ↓ (새 피하기)\n";
    cout << " - 일시정지: P\n";
    cout << " - 종료: Q\n\n";
    cout << "[룰]\n";
    cout << " - ★ 코인: 점수 400마다 1코인 획득.\n\n";
    cout << "아무 키나 누르면 메뉴로 돌아갑니다...";
    _getch();
}

// ==================== 게임 실행 ====================
void DinoRunner::run() {
    int pick = showMenuAndPick();
    if (pick == 1) startGame();
    else if (pick == 2) showHelp();
    else {
        restoreCursor();
        clearScreen();
        cout << "게임을 종료합니다.\n";
    }
}