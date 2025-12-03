#include "PopGame.h"

PoopGame::PoopGame() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    bestScore = 0;
    prevPlayerX = -1;
    setupConsole();
}

void PoopGame::run() {
    runGame();
}

void PoopGame::setupConsole() {
    COORD bufSize = { (SHORT)(cfg.width + 4), (SHORT)(cfg.height + 12) };
    SetConsoleScreenBufferSize(hOut, bufSize);
    SMALL_RECT win = { 0,0, bufSize.X - 1, min<SHORT>(bufSize.Y - 1, 60) };
    SetConsoleWindowInfo(hOut, TRUE, &win);

    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hOut, &ci);
}

void PoopGame::restoreCursor() {
    CONSOLE_CURSOR_INFO ci = { 20, TRUE };
    SetConsoleCursorInfo(hOut, &ci);
}

void PoopGame::moveHome() { SetConsoleCursorPosition(hOut, { 0,0 }); }

void PoopGame::clearScreen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacter(hOut, ' ', size, { 0,0 }, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
    moveHome();
}

void PoopGame::initBoard() {
    poops.clear();
    score = 0;
    level = 1;
    running = true;
    paused = false;
    playerX = cfg.width / 2;
    prevPlayerX = -1;
    prevPoops.clear();
}

void PoopGame::handleInput() {
    if (!_kbhit()) return;
    int ch = _getch();
    if (ch == 224 || ch == 0) {
        int arrow = _getch();
        if (arrow == 75) playerX = max(0, playerX - 1);
        else if (arrow == 77) playerX = min(cfg.width - 1, playerX + 1);
        return;
    }
    ch = tolower(ch);
    if (ch == 'a') playerX = max(0, playerX - 1);
    else if (ch == 'd') playerX = min(cfg.width - 1, playerX + 1);
    else if (ch == 'p') paused = !paused;
    else if (ch == 'q') running = false;
}

void PoopGame::maybeSpawnPoop() {
    double p = cfg.baseSpawnProb + (level - 1) * cfg.spawnIncPerLevel;
    if (p > 0.9) p = 0.9;
    for (int i = 0; i < 2; ++i) {
        double r = (double)rand() / RAND_MAX;
        if (r < p) poops.push_back({ rand() % cfg.width, 0 });
    }
}

void PoopGame::tick() {
    level = max(1, score / cfg.levelUpEvery + 1);
    maybeSpawnPoop();
    for (auto& po : poops) po.y++;
    poops.erase(remove_if(poops.begin(), poops.end(),
        [&](const Poop& po) { return po.y >= cfg.height; }),
        poops.end());
    for (const auto& po : poops) {
        if (po.y == cfg.playerY && po.x == playerX) {
            running = false;
            break;
        }
    }
    score++;
}

void PoopGame::draw() {
    COORD pos = { 0,0 };
    SetConsoleCursorPosition(hOut, pos);
    cout << "[똥피하기]  점수: " << score
        << "   레벨: " << level
        << "   최고점: " << bestScore << "        \n";
    cout << "   (A/D 또는 ←/→ 이동, P 일시정지, Q 종료)\n";

    for (auto& po : prevPoops) {
        if (po.y >= 0 && po.y < cfg.height && po.x >= 0 && po.x < cfg.width) {
            COORD c = { (SHORT)po.x, (SHORT)(po.y + 3) };
            SetConsoleCursorPosition(hOut, c);
            cout << ' ';
        }
    }

    for (auto& po : poops) {
        if (po.y >= 0 && po.y < cfg.height && po.x >= 0 && po.x < cfg.width) {
            COORD c = { (SHORT)po.x, (SHORT)(po.y + 3) };
            SetConsoleCursorPosition(hOut, c);
            cout << '$';
        }
    }

    if (prevPlayerX >= 0) {
        COORD c = { (SHORT)prevPlayerX, (SHORT)(cfg.playerY + 3) };
        SetConsoleCursorPosition(hOut, c);
        cout << ' ';
    }

    COORD c = { (SHORT)playerX, (SHORT)(cfg.playerY + 3) };
    SetConsoleCursorPosition(hOut, c);
    cout << '@';

    prevPlayerX = playerX;
    prevPoops = poops;

    cout.flush();
}

int PoopGame::delayForLevel() { return max(20, cfg.baseDelay - (level - 1) * cfg.speedGainPerLevel); }

void PoopGame::showTitle() {
    cout << "=====================================\n";
    cout << "           똥   피   하   기         \n";
    cout << "=====================================\n\n";
}

void PoopGame::showHelp() {
    clearScreen();
    showTitle();
    cout << "[조작법]\n";
    cout << " - 이동: A/D 또는 ←/→\n";
    cout << " - 일시정지: P\n";
    cout << " - 게임 중 메뉴로: Q (게임 종료)\n\n";
    cout << "[코인 규칙]\n";
    cout << " - 점수 500 이하 → 1코인\n";
    cout << " - 점수 500~1000 → 3코인\n";
    cout << " - 점수 1000 이상 → 5코인\n";
    cout << "아무 키나 누르면 메뉴로 돌아갑니다...";
    _getch();
}

int PoopGame::showMenuAndPick() {
    clearScreen();
    showTitle();
    cout << "  1) 시작\n";
    cout << "  2) 조작법 보기\n";
    cout << "  3) 종료\n\n";
    cout << "번호를 선택하세요: ";
    while (true) {
        int ch = _getch();
        if (ch == '1') {
            clearScreen();
            return 1;
        }
        if (ch == '2') return 2;
        if (ch == '3' || ch == 27) return 3;
    }
}

void PoopGame::runGame() {
    int pick = showMenuAndPick();
    if (pick == 1) start();
    else if (pick == 2) showHelp();
}

void PoopGame::start() {
    initBoard();

    while (running) {
        handleInput();
        if (!paused) tick();
        draw();
        Sleep(delayForLevel());
    }

    bestScore = max(bestScore, score);

    int coin = (score < 500) ? 1 : (score <= 1000) ? 3 : 5;

    moveHome();
    cout << "\n\n================ GAME OVER ================\n";
    cout << " 점수: " << score << "  최고점수: " << bestScore
        << "  현재 코인: " << coin << "\n";

    Sleep(3000);
    clearScreen();
}