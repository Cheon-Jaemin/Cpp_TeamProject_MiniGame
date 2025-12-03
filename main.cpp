//#include "Game.h"
//#include "Blackjack.h"
//#include "GameManager.h"
//#include <stdio.h>
//#include "PoopGame.h"
//#include "JumpGame.h"
//#include "Shop.h"
//#include "BossBattle.h"
//#include <iostream>
//#include <vector>
//#include <string>
//#include <cstdlib>
//#include <ctime>
//#include <cctype>
//#include <algorithm>
//#include <cmath>
//#include "dmain.cpp"
//
//#define NOMINMAX
//#include <conio.h>
//#include <windows.h>
//
//
//
//using namespace std;
//int coin = 0;
//int playerAttack = 1;
//int playerHp = 100;
//int petCount = 0;
//int nowHp;
//int bossHP = 1000;
//int bossAttack = 20;
//
//
//class DinoRunner;
//
//
//
//
//struct Configs {
//    int   width = 70;
//    int   height = 26;
//    int   groundY = 22;             // 0(top) 기준
//    int   baseDelay = 18;           // 프레임(ms), Sleep에 그대로 사용
//    double baseSpeed = 1.1;         // 장애물 속도(칸/프레임) — 일정
//
//    // 물리/플레이어
//    double gravity = 0.9;
//    double jumpV = -4.5;            // 점프 초기속도(위로 음수)
//    int   playerX = 10;
//
//    // 스폰 확률(프레임당, 0~1) - 고정
//    double cactusSpawnProb = 0.10;  // 선인장
//    double birdSpawnProb = 0.06;    // 새
//
//    int  birdWidth = 3;             // "<^>" 폭 3
//
//    // 장애물 간 최소/최대 간격(타일)
//    int    minGapTiles = 22;
//    int    maxGapTiles = 34;
//
//    // 선인장 고정 높이 (1~3 권장)
//    int    cactusHeight = 2;
//
//    // 코인 규칙
//    int    scorePerCoin = 400;      // ★ 400점 당 코인 1개
//};
//
//// 장애물 타입
//enum class ObType { CACTUS_LOW, BIRD_HIGH };
//
//struct Obstacle {
//    ObType type;
//    double x;  // 좌측 기준 위치(소수 이동)
//    int    h;  // 높이(선인장: 고정, 새: 1)
//};
//
//struct State {
//    // 플레이어
//    double y = 0.0;
//    double vy = 0.0;
//    bool   onGround = true;
//    bool   duck = false;
//
//    // 월드
//    vector<Obstacle> obs;
//
//    // 진행
//    int    score = 0;
//    int    bestScore = 0;
//    bool   running = true;
//    bool   paused = false;
//
//    // 다음 스폰까지 남은 거리(타일)
//    double gapRemain = 0.0;
//};
//
//static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//static void setupConsole(int width, int height) {
//    COORD bufSize;
//    bufSize.X = width + 4;
//    bufSize.Y = height + 8;
//    SetConsoleScreenBufferSize(hOut, bufSize);
//
//    SMALL_RECT win;
//    win.Left = 0; win.Top = 0;
//    win.Right = bufSize.X - 1;
//    win.Bottom = std::min<SHORT>(bufSize.Y - 1, 60);
//    SetConsoleWindowInfo(hOut, TRUE, &win);
//
//    CONSOLE_CURSOR_INFO ci;
//    ci.bVisible = FALSE;
//    ci.dwSize = 1;
//    SetConsoleCursorInfo(hOut, &ci);
//}
//
//static void moveHome() {
//    COORD org = { 0, 0 };
//    SetConsoleCursorPosition(hOut, org);
//}
//
//static void clearScreen() {
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    GetConsoleScreenBufferInfo(hOut, &csbi);
//    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
//    DWORD written;
//    FillConsoleOutputCharacterA(hOut, ' ', size, { 0,0 }, &written);
//    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
//    moveHome();
//}
//
//static void restoreCursor() {
//    CONSOLE_CURSOR_INFO ci;
//    ci.bVisible = TRUE;
//    ci.dwSize = 20;
//    SetConsoleCursorInfo(hOut, &ci);
//}
//
//static void initGame(const Configs& cfg, State& st) {
//    st.y = cfg.groundY;
//    st.vy = 0.0;
//    st.onGround = true;
//    st.duck = false;
//
//    st.obs.clear();
//    st.score = 0;
//
//    st.running = true;
//    st.paused = false;
//    st.gapRemain = 0.0;     // 시작 직후 바로 스폰 가능
//}
//
//static void showTitle() {
//    cout << "         ====================================================\n";
//    cout << "         ★★★★★★★  D I N O   R U N N E R ★★★★★★★\n";
//    cout << "         ====================================================\n\n";
//}
//
//static int showMenuAndPick() {
//    clearScreen();
//    showTitle();
//    cout << " \t\t\t 1) 시작\n";
//    cout << " \t\t\t 2) 조작법 보기\n";
//    cout << " \t\t\t 3) 종료\n\n";
//    cout << "번호를 선택하세요: ";
//
//    while (true) {
//        int ch = _getch();
//        if (ch == '1') return 1;
//        if (ch == '2') return 2;
//        if (ch == '3' || ch == 27) return 3;
//    }
//}
//
//static void showHelp() {
//    cout << "\n\n[조작법]\n";
//    cout << " - 점프: Space 또는 ↑ (선인장 넘기)\n";
//    cout << " - 웅크리기(홀드): S 또는 ↓ (새 피하기)\n";
//    cout << " - 일시정지: P\n";
//    cout << " - 종료: Q\n\n";
//    cout << "[룰]\n";
//    cout << " - ★ 코인: 점수 400마다 1코인 획득.\n\n";
//    cout << "아무 키나 누르면 메뉴로 돌아갑니다...";
//    _getch();
//}
//
//static void handleInput(const Configs& cfg, State& st) {
//    if (_kbhit()) {
//        int ch = _getch();
//
//        if (ch == 224 || ch == 0) {
//            int arrow = _getch();
//            if (arrow == 72) { // ↑
//                if (st.onGround) {
//                    st.onGround = false;
//                    st.vy = cfg.jumpV;
//                    st.duck = false;
//                }
//            }
//        }
//        else {
//            ch = tolower(ch);
//            if (ch == ' ') {
//                if (st.onGround) {
//                    st.onGround = false;
//                    st.vy = cfg.jumpV;
//                    st.duck = false;
//                }
//            }
//            else if (ch == 'p') {
//                st.paused = !st.paused;
//            }
//            else if (ch == 'q') {
//                st.running = false;
//            }
//        }
//    }
//
//    // 웅크리기: 누르는 동안만
//    SHORT downHeld = GetAsyncKeyState(VK_DOWN) & 0x8000;
//    SHORT sHeld = GetAsyncKeyState('S') & 0x8000;
//    st.duck = st.onGround && (downHeld || sHeld);
//}
//
//// 간격을 지키며 Cactus 또는 Bird 중 "하나만" 스폰
//static void maybeSpawnObstacle(const Configs& cfg, State& st) {
//    if (st.gapRemain > 0.0) return; // 간격 남았으면 스폰 금지
//
//    bool tryBirdFirst = (rand() % 2) == 0;
//
//    auto spawnCactus = [&]() -> bool {
//        double r = (double)rand() / RAND_MAX;
//        if (r < cfg.cactusSpawnProb) {
//            Obstacle o;
//            o.type = ObType::CACTUS_LOW;
//            o.x = cfg.width - 2;
//            o.h = cfg.cactusHeight;     // 고정 높이
//            st.obs.push_back(o);
//            st.gapRemain = (double)(cfg.minGapTiles + (rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1)));
//            return true;
//        }
//        return false;
//        };
//
//    auto spawnBird = [&]() -> bool {
//        double r = (double)rand() / RAND_MAX;
//        if (r < cfg.birdSpawnProb) {
//            Obstacle o;
//            o.type = ObType::BIRD_HIGH;
//            o.x = (double)cfg.width; // 오른쪽 밖
//            o.h = 1;
//            st.obs.push_back(o);
//            st.gapRemain = (double)(cfg.minGapTiles + (rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1)));
//            return true;
//        }
//        return false;
//        };
//
//    if (tryBirdFirst) {
//        if (!spawnBird()) spawnCactus();
//    }
//    else {
//        if (!spawnCactus()) spawnBird();
//    }
//}
//
//static void tick(const Configs& cfg, State& st) {
//    const double speed = cfg.baseSpeed; // 속도 고정
//
//    // 스폰 (간격 준수)
//    maybeSpawnObstacle(cfg, st);
//
//    // 플레이어 물리
//    st.vy += cfg.gravity;
//    st.y += st.vy;
//
//    if (st.y >= cfg.groundY) {
//        st.y = cfg.groundY;
//        st.vy = 0.0;
//        st.onGround = true;
//    }
//    else {
//        st.onGround = false;
//    }
//    if (!st.onGround) st.duck = false;
//
//    // 장애물 이동
//    for (auto& o : st.obs) o.x -= speed;
//
//    // 화면 밖 정리
//    st.obs.erase(remove_if(st.obs.begin(), st.obs.end(),
//        [&](const Obstacle& o) { return o.x < -4.0; }), st.obs.end());
//
//    // 간격 잔여거리 감소(현재 속도만큼)
//    if (st.gapRemain > 0.0) {
//        st.gapRemain -= speed;
//        if (st.gapRemain < 0.0) st.gapRemain = 0.0;
//    }
//
//    // 충돌
//    int pX = cfg.playerX;
//    int footY = (int)round(st.y);
//    int headY = st.duck ? footY : footY - 1;
//
//    for (const auto& o : st.obs) {
//        int ox = (int)round(o.x);
//
//        if (o.type == ObType::CACTUS_LOW) {
//            int topY = cfg.groundY - (o.h - 1);
//            bool xHit = (ox >= pX - 1 && ox <= pX + 1);
//            bool yHit = !(footY < topY || headY > cfg.groundY);
//            if (xHit && yHit) { st.running = false; break; }
//        }
//        else { // BIRD_HIGH
//            int birdY = cfg.groundY - 1;
//            bool yHit = (headY == birdY || footY == birdY);
//            bool xHit = (ox >= pX - 2 && ox <= pX + 2);
//            if (xHit && yHit && !st.duck) { st.running = false; break; }
//        }
//    }
//
//    // 점수/코인 갱신
//    st.score += 1;
//    int shouldCoins = st.score / cfg.scorePerCoin; // 400점당 1코인
//    if (shouldCoins > coin) coin = shouldCoins;
//}
//
//static void draw(const Configs& cfg, const State& st) {
//    moveHome();
//
//    cout << "[DINO RUNNER]  점수: " << st.score
//        << "   코인: " << coin
//        << "   최고점: " << st.bestScore << "\n";
//    cout << "  (Space/↑ 점프: 선인장, S/↓ 숙이기: 새 회피, P 일시정지, Q 종료)\n";
//
//    vector<string> disp(cfg.height, string(cfg.width, ' '));
//
//    // 지면
//    if (cfg.groundY >= 0 && cfg.groundY < cfg.height) {
//        for (int x = 0; x < cfg.width; ++x) disp[cfg.groundY][x] = '_';
//    }
//
//    // 장애물
//    for (const auto& o : st.obs) {
//        int ox = (int)round(o.x);
//        if (o.type == ObType::CACTUS_LOW) {
//            for (int k = 0; k < o.h; ++k) {
//                int y = cfg.groundY - k;
//                if (0 <= y && y < cfg.height && 0 <= ox && ox < cfg.width)
//                    disp[y][ox] = '|';
//            }
//            if (o.h >= 2) {
//                if ((cfg.groundY - 1) >= 0 && ox - 1 >= 0 && ox - 1 < cfg.width)
//                    disp[cfg.groundY - 1][ox - 1] = '-';
//            }
//            if (o.h >= 3) {
//                if ((cfg.groundY - 2) >= 0 && ox + 1 >= 0 && ox + 1 < cfg.width)
//                    disp[cfg.groundY - 2][ox + 1] = '-';
//            }
//        }
//        else {
//            int y = cfg.groundY - 1;
//            if (0 <= y && y < cfg.height) {
//                if (ox - 1 >= 0 && ox - 1 < cfg.width) disp[y][ox - 1] = '<';
//                if (ox >= 0 && ox < cfg.width) disp[y][ox] = '^';
//                if (ox + 1 >= 0 && ox + 1 < cfg.width) disp[y][ox + 1] = '>';
//            }
//        }
//    }
//
//    // 플레이어
//    int pX = cfg.playerX;
//    int footY = (int)round(st.y);
//    if (st.duck) {
//        if (0 <= footY && footY < cfg.height) {
//            if (pX - 1 >= 0) disp[footY][pX - 1] = '[';
//            if (0 <= pX && pX < cfg.width) disp[footY][pX] = 'd';
//            if (pX + 1 < cfg.width) disp[footY][pX + 1] = ']';
//        }
//    }
//    else {
//        int headY = footY - 1;
//        if (0 <= headY && headY < cfg.height && 0 <= pX && pX < cfg.width)
//            disp[headY][pX] = 'O';
//        if (0 <= footY && footY < cfg.height) {
//            if (0 <= pX && pX < cfg.width) disp[footY][pX] = '#';
//            if (pX - 1 >= 0) disp[footY][pX - 1] = '/';
//            if (pX + 1 < cfg.width) disp[footY][pX + 1] = '\\';
//        }
//    }
//
//    // 외곽
//    cout << '+' << string(cfg.width, '-') << "+\n";
//    for (int y = 0; y < cfg.height; ++y)
//        cout << '|' << disp[y] << "|\n";
//    cout << '+' << string(cfg.width, '-') << "+\n\n";
//
//    if (st.paused) cout << "=== 일시정지 (P: 해제) ===\n\n";
//}
//
//static void startGame(const Configs& cfg, State& st) {
//    initGame(cfg, st);
//
//    while (st.running) {
//        handleInput(cfg, st);
//        if (!st.paused) tick(cfg, st);
//        draw(cfg, st);
//        Sleep(max(1, cfg.baseDelay));
//    }
//
//    st.bestScore = max(st.bestScore, st.score);
//
//    moveHome();
//    cout << "\n\n=======================================================\n";
//    cout << " ★★★★★★★★   G A M E  O V E R   ★★★★★★★★\n";
//    cout << "=======================================================\n\n";
//    cout << "   점수: " << st.score
//        << "   코인: " << coin
//        << "   최고점수: " << st.bestScore << "\n\n";
//    cout << "   (코인은 이번 판 기준: 400점 당 1개)\n\n";
//
//    return;
//}
//
//void run() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//    srand((unsigned)time(nullptr));
//
//    Configs cfg;
//    State  st;
//
//    // 안전 체크(지면 보장)
//    if (cfg.groundY < 2) cfg.groundY = 2;
//    if (cfg.groundY > cfg.height - 2) cfg.groundY = cfg.height - 2;
//
//    setupConsole(cfg.width, cfg.height);
//    while (1) {
//        int pick = showMenuAndPick();
//        if (pick == 1) {
//            startGame(cfg, st);
//            restoreCursor();
//            clearScreen();
//            cout << "게임이 종료되었습니다. 수고하셨습니다!\n";
//            return;
//        }
//        else if (pick == 2) {
//            showHelp();
//            restoreCursor();
//            clearScreen();
//        }
//        else {
//            restoreCursor();
//            clearScreen();
//            cout << "게임을 종료합니다. 수고하셨습니다!\n";
//            return;
//        }
//    }
//}
//
//
//
//int main()
//{
//
//
//    DungeonRun();
//
//    return 0;
//}