//#include <iostream>
//#include <vector>
//#include <string>
//#include <cstdlib>
//#include <ctime>
//#include <cctype>
//#include <algorithm>
//
//#define nominmax
//#include <conio.h>
//#include <windows.h>
//
//using namespace std;
//
//
////점프맵
//
//static int coin=0;
//
//class DinoRunner {
//public:
//    struct Config {
//        int   width = 70;
//        int   height = 26;
//        int   groundY = 22;
//        int   baseDelay = 18;
//        double baseSpeed = 1.1;
//        double gravity = 0.9;
//        double jumpV = -4.5;
//        int    playerX = 10;
//        double cactusSpawnProb = 0.10;
//        double birdSpawnProb = 0.06;
//        int    birdWidth = 3;
//        int    minGapTiles = 22;
//        int    maxGapTiles = 34;
//        int    cactusHeight = 2;
//        int    scorePerCoin = 400;
//    };
//
//    enum class ObType { CACTUS_LOW, BIRD_HIGH };
//
//    struct Obstacle {
//        ObType type;
//        double x;
//        int h;
//    };
//
//    struct State {
//        double y = 0.0;
//        double vy = 0.0;
//        bool   onGround = true;
//        bool   duck = false;
//        vector<Obstacle> obs;
//        int    score = 0;
//        int    bestScore = 0;
//        int    coins = 0;
//        bool   running = true;
//        bool   paused = false;
//        double gapRemain = 0.0;
//    };
//
//private:
//    Config cfg;
//    State st;
//    HANDLE hOut;
//
//public:
//    DinoRunner() {
//        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//        srand((unsigned)time(nullptr));
//        if (cfg.groundY < 2) cfg.groundY = 2;
//        if (cfg.groundY > cfg.height - 2) cfg.groundY = cfg.height - 2;
//        setupConsole();
//    }
//
//    void run() {
//        int pick = showMenuAndPick();
//        if (pick == 1) {
//            startGame();
//            return; // 게임 종료 후 바로 run() 종료
//        }
//        else if (pick == 2) {
//            showHelp();
//            return; // 도움말 보고 나와도 종료
//        }
//        else {
//            restoreCursor();
//            clearScreen();
//            cout << "게임을 종료합니다.\n";
//            return;
//        }
//    }
//
//private:
//    void setupConsole() {
//        COORD bufSize = { (SHORT)(cfg.width + 4), (SHORT)(cfg.height + 8) };
//        SetConsoleScreenBufferSize(hOut, bufSize);
//        SMALL_RECT win = { 0,0, bufSize.X - 1, min<SHORT>(bufSize.Y - 1, 60) };
//        SetConsoleWindowInfo(hOut, TRUE, &win);
//        CONSOLE_CURSOR_INFO ci = { 1, FALSE };
//        SetConsoleCursorInfo(hOut, &ci);
//    }
//
//    void moveHome() { SetConsoleCursorPosition(hOut, { 0,0 }); }
//
//    void clearScreen() {
//        CONSOLE_SCREEN_BUFFER_INFO csbi;
//        GetConsoleScreenBufferInfo(hOut, &csbi);
//        DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
//        DWORD written;
//        FillConsoleOutputCharacterA(hOut, ' ', size, { 0,0 }, &written);
//        FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
//        moveHome();
//    }
//
//    void restoreCursor() {
//        CONSOLE_CURSOR_INFO ci = { 20, TRUE };
//        SetConsoleCursorInfo(hOut, &ci);
//    }
//
//    void initGame() {
//        st.y = cfg.groundY;
//        st.vy = 0.0;
//        st.onGround = true;
//        st.duck = false;
//        st.obs.clear();
//        st.score = 0;
//        st.coins = 0;
//        st.running = true;
//        st.paused = false;
//        st.gapRemain = 0.0;
//    }
//
//    void showTitle() {
//        cout << "         ====================================================\n";
//        cout << "         ★★★★★★★  D I N O   R U N N E R ★★★★★★★\n";
//        cout << "         ====================================================\n\n";
//    }
//
//    int showMenuAndPick() {
//        clearScreen();
//        showTitle();
//        cout << " \t\t\t 1) 시작\n";
//        cout << " \t\t\t 2) 조작법 보기\n";
//        cout << " \t\t\t 3) 종료\n\n";
//        cout << "번호를 선택하세요: ";
//
//        while (true) {
//            int ch = _getch();
//            if (ch == '1') return 1;
//            if (ch == '2') return 2;
//            if (ch == '3' || ch == 27) return 3;
//        }
//    }
//
//    void showHelp() {
//        clearScreen();
//        cout << "\n\n[조작법]\n";
//        cout << " - 점프: Space 또는 ↑ (선인장 넘기)\n";
//        cout << " - 웅크리기(홀드): S 또는 ↓ (새 피하기)\n";
//        cout << " - 일시정지: P\n";
//        cout << " - 종료: Q\n\n";
//        cout << "[룰]\n";
//        cout << " - ★ 코인: 점수 400마다 1코인 획득.\n\n";
//        cout << "아무 키나 누르면 메뉴로 돌아갑니다...";
//        _getch();
//    }
//
//    void handleInput() {
//        if (_kbhit()) {
//            int ch = _getch();
//            if (ch == 224 || ch == 0) {
//                int arrow = _getch();
//                if (arrow == 72 && st.onGround) { st.onGround = false; st.vy = cfg.jumpV; st.duck = false; }
//            }
//            else {
//                ch = tolower(ch);
//                if (ch == ' ' && st.onGround) { st.onGround = false; st.vy = cfg.jumpV; st.duck = false; }
//                else if (ch == 'p') st.paused = !st.paused;
//                else if (ch == 'q') st.running = false;
//            }
//        }
//        SHORT downHeld = GetAsyncKeyState(VK_DOWN) & 0x8000;
//        SHORT sHeld = GetAsyncKeyState('S') & 0x8000;
//        st.duck = st.onGround && (downHeld || sHeld);
//    }
//
//    void maybeSpawnObstacle() {
//        if (st.gapRemain > 0.0) return;
//        bool tryBirdFirst = (rand() % 2) == 0;
//
//        auto spawnCactus = [&]() -> bool {
//            double r = (double)rand() / RAND_MAX;
//            if (r < cfg.cactusSpawnProb) {
//                Obstacle o{ ObType::CACTUS_LOW, (double)(cfg.width - 2), cfg.cactusHeight };
//                st.obs.push_back(o);
//                st.gapRemain = cfg.minGapTiles + rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1);
//                return true;
//            }
//            return false;
//            };
//
//        auto spawnBird = [&]() -> bool {
//            double r = (double)rand() / RAND_MAX;
//            if (r < cfg.birdSpawnProb) {
//                Obstacle o{ ObType::BIRD_HIGH, (double)cfg.width, 1 };
//                st.obs.push_back(o);
//                st.gapRemain = cfg.minGapTiles + rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1);
//                return true;
//            }
//            return false;
//            };
//
//        if (tryBirdFirst) { if (!spawnBird()) spawnCactus(); }
//        else { if (!spawnCactus()) spawnBird(); }
//    }
//
//    void tick() {
//        const double speed = cfg.baseSpeed;
//        maybeSpawnObstacle();
//
//        st.vy += cfg.gravity;
//        st.y += st.vy;
//        if (st.y >= cfg.groundY) { st.y = cfg.groundY; st.vy = 0; st.onGround = true; }
//        else st.onGround = false;
//        if (!st.onGround) st.duck = false;
//
//        for (auto& o : st.obs) o.x -= speed;
//        st.obs.erase(remove_if(st.obs.begin(), st.obs.end(), [&](const Obstacle& o) { return o.x < -4.0; }), st.obs.end());
//
//        if (st.gapRemain > 0.0) { st.gapRemain -= speed; if (st.gapRemain < 0.0) st.gapRemain = 0.0; }
//
//        int pX = cfg.playerX;
//        int footY = (int)round(st.y);
//        int headY = st.duck ? footY : footY - 1;
//
//        for (const auto& o : st.obs) {
//            int ox = (int)round(o.x);
//            if (o.type == ObType::CACTUS_LOW) {
//                int topY = cfg.groundY - (o.h - 1);
//                bool xHit = (ox >= pX - 1 && ox <= pX + 1);
//                bool yHit = !(footY < topY || headY > cfg.groundY);
//                if (xHit && yHit) { st.running = false; break; }
//            }
//            else {
//                int birdY = cfg.groundY - 1;
//                bool yHit = (headY == birdY || footY == birdY);
//                bool xHit = (ox >= pX - 2 && ox <= pX + 2);
//                if (xHit && yHit && !st.duck) { st.running = false; break; }
//            }
//        }
//
//        st.score += 1;
//        int shouldCoins = st.score / cfg.scorePerCoin;
//        if (shouldCoins > st.coins) st.coins = shouldCoins;
//    }
//
//    void draw() {
//        moveHome();
//        cout << "[DINO RUNNER]  점수: " << st.score << "   코인: " << st.coins << "   최고점: " << st.bestScore << "\n";
//        cout << "  (Space/↑ 점프, S/↓ 숙이기, P 일시정지, Q 종료)\n";
//
//        vector<string> disp(cfg.height, string(cfg.width, ' '));
//        for (int x = 0; x < cfg.width; ++x) if (cfg.groundY >= 0 && cfg.groundY < cfg.height) disp[cfg.groundY][x] = '_';
//
//        for (const auto& o : st.obs) {
//            int ox = (int)round(o.x);
//            if (o.type == ObType::CACTUS_LOW) {
//                for (int k = 0; k < o.h; ++k) {
//                    int y = cfg.groundY - k;
//                    if (0 <= y && y < cfg.height && 0 <= ox && ox < cfg.width) disp[y][ox] = '|';
//                }
//            }
//            else {
//                int y = cfg.groundY - 1;
//                if (0 <= y && y < cfg.height) {
//                    if (ox - 1 >= 0 && ox - 1 < cfg.width) disp[y][ox - 1] = '<';
//                    if (0 <= ox && ox < cfg.width) disp[y][ox] = '^';
//                    if (ox + 1 >= 0 && ox + 1 < cfg.width) disp[y][ox + 1] = '>';
//                }
//            }
//        }
//
//        int pX = cfg.playerX;
//        int footY = (int)round(st.y);
//        if (st.duck) {
//            if (0 <= footY && footY < cfg.height) {
//                if (pX - 1 >= 0) disp[footY][pX - 1] = '[';
//                if (0 <= pX && pX < cfg.width) disp[footY][pX] = 'd';
//                if (pX + 1 < cfg.width) disp[footY][pX + 1] = ']';
//            }
//        }
//        else {
//            int headY = footY - 1;
//            if (0 <= headY && headY < cfg.height && 0 <= pX && pX < cfg.width) disp[headY][pX] = 'O';
//            if (0 <= footY && footY < cfg.height) {
//                if (0 <= pX && pX < cfg.width) disp[footY][pX] = '#';
//                if (pX - 1 >= 0) disp[footY][pX - 1] = '/';
//                if (pX + 1 < cfg.width) disp[footY][pX + 1] = '\\';
//            }
//        }
//
//        cout << '+' << string(cfg.width, '-') << "+\n";
//        for (int y = 0; y < cfg.height; ++y) cout << '|' << disp[y] << "|\n";
//        cout << '+' << string(cfg.width, '-') << "+\n\n";
//        if (st.paused) cout << "=== 일시정지 (P: 해제) ===\n\n";
//    }
//
//    void startGame() {
//        initGame();
//        while (st.running) {
//            handleInput();
//            if (!st.paused) tick();
//            draw();
//            Sleep(max(1, cfg.baseDelay));
//        }
//
//        // 게임 종료 후 화면 정리
//        restoreCursor();   // 커서 보이기
//        clearScreen();     // 화면 초기화
//        cout << "게임이 종료되었습니다....\n";
//        Sleep(3000);       // 3초 정도 딜레이
//        return;            // main()으로 돌아감
//    }
//};
//
//
//struct Config {
//    int width = 55;
//    int height = 31;
//    int playerY = 30;
//    int baseDelay = 80;
//    int speedGainPerLevel = 5;
//    int levelUpEvery = 150;
//    double baseSpawnProb = 0.18;
//    double spawnIncPerLevel = 0.03;
//};
//
//struct Poop {
//    int x, y;
//};
//
//class PoopGame {
//private:
//    Config cfg;
//    vector<Poop> poops;
//    int playerX;
//    int score;
//    int level;
//    int bestScore = 0;
//    bool running;
//    bool paused;
//    HANDLE hOut;
//
//    int prevPlayerX = -1;
//    vector<Poop> prevPoops;
//
//public:
//    PoopGame() {
//        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//        setupConsole();
//    }
//
//    void run() { runGame(); }
//
//private:
//    void setupConsole() {
//        COORD bufSize = { (SHORT)(cfg.width + 4), (SHORT)(cfg.height + 12) };
//        SetConsoleScreenBufferSize(hOut, bufSize);
//        SMALL_RECT win = { 0,0, bufSize.X - 1, min<SHORT>(bufSize.Y - 1, 60) };
//        SetConsoleWindowInfo(hOut, TRUE, &win);
//
//        CONSOLE_CURSOR_INFO ci;
//        ci.bVisible = FALSE;
//        ci.dwSize = 1;
//        SetConsoleCursorInfo(hOut, &ci);
//    }
//
//    void restoreCursor() {
//        CONSOLE_CURSOR_INFO ci = { 20, TRUE };
//        SetConsoleCursorInfo(hOut, &ci);
//    }
//
//    void moveHome() { SetConsoleCursorPosition(hOut, { 0,0 }); }
//
//    void clearScreen() {
//        CONSOLE_SCREEN_BUFFER_INFO csbi;
//        GetConsoleScreenBufferInfo(hOut, &csbi);
//        DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
//        DWORD written;
//        FillConsoleOutputCharacter(hOut, ' ', size, { 0,0 }, &written);
//        FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
//        moveHome();
//    }
//
//    void initBoard() {
//        poops.clear();
//        score = 0;
//        level = 1;
//        running = true;
//        paused = false;
//        playerX = cfg.width / 2;
//        prevPlayerX = -1;
//        prevPoops.clear();
//    }
//
//    void handleInput() {
//        if (!_kbhit()) return;
//        int ch = _getch();
//        if (ch == 224 || ch == 0) {
//            int arrow = _getch();
//            if (arrow == 75) playerX = max(0, playerX - 1);
//            else if (arrow == 77) playerX = min(cfg.width - 1, playerX + 1);
//            return;
//        }
//        ch = tolower(ch);
//        if (ch == 'a') playerX = max(0, playerX - 1);
//        else if (ch == 'd') playerX = min(cfg.width - 1, playerX + 1);
//        else if (ch == 'p') paused = !paused;
//        else if (ch == 'q') running = false;
//    }
//
//    void maybeSpawnPoop() {
//        double p = cfg.baseSpawnProb + (level - 1) * cfg.spawnIncPerLevel;
//        if (p > 0.9) p = 0.9;
//        for (int i = 0; i < 2; ++i) {
//            double r = (double)rand() / RAND_MAX;
//            if (r < p) poops.push_back({ rand() % cfg.width, 0 });
//        }
//    }
//
//    void tick() {
//        level = max(1, score / cfg.levelUpEvery + 1);
//        maybeSpawnPoop();
//        for (auto& po : poops) po.y++;
//        poops.erase(remove_if(poops.begin(), poops.end(),
//            [&](const Poop& po) { return po.y >= cfg.height; }),
//            poops.end());
//        for (const auto& po : poops) {
//            if (po.y == cfg.playerY && po.x == playerX) {
//                running = false;
//                break;
//            }
//        }
//        score++;
//    }
//
//    void draw() {
//        // UI 고정
//        COORD pos = { 0,0 };
//        SetConsoleCursorPosition(hOut, pos);
//        cout << "[똥피하기]  점수: " << score
//            << "   레벨: " << level
//            << "   최고점: " << bestScore << "        \n";
//        cout << "   (A/D 또는 ←/→ 이동, P 일시정지, Q 종료)\n";
//
//        // 이전 똥 지우기
//        for (auto& po : prevPoops) {
//            if (po.y >= 0 && po.y < cfg.height && po.x >= 0 && po.x < cfg.width) {
//                COORD c = { (SHORT)po.x, (SHORT)(po.y + 3) };
//                SetConsoleCursorPosition(hOut, c);
//                cout << ' ';
//            }
//        }
//
//        // 현재 똥 그리기
//        for (auto& po : poops) {
//            if (po.y >= 0 && po.y < cfg.height && po.x >= 0 && po.x < cfg.width) {
//                COORD c = { (SHORT)po.x, (SHORT)(po.y + 3) };
//                SetConsoleCursorPosition(hOut, c);
//                cout << '$';
//            }
//        }
//
//        // 이전 플레이어 지우기
//        if (prevPlayerX >= 0) {
//            COORD c = { (SHORT)prevPlayerX, (SHORT)(cfg.playerY + 3) };
//            SetConsoleCursorPosition(hOut, c);
//            cout << ' ';
//        }
//
//        // 플레이어 그리기
//        COORD c = { (SHORT)playerX, (SHORT)(cfg.playerY + 3) };
//        SetConsoleCursorPosition(hOut, c);
//        cout << '@';
//
//        prevPlayerX = playerX;
//        prevPoops = poops;
//
//        cout.flush();
//    }
//
//    int delayForLevel() { return max(20, cfg.baseDelay - (level - 1) * cfg.speedGainPerLevel); }
//
//    void showTitle() {
//        cout << "=====================================\n";
//        cout << "           똥   피   하   기         \n";
//        cout << "=====================================\n\n";
//    }
//
//    void showHelp() {
//        clearScreen();
//        showTitle();
//        cout << "[조작법]\n";
//        cout << " - 이동: A/D 또는 ←/→\n";
//        cout << " - 일시정지: P\n";
//        cout << " - 게임 중 메뉴로: Q (게임 종료)\n\n";
//        cout << "[코인 규칙]\n";
//        cout << " - 점수 500 이하 → 1코인\n";
//        cout << " - 점수 500~1000 → 3코인\n";
//        cout << " - 점수 1000 이상 → 5코인\n";
//        cout << "아무 키나 누르면 메뉴로 돌아갑니다...";
//        _getch();
//    }
//
//    int showMenuAndPick() {
//        clearScreen();
//        showTitle();
//        cout << "  1) 시작\n";
//        cout << "  2) 조작법 보기\n";
//        cout << "  3) 종료\n\n";
//        cout << "번호를 선택하세요: ";
//        while (true) {
//            int ch = _getch();
//            if (ch == '1') {
//                clearScreen(); // 메뉴 선택 후 바로 화면 지우기
//                return 1;
//            }
//            if (ch == '2') return 2;
//            if (ch == '3' || ch == 27) return 3;
//        }
//    }
//
//    void runGame() {
//        int pick = showMenuAndPick();
//        if (pick == 1) start();  // 게임 실행
//        else if (pick == 2) showHelp();  // 도움말
//        // pick == 3이면 그냥 return
//    }
//
//    void start() {
//        initBoard();
//
//        while (running) {
//            handleInput();
//            if (!paused) tick();
//            draw();
//            Sleep(delayForLevel());
//        }
//
//        bestScore = max(bestScore, score);
//
//        int coin = (score < 500) ? 1 : (score <= 1000) ? 3 : 5;
//
//        // 게임 종료 메시지
//        moveHome();
//        cout << "\n\n================ GAME OVER ================\n";
//        cout << " 점수: " << score << "  최고점수: " << bestScore
//            << "  현재 코인: " << coin << "\n";
//
//        // 3초 정도 잠시 보여주고, 화면 정리
//        Sleep(3000);
//        clearScreen();  // 이 게임 화면만 초기화
//        return;         // runGame()로 돌아가서 다음 게임 실행 가능
//    }
//};
//
//
//
//
//class JumpGame {
//public:
//    // 상수
//    const int WIDTH = 30;
//    const int HEIGHT = 10;
//    const int MAX_LEVEL = 5;
//    const int MAX_JUMP = 3;
//
//    // 상태
//    int Heart;
//    int resetcount;
//    int playerX;
//    int playerY;
//    char playerChar;
//    bool jumping;
//    int jumpHeight;
//    int currentLevel;
//    bool gameWin;
//    bool restartLevel;
//    bool doubleJumpAvailable;
//
//    string maps[5][10] = {
//        {   // Level 1
//            "##############################",
//            "#                            #",
//            "#     C             E        #",
//            "#     C    X       ###       #",
//            "#     C          X           #",
//            "#             ####           #",
//            "#         ##                 #",
//            "#      X                     #",
//            "#C                       J   #",
//            "##############################"
//        },
//        {   // Level 2
//            "##############################",
//            "#    H   X      C   X        #",
//            "#   ###        ##    E       #",
//            "# X           X    ###       #",
//            "#  X                        X#",
//            "#    X        ####           #",
//            "#       ####   X             #",
//            "#   X                        #",
//            "#                            #",
//            "##############################"
//        },
//        {   // Level 3
//            "##############################",
//            "# X C           X       E X #",
//            "#   ###   ###   ###   ###   #",
//            "#   X   X   X       X   X   #",
//            "# #### #### #### #### ####  #",
//            "#      X     X     X     X  #",
//            "#   ###   ###   ###   ###   #",
//            "# X                        X#",
//            "#                           #",
//            "##############################"
//        },
//        {   // Level 4
//            "##############################",
//            "#   C X       X       X J   #",
//            "#  ### ### ### ### ### ###  #",
//            "# X   X   X  X    X   X   X #",
//            "#   ######     ######       #",
//            "#X   H    X X       X X     #",
//            "#   ###     ###     ###     #",
//            "# X      X       X       X  #",
//            "#        X         X      E #",
//            "#############################"
//        },
//        {   // Level 5
//            "##############################",
//            "#                         E  #",
//            "#                          X #",
//            "#                        X   #",
//            "#                      X     #",
//            "#                  XX        #",
//            "#               XX           #",
//            "#            XX              #",
//            "#         XX               C #",
//            "##############################"
//        }
//    };
//
//    char map[10][31]; // 맵 복사용
//
//    JumpGame() {
//        playerChar = '@';
//        playerX = 1;
//        playerY = HEIGHT - 2;
//        jumping = false;
//        jumpHeight = 0;
//        currentLevel = 0;
//        gameWin = false;
//        restartLevel = false;
//        doubleJumpAvailable = false;
//    }
//
//    void loadLevel(int level) {
//        for (int y = 0; y < HEIGHT; y++) {
//            for (int x = 0; x < WIDTH; x++) {
//                map[y][x] = maps[level][y][x];
//            }
//            map[y][WIDTH] = '\0';
//        }
//        playerX = 1;
//        playerY = HEIGHT - 2;
//        jumping = false;
//        jumpHeight = 0;
//    }
//
//    void gotoxy(int x, int y) {
//        COORD coord = { (SHORT)x, (SHORT)y };
//        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
//    }
//
//    bool isOnGround() {
//        return map[playerY + 1][playerX] == '#';
//    }
//
//    void draw() {
//        gotoxy(0, 0);
//        for (int y = 0; y < HEIGHT; y++) {
//            for (int x = 0; x < WIDTH; x++) {
//                if (x == playerX && y == playerY) cout << playerChar;
//                else cout << map[y][x];
//            }
//            cout << '\n';
//        }
//        cout << "체력: " << Heart << "   레벨: " << currentLevel + 1 << "/" << MAX_LEVEL << "\n";
//        cout << "현재 코인: " << coin << "        다시하기 가능 기회: " << resetcount << endl;
//        cout << "게임 중 R을 눌러 재시작 가능\n";
//    }
//
//    void input() {
//        if (GetAsyncKeyState('A') & 0x8000) {
//            if (map[playerY][playerX - 1] != '#') playerX--;
//        }
//        if (GetAsyncKeyState('D') & 0x8000) {
//            if (map[playerY][playerX + 1] != '#') playerX++;
//        }
//        if ((GetAsyncKeyState(' ') & 0x8000) && !jumping && isOnGround()) {
//            jumping = true;
//            jumpHeight = MAX_JUMP;
//        }
//        if (GetAsyncKeyState('R') & 0x8000 && resetcount > 0) {
//            restartLevel = true;
//            resetcount--;
//        }
//    }
//
//    void jump() {
//        if (jumping) {
//            if (jumpHeight > 0 && map[playerY - 1][playerX] != '#') {
//                playerY--;
//                jumpHeight--;
//            }
//            else {
//                jumping = false;
//            }
//        }
//    }
//
//    void update() {
//        jump();
//
//        if (map[playerY + 1][playerX] == 'X') {
//            map[playerY + 1][playerX] = ' ';
//            jumping = true;
//            jumpHeight = MAX_JUMP;
//        }
//        else if (map[playerY][playerX] == 'X') {
//            Heart--;
//            map[playerY][playerX] = ' ';
//        }
//        if (!jumping && !isOnGround()) playerY++;
//
//        if (map[playerY][playerX] == 'E') {
//            currentLevel++;
//            if (currentLevel >= MAX_LEVEL) gameWin = true;
//            else loadLevel(currentLevel);
//        }
//        if (map[playerY][playerX] == 'H') {
//            Heart++;
//            map[playerY][playerX] = ' ';
//        }
//        if (map[playerY][playerX] == 'J') {
//            doubleJumpAvailable = true;
//            map[playerY][playerX] = ' ';
//        }
//        if (map[playerY][playerX] == 'C') {
//            coin++;
//            map[playerY][playerX] = ' ';
//        }
//    }
//
//    void clearScreen() { system("cls"); }
//
//    void showEndMessage() {
//        clearScreen();
//        if (gameWin) {
//            cout << "모든 레벨 클리어!\n";
//            cout << "추가코인 10개 증정!!" << endl;
//            coin += 10;
//            Sleep(3000);
//        }
//        else {
//            cout << "게임 오버!\n";
//            Sleep(3000);
//        }
//    }
//
//    void chooseCharacter() {
//        clearScreen();
//        cout << "캐릭터를 선택하세요:\n";
//        cout << "현재코인: " << coin << endl;
//        cout << "1. @ 능력없음\n2. & 벽점프 가능 (5코인)\n3. # 기본체력 4칸 (1코인)\n";
//        char choice;
//        while (true) {
//            choice = _getch();
//            if (choice == '1') { playerChar = '@'; break; }
//            else if (choice == '2' && coin >= 5) { playerChar = '&'; coin -= 5; break; }
//            else if (choice == '3' && coin >= 1) { playerChar = '#'; coin -= 1; Heart = 4; break; }
//        }
//    }
//
//    void start() {
//        CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
//        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
//
//        while (true) {
//            currentLevel = 0; Heart = 3; resetcount = 3; gameWin = false; restartLevel = false;
//
//            clearScreen();
//            cout << "==============================\n";
//            cout << "        Game Start!\n";
//            cout << "  아무 키나 누르면 시작합니다.\n";
//            cout << "==============================\n";
//            system("pause > nul");
//
//            chooseCharacter();
//            loadLevel(currentLevel);
//
//            while (true) {
//                draw();
//                input();
//                update();
//                Sleep(50);
//
//                if (Heart <= 0 || gameWin) break;
//                if (restartLevel) { loadLevel(currentLevel); restartLevel = false; }
//            }
//
//            showEndMessage();
//            cout << "\nR을 눌러 다시 시작, 아무 키나 누르면 종료합니다.\n";
//            char ch = _getch();
//            if (ch != 'r' && ch != 'R') break;
//        }
//    }
//};
//
//static int petCount = 0;
//static int nowHp;
//static int bossHP = 1000;
//static int bossAttack = 20;
//
////상점
//class Shop {
//public:
//   
//
//    // 상점 화면 그리기
//    void drawShop() {
//        system("cls");
//        cout << "=========================================\n";
//        cout << "                 🏪 상점 🏪\n";
//        cout << "=========================================\n";
//        cout << R"(
//        (\_ _/)   어서오게, 여행자!
//        ( •w•)    좋은 물건들이 있지~
//        / >🍗
//        )" << endl;
//
//        cout << "-----------------------------------------\n";
//        cout << "1. 🗡 공격력 증가 아이템 (1 코인)\n";
//        cout << "2. ❤ 체력 증가 아이템 (1 코인)\n";
//        cout << "3. 🐾 귀여운 펫 (2 코인, 3초마다 10의 데미지)\n";
//        cout << "-----------------------------------------\n";
//        cout << "당신의 코인: " << coin << "\n";
//        cout << "현재 공격력: " << playerAttack
//            << " / 체력: " << playerHp
//            << " / 펫: " << petCount << " 마리\n";
//        cout << "-----------------------------------------\n";
//        cout << "구매할 아이템 번호를 입력하세요 (0: 종료): ";
//    }
//
//    // 상점 메인 루프
//    void enterShop() {
//        while (true) {
//            drawShop();
//            int choice;
//            cin >> choice;
//
//            if (choice == 0) {
//                cout << "상점을 떠납니다...\n";
//                Sleep(3000);
//                break;
//            }
//            else if (choice == 1) {
//                buyAttack();
//                Sleep(3000);
//            }
//            else if (choice == 2) {
//                buyHp();
//                Sleep(3000);
//            }
//            else if (choice == 3) {
//                buyPet();
//                Sleep(3000);
//            }
//            else {
//                cout << "잘못된 입력입니다.\n";
//                Sleep(3000);
//            }
//
//            cout << "계속하려면 아무 키나 누르세요...\n";
//            Sleep(3000);
//            _getch();
//        }
//    }
//
//    // 공격력 아이템 구매
//    void buyAttack() {
//        if (coin >= 1) {
//            coin -= 1;
//            playerAttack++;
//            cout << "🗡 공격력이 1 증가했습니다! 현재 공격력: " << playerAttack << "\n";
//        }
//        else cout << "코인이 부족합니다!\n";
//    }
//
//    // 체력 아이템 구매
//    void buyHp() {
//        if (coin >= 1) {
//            coin -= 1;
//            playerHp += 10;
//            cout << "❤ 체력이 10 증가했습니다! 현재 체력: " << playerHp << "\n";
//        }
//        else cout << "코인이 부족합니다!\n";
//    }
//
//    // 펫 구매
//    void buyPet() {
//        if (coin >= 2) {
//            coin -= 2;
//            petCount++;
//            cout << "🐾 펫을 샀습니다! 현재 펫 수: " << petCount << " 마리\n";
//
//            if (petCount == 1) {
//                cout << "🐶 귀여운 펫이 당신을 따라다니기 시작합니다!\n";
//            }
//            else if (petCount == 2) {
//                cout << "🐱 두 번째 펫도 합류했어요! 시끌벅적~\n";
//            }
//            else if (petCount == 3) {
//                cout << "🐰 세 번째 펫이 뒤뚱뒤뚱 따라옵니다! 귀여움 +100\n";
//            }
//            else {
//                cout << "🐾 새로운 펫이 무리를 이루어 당신을 따라다닙니다! (총 "
//                    << petCount << " 마리)\n";
//            }
//        }
//        else cout << "코인이 부족합니다!\n";
//    }
//};
//
////보스
//
//
//class BossBattle {
//public:
//    clock_t lastBossAttack;
//    clock_t lastPetAttack;
//
//    BossBattle() {
//        nowHp = playerHp;
//        lastBossAttack = clock();
//        lastPetAttack = clock();
//    }
//
//    void clearScreens() {
//        system("cls");
//    }
//
//    void drawHPBar(const string& name, int hp, int maxHP, int barWidth = 30) {
//        cout << name << " [";
//        int filled = (double)hp / maxHP * barWidth;
//        for (int i = 0; i < barWidth; i++)
//            cout << (i < filled ? "■" : " ");
//        cout << "] " << hp << "/" << maxHP << "\n";
//    }
//
//    void drawBattle() {
//        clearScreens();
//        cout << "=================== ⚔ 보스 전투 ⚔ ===================\n\n";
//
//        cout << R"(
//       /^^^^^^^^^^^\
//      /             \
//     |   (o)   (o)   |
//     |       ^       |
//     |     '-'       |
//      \   \___/     /
//       \___________/
//)";
//
//        drawHPBar("보스 HP", bossHP, 1000);
//        cout << "\n";
//
//        drawHPBar("플레이어 HP", nowHp, playerHp);
//        cout << "플레이어 공격력: " << playerAttack << "\n";
//        cout << "펫 수: " << petCount << " (3초마다 " << (petCount * 30) << " 데미지)\n";
//        cout << "-----------------------------------------------------\n";
//        cout << "스페이스바: 공격   |   보스는 3초마다 공격합니다!\n";
//        cout << "=====================================================\n";
//    }
//
//    void startBattle() {
//        bool spacePressedPrev = false;
//        clock_t lastDraw = 0;
//
//        while (nowHp > 0 && bossHP > 0) {
//            bool spacePressedNow = (GetAsyncKeyState(VK_SPACE) & 0x8000);
//
//            if (spacePressedNow && !spacePressedPrev) {
//                bossHP -= playerAttack;
//                if (bossHP < 0) bossHP = 0;
//                cout << ">> 당신이 공격하여 보스에게 " << playerAttack << " 데미지를 입혔습니다!\n";
//            }
//            spacePressedPrev = spacePressedNow;
//
//            if ((clock() - lastBossAttack) / CLOCKS_PER_SEC >= 3) {
//                nowHp -= bossAttack;
//                if (nowHp < 0) nowHp = 0;
//                cout << "🔥 보스가 강력한 공격을 가해 " << bossAttack << " 피해를 입었습니다!\n";
//                lastBossAttack = clock();
//            }
//
//            if (petCount > 0 && (clock() - lastPetAttack) / CLOCKS_PER_SEC >= 3) {
//                int petDamage = petCount * 30;
//                bossHP -= petDamage;
//                if (bossHP < 0) bossHP = 0;
//                cout << "🐾 펫들이 달려들어 보스에게 " << petDamage << " 피해를 입혔습니다!\n";
//                lastPetAttack = clock();
//            }
//
//            if ((clock() - lastDraw) / (double)CLOCKS_PER_SEC >= 0.2) {
//                drawBattle();
//                lastDraw = clock();
//            }
//
//            Sleep(10);
//        }
//
//        drawBattle();
//        if (bossHP <= 0) cout << "\n🎉 승리했습니다! 보스를 쓰러뜨렸습니다!\n";
//        else cout << "\n💀 패배했습니다... 플레이어가 쓰러졌습니다...\n";
//
//        Sleep(3000);
//        system("pause");
//    }
//};
////int main() {
////
////    DinoRunner dinorunner;
////    dinorunner.run();
////
////    PoopGame popgame;
////    popgame.run();
////
////    JumpGame jumpgame;
////    jumpgame.start();
////
////    Shop shop;
////    shop.enterShop();
////  
////    BossBattle bossbattle;
////    bossbattle.startBattle();
////    //BossGameStart();
////    //shop();
////    //JumpGameStart();
////
////    return 0;
////}
