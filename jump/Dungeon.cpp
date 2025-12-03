#include "Game.h"
#include "Blackjack.h"
#include "GameManager.h"
#include <stdio.h>
#include "PoopGame.h"
#include "JumpGame.h"
#include "Shop.h"
#include "BossBattle.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <cmath>
#include "Dungeon.h"
#include"MemoryGame.h"

#define NOMINMAX
#include <conio.h>
#include <windows.h>


using namespace std;
int coin = 0;
int playerAttack = 1;
int playerHp = 100;
int petCount = 0;
int nowHp;
int bossHP = 1000;
int bossAttack = 20;


class DinoRunner;




struct Configs {
    int   width = 70;
    int   height = 26;
    int   groundY = 22;             // 0(top) 기준
    int   baseDelay = 18;           // 프레임(ms), Sleep에 그대로 사용
    double baseSpeed = 1.1;         // 장애물 속도(칸/프레임) — 일정

    // 물리/플레이어
    double gravity = 0.9;
    double jumpV = -4.5;            // 점프 초기속도(위로 음수)
    int   playerX = 10;

    // 스폰 확률(프레임당, 0~1) - 고정
    double cactusSpawnProb = 0.10;  // 선인장
    double birdSpawnProb = 0.06;    // 새

    int  birdWidth = 3;             // "<^>" 폭 3

    // 장애물 간 최소/최대 간격(타일)
    int    minGapTiles = 22;
    int    maxGapTiles = 34;

    // 선인장 고정 높이 (1~3 권장)
    int    cactusHeight = 2;

    // 코인 규칙
    int    scorePerCoin = 400;      // ★ 400점 당 코인 1개
};

// 장애물 타입
enum class ObType { CACTUS_LOW, BIRD_HIGH };

struct Obstacle {
    ObType type;
    double x;  // 좌측 기준 위치(소수 이동)
    int    h;  // 높이(선인장: 고정, 새: 1)
};

struct State {
    // 플레이어
    double y = 0.0;
    double vy = 0.0;
    bool   onGround = true;
    bool   duck = false;

    // 월드
    vector<Obstacle> obs;

    // 진행
    int    score = 0;
    int    bestScore = 0;
    bool   running = true;
    bool   paused = false;

    // 다음 스폰까지 남은 거리(타일)
    double gapRemain = 0.0;
};

static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

static void setupConsole(int width, int height) {
    COORD bufSize;
    bufSize.X = width + 4;
    bufSize.Y = height + 8;
    SetConsoleScreenBufferSize(hOut, bufSize);

    SMALL_RECT win;
    win.Left = 0; win.Top = 0;
    win.Right = bufSize.X - 1;
    win.Bottom = std::min<SHORT>(bufSize.Y - 1, 60);
    SetConsoleWindowInfo(hOut, TRUE, &win);

    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = FALSE;
    ci.dwSize = 1;
    SetConsoleCursorInfo(hOut, &ci);
}

static void moveHome() {
    COORD org = { 0, 0 };
    SetConsoleCursorPosition(hOut, org);
}

static void clearScreen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacterA(hOut, ' ', size, { 0,0 }, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, size, { 0,0 }, &written);
    moveHome();
}

static void restoreCursor() {
    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = TRUE;
    ci.dwSize = 20;
    SetConsoleCursorInfo(hOut, &ci);
}

static void initGame(const Configs& cfg, State& st) {
    st.y = cfg.groundY;
    st.vy = 0.0;
    st.onGround = true;
    st.duck = false;

    st.obs.clear();
    st.score = 0;

    st.running = true;
    st.paused = false;
    st.gapRemain = 0.0;     // 시작 직후 바로 스폰 가능
}

static void showTitle() {
    cout << "         ====================================================\n";
    cout << "         ★★★★★★★  D I N O   R U N N E R ★★★★★★★\n";
    cout << "         ====================================================\n\n";
}

static int showMenuAndPick() {
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

static void showHelp() {
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

static void handleInput(const Configs& cfg, State& st) {
    if (_kbhit()) {
        int ch = _getch();

        if (ch == 224 || ch == 0) {
            int arrow = _getch();
            if (arrow == 72) { // ↑
                if (st.onGround) {
                    st.onGround = false;
                    st.vy = cfg.jumpV;
                    st.duck = false;
                }
            }
        }
        else {
            ch = tolower(ch);
            if (ch == ' ') {
                if (st.onGround) {
                    st.onGround = false;
                    st.vy = cfg.jumpV;
                    st.duck = false;
                }
            }
            else if (ch == 'p') {
                st.paused = !st.paused;
            }
            else if (ch == 'q') {
                st.running = false;
            }
        }
    }

    // 웅크리기: 누르는 동안만
    SHORT downHeld = GetAsyncKeyState(VK_DOWN) & 0x8000;
    SHORT sHeld = GetAsyncKeyState('S') & 0x8000;
    st.duck = st.onGround && (downHeld || sHeld);
}

// 간격을 지키며 Cactus 또는 Bird 중 "하나만" 스폰
static void maybeSpawnObstacle(const Configs& cfg, State& st) {
    if (st.gapRemain > 0.0) return; // 간격 남았으면 스폰 금지

    bool tryBirdFirst = (rand() % 2) == 0;

    auto spawnCactus = [&]() -> bool {
        double r = (double)rand() / RAND_MAX;
        if (r < cfg.cactusSpawnProb) {
            Obstacle o;
            o.type = ObType::CACTUS_LOW;
            o.x = cfg.width - 2;
            o.h = cfg.cactusHeight;     // 고정 높이
            st.obs.push_back(o);
            st.gapRemain = (double)(cfg.minGapTiles + (rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1)));
            return true;
        }
        return false;
        };

    auto spawnBird = [&]() -> bool {
        double r = (double)rand() / RAND_MAX;
        if (r < cfg.birdSpawnProb) {
            Obstacle o;
            o.type = ObType::BIRD_HIGH;
            o.x = (double)cfg.width; // 오른쪽 밖
            o.h = 1;
            st.obs.push_back(o);
            st.gapRemain = (double)(cfg.minGapTiles + (rand() % (cfg.maxGapTiles - cfg.minGapTiles + 1)));
            return true;
        }
        return false;
        };

    if (tryBirdFirst) {
        if (!spawnBird()) spawnCactus();
    }
    else {
        if (!spawnCactus()) spawnBird();
    }
}

static void tick(const Configs& cfg, State& st) {
    const double speed = cfg.baseSpeed; // 속도 고정

    // 스폰 (간격 준수)
    maybeSpawnObstacle(cfg, st);

    // 플레이어 물리
    st.vy += cfg.gravity;
    st.y += st.vy;

    if (st.y >= cfg.groundY) {
        st.y = cfg.groundY;
        st.vy = 0.0;
        st.onGround = true;
    }
    else {
        st.onGround = false;
    }
    if (!st.onGround) st.duck = false;

    // 장애물 이동
    for (auto& o : st.obs) o.x -= speed;

    // 화면 밖 정리
    st.obs.erase(remove_if(st.obs.begin(), st.obs.end(),
        [&](const Obstacle& o) { return o.x < -4.0; }), st.obs.end());

    // 간격 잔여거리 감소(현재 속도만큼)
    if (st.gapRemain > 0.0) {
        st.gapRemain -= speed;
        if (st.gapRemain < 0.0) st.gapRemain = 0.0;
    }

    // 충돌
    int pX = cfg.playerX;
    int footY = (int)round(st.y);
    int headY = st.duck ? footY : footY - 1;

    for (const auto& o : st.obs) {
        int ox = (int)round(o.x);

        if (o.type == ObType::CACTUS_LOW) {
            int topY = cfg.groundY - (o.h - 1);
            bool xHit = (ox >= pX - 1 && ox <= pX + 1);
            bool yHit = !(footY < topY || headY > cfg.groundY);
            if (xHit && yHit) { st.running = false; break; }
        }
        else { // BIRD_HIGH
            int birdY = cfg.groundY - 1;
            bool yHit = (headY == birdY || footY == birdY);
            bool xHit = (ox >= pX - 2 && ox <= pX + 2);
            if (xHit && yHit && !st.duck) { st.running = false; break; }
        }
    }

    // 점수/코인 갱신
    st.score += 1;
    int shouldCoins = st.score / cfg.scorePerCoin; // 400점당 1코인
    if (shouldCoins > coin) coin = shouldCoins;
}

static void draw(const Configs& cfg, const State& st) {
    moveHome();

    cout << "[DINO RUNNER]  점수: " << st.score
        << "   코인: " << coin
        << "   최고점: " << st.bestScore << "\n";
    cout << "  (Space/↑ 점프: 선인장, S/↓ 숙이기: 새 회피, P 일시정지, Q 종료)\n";

    vector<string> disp(cfg.height, string(cfg.width, ' '));

    // 지면
    if (cfg.groundY >= 0 && cfg.groundY < cfg.height) {
        for (int x = 0; x < cfg.width; ++x) disp[cfg.groundY][x] = '_';
    }

    // 장애물
    for (const auto& o : st.obs) {
        int ox = (int)round(o.x);
        if (o.type == ObType::CACTUS_LOW) {
            for (int k = 0; k < o.h; ++k) {
                int y = cfg.groundY - k;
                if (0 <= y && y < cfg.height && 0 <= ox && ox < cfg.width)
                    disp[y][ox] = '|';
            }
            if (o.h >= 2) {
                if ((cfg.groundY - 1) >= 0 && ox - 1 >= 0 && ox - 1 < cfg.width)
                    disp[cfg.groundY - 1][ox - 1] = '-';
            }
            if (o.h >= 3) {
                if ((cfg.groundY - 2) >= 0 && ox + 1 >= 0 && ox + 1 < cfg.width)
                    disp[cfg.groundY - 2][ox + 1] = '-';
            }
        }
        else {
            int y = cfg.groundY - 1;
            if (0 <= y && y < cfg.height) {
                if (ox - 1 >= 0 && ox - 1 < cfg.width) disp[y][ox - 1] = '<';
                if (ox >= 0 && ox < cfg.width) disp[y][ox] = '^';
                if (ox + 1 >= 0 && ox + 1 < cfg.width) disp[y][ox + 1] = '>';
            }
        }
    }

    // 플레이어
    int pX = cfg.playerX;
    int footY = (int)round(st.y);
    if (st.duck) {
        if (0 <= footY && footY < cfg.height) {
            if (pX - 1 >= 0) disp[footY][pX - 1] = '[';
            if (0 <= pX && pX < cfg.width) disp[footY][pX] = 'd';
            if (pX + 1 < cfg.width) disp[footY][pX + 1] = ']';
        }
    }
    else {
        int headY = footY - 1;
        if (0 <= headY && headY < cfg.height && 0 <= pX && pX < cfg.width)
            disp[headY][pX] = 'O';
        if (0 <= footY && footY < cfg.height) {
            if (0 <= pX && pX < cfg.width) disp[footY][pX] = '#';
            if (pX - 1 >= 0) disp[footY][pX - 1] = '/';
            if (pX + 1 < cfg.width) disp[footY][pX + 1] = '\\';
        }
    }

    // 외곽
    cout << '+' << string(cfg.width, '-') << "+\n";
    for (int y = 0; y < cfg.height; ++y)
        cout << '|' << disp[y] << "|\n";
    cout << '+' << string(cfg.width, '-') << "+\n\n";

    if (st.paused) cout << "=== 일시정지 (P: 해제) ===\n\n";
}

static void startGame(const Configs& cfg, State& st) {
    initGame(cfg, st);

    while (st.running) {
        handleInput(cfg, st);
        if (!st.paused) tick(cfg, st);
        draw(cfg, st);
        Sleep(max(1, cfg.baseDelay));
    }

    st.bestScore = max(st.bestScore, st.score);

    moveHome();
    cout << "\n\n=======================================================\n";
    cout << " ★★★★★★★★   G A M E  O V E R   ★★★★★★★★\n";
    cout << "=======================================================\n\n";
    cout << "   점수: " << st.score
        << "   코인: " << coin
        << "   최고점수: " << st.bestScore << "\n\n";
    cout << "   (코인은 이번 판 기준: 400점 당 1개)\n\n";

    Sleep(3000);
    clearScreen();

    return;
}

void run() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand((unsigned)time(nullptr));

    Configs cfg;
    State  st;

    // 안전 체크(지면 보장)
    if (cfg.groundY < 2) cfg.groundY = 2;
    if (cfg.groundY > cfg.height - 2) cfg.groundY = cfg.height - 2;

    setupConsole(cfg.width, cfg.height);
    while (1) {
        int pick = showMenuAndPick();
        if (pick == 1) {
            startGame(cfg, st);
            restoreCursor();
            clearScreen();
            cout << "게임이 종료되었습니다. 수고하셨습니다!\n";
            return;
        }
        else if (pick == 2) {
            showHelp();
            restoreCursor();
            clearScreen();
        }
        else {
            restoreCursor();
            clearScreen();
            cout << "게임을 종료합니다. 수고하셨습니다!\n";
            return;
        }
    }
}

void Map::gotoxy(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return; // 실패하면 그냥 리턴

    SHORT maxX = csbi.dwSize.X - 1;
    SHORT maxY = csbi.dwSize.Y - 1;

    // 좌표 제한
    if (x < 0) x = 0;
    if (x > maxX) x = maxX;
    if (y < 0) y = 0;
    if (y > maxY) y = maxY;

    COORD coord = { x, y };
    SetConsoleCursorPosition(hOut, coord);
}


void Map:: hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = false;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

Map::Map(int w, int h) : W(w), H(h), tiles(w* h) {
    if (w <= 0 || h <= 0) throw invalid_argument("Map size must be positive");
}

bool Map::in(int x, int y) const {
    return 0 <= x && x < W && 0 <= y && y < H;
}

Tile& Map::at(int x, int y) {
    return tiles[idx(x, y)];
}

const Tile& Map::at(int x, int y) const {
    return tiles[idx(x, y)];
}

void Map::fillWall() {
    for (auto& c : tiles) {
        c.glyph = ' ';          // 기본은 빈 공간
        c.walkable = false;
        //c.transparent = true;   // 공백은 시야 통과 가능하게 할 수도 있음
        //c.discovered = false;
        //c.visible = false;
    }
}

void Map::carveRoom(const Rect& r) {
    for (int y = r.y; y < r.y + r.h; ++y) {
        for (int x = r.x; x < r.x + r.w; ++x) {
            if (!in(x, y)) continue;
            Tile& c = at(x, y);

            // 테두리 → 벽 '#'
            if (y == r.y || y == r.y + r.h - 1 ||
                x == r.x || x == r.x + r.w - 1) {
                c.glyph = '#';
                c.walkable = false;
                //c.transparent = false;
            }
            // 내부 → 바닥 '.'
            else {
                c.glyph = '.';
                c.walkable = true;
                //c.transparent = true;
            }
        }
    }

}

void Map::carveHCorridor(int x1, int x2, int y) {
    if (x1 > x2) std::swap(x1, x2);
    for (int x = x1; x <= x2; ++x) {
        if (!in(x, y)) continue;

        // 가운데 길
        Tile& c = at(x, y);
        c.glyph = '.';
        c.walkable = true;
        //c.transparent = true;

        // 위쪽 벽
        if (in(x, y - 1)) {
            Tile& up = at(x, y - 1);
            if (up.glyph == ' ') { // 아직 빈 공간이면 벽으로 설정
                up.glyph = '#';
                up.walkable = false;
                //up.transparent = false;
            }
        }

        // 아래쪽 벽
        if (in(x, y + 1)) {
            Tile& down = at(x, y + 1);
            if (down.glyph == ' ') {
                down.glyph = '#';
                down.walkable = false;
                //down.transparent = false;
            }
        }
    }
}

void Map::generateLinearDungeon(int numMiniRooms) {
    if (numMiniRooms < 0) throw invalid_argument("numMiniRooms must be >= 0");

    fillWall();
    rooms.clear();

    // 레이아웃 파라미터
    const int roomW = 12;
    const int roomH = 9;
    const int spacing = 4;      // 방 사이 복도/공백
    const int bossW = 18;
    const int bossH = 11;

    // 맵 폭이 충분한지 체크 (대략적인 필요 폭 계산)
    // 시작방 1 + 미니방 N + 보스방 1, 사이사이 spacing
    const int neededWidth =
        2 + // 여유
        roomW * (1 + numMiniRooms) +
        bossW +
        spacing * (1 + numMiniRooms + 1);
    if (W < neededWidth || H < max(roomH, bossH) + 4) {
        throw std::runtime_error("Map is too small for the requested linear dungeon.");
    }

    // 기준 Y (수평 일자 배치)
    const int startX = 3;
    const int startY = H / 2 - roomH / 2;

    // 시작방
    Rect start = { startX, startY, roomW, roomH };
    carveRoom(start);
    rooms.emplace_back(start, RoomKind::Start, -1);

    int prevCX = start.x + start.w / 2;
    int prevCY = start.y + start.h / 2;

    // 미니게임 방들
    for (int i = 0; i < numMiniRooms; ++i) {
        int x = startX + (roomW + spacing) * (i + 1);
        Rect r{ x, startY, roomW, roomH };
        carveRoom(r);
        rooms.emplace_back(r, RoomKind::MiniGame, i + 1);

        // 이전 방 중심과 현재 방 중심을 복도로 연결
        int curCX = r.x + r.w / 2;
        int curCY = r.y + r.h / 2;
        carveHCorridor(prevCX, curCX, prevCY);

        // 문(+) : 현재 방의 왼쪽 쪽 중앙
        int doorX = r.x - spacing / 2;
        int doorY = r.y + r.h / 2;
        if (in(doorX, doorY)) {
            Tile& d = at(doorX, doorY);
            d.glyph = '+';
            d.walkable = true;
            //d.transparent = true;
        }

        prevCX = curCX;
        prevCY = curCY;
    }

    // 보스방
    const int bossX = startX + (roomW + spacing) * (numMiniRooms + 1);
    const int bossY = H / 2 - bossH / 2;
    Rect boss{ bossX, bossY, bossW, bossH };
    carveRoom(boss);
    rooms.push_back(Room(boss, RoomKind::Boss, -1));

    // 마지막 미니게임 방 중심에서 보스방 중심까지 복도
    int bossCX = boss.x + boss.w / 2;
    int bossCY = boss.y + boss.h / 2;
    carveHCorridor(prevCX, bossCX, prevCY);

    // 보스방 입구 문(+) : 보스방 왼쪽
    int bDoorX = boss.x - spacing / 2;
    int bDoorY = boss.y + boss.h / 2;
    if (in(bDoorX, bDoorY)) {
        Tile& d = at(bDoorX, bDoorY);
        d.glyph = '+';
        d.walkable = true;
        //d.transparent = true;
    }
}

// 맵 전체 출력 (처음 1회만)
 void Map::drawMap(const Map& m) 
 {
    gotoxy(0, 0);
    for (int y = 0; y < m.height(); ++y) {
        for (int x = 0; x < m.width(); ++x) {
            cout << m.at(x, y).glyph;
        }
        cout << '\n';
    }
    cout << "W,A,S,D 이동, Q or ESC 종료\n";
}

 // 몬스터 기능
 void Monster::drawMonster() 
 {
     Map::gotoxy(X, Y);
     cout << symbol;

 }

 string Title;
 // 플레이어 접근 여부 확인
 bool Monster::isNearPlayer(int px, int py) {
     return (abs(px - X) + abs(py - Y) == 1);
 }

 // 몬스터와 플레이어 상호작용
 void Monster::interact(Map& dungeon) {

     if (isActive) {
         // 1. 맵 화면 백업
         vector<string> backupMap;
         for (int y = 0; y < dungeon.height(); ++y) {
             string line;
             for (int x = 0; x < dungeon.width(); ++x) {
                 if (dungeon.in(x, y))
                     line += dungeon.at(x, y).glyph;
                 else
                     line += '#'; // 안전 처리
             }
             backupMap.push_back(line);
         }

         // 2. 미니게임 안내 출력
         Map::gotoxy(0, dungeon.height());
         cout << Title<<" ~~~ 미니게임 시작!\n";
         Sleep(1000);

         // 3. 미니게임 실행
         startMiniGame();

         // 4. 화면 복원
         Map::gotoxy(0, 0);
         dungeon.drawMap(dungeon);   // 👉 백업 대신 맵 다시 그림
    
		 

         // 5. 몬스터 비활성화 처리
         isActive = false;
         if (dungeon.in(X, Y)) {
             dungeon.at(X, Y).walkable = true;
             Map::gotoxy(X, Y);
             cout << dungeon.at(X, Y).glyph;
         }
     }
 }

 //순발력 게임

#include <chrono>   // 시간 측정 관련 라이브러리
#include <thread>   // 스레드(병렬 처리)기능, 일지정지 기능



 using namespace std::chrono;

 void sun() {
     srand((unsigned)(time(0))); // 난수 발생기 초기화

     cout << "=== 반응 속도 측정 게임 ===\n";
     cout << "준비되면 Enter 키를 누르세요...";
     cin.get();  // Enter 입력 대기

     int waitTime = rand() % 5 + 1;
     cout << "준비...\n";
     this_thread::sleep_for(seconds(waitTime));

     cout << "\n지금! 아무 키나 누르세요!\n";

     // "지금!" 출력되기 전에 눌린 키 무시
     while (_kbhit())
         _getch();

     auto start = steady_clock::now();
     _getch();
     auto end = steady_clock::now();

     auto userTime = duration_cast<milliseconds>(end - start).count();
     cout << "\n당신의 반응 속도는 " << userTime << " ms 입니다!\n";

     if (userTime < 200)
         cout << "반응 속도 최고예요!";
     else if (userTime < 400)
         cout << "꽤 빠르네요!";
     else

         cout << "조금 느려요! 다시 도전해보세요.";

     // 종료 전에 입력 대기
     cout << "\n\nEnter키를 누르면 종료됩니다...";

     cin.get();
 }



 void Monster::startMiniGame() {

     switch (minigameId) {
     case 2: {
		 Title = "재민이의 똥피하기 게임";
         PoopGame poopGame;
         poopGame.run();
         break;
     }
         
     case 3: {
		 Title = "재민왕자의 공룡 달리기 게임";
         run(); //공룡
         break;
     }
         
         
     case 4: {
		 Title = "미진공주 구하기 기억력 게임";
         MemoryGame game;
         game.run();
         break;
     }
       
     case 5: {
		 Title = "재왕의 점프 게임";
		 cout << " 점프게임 시작전 손풀기용 게임입니다. 준비되셨으면 엔터키를 눌러주세요!";
         system("cls");
		 cin.ignore(); // 엔터키 대기
		 sun(); //순발력 게임
		 system("cls");
         JumpGame jumpGame;
         jumpGame.start();
         break;
     }
        
     case 6: {
		 Title = "상점";
         Shop shop;
         shop.enterShop();
         break;
     }
        
     case 999: {
		 Title = "보스전";
         BossBattle boss;
         boss.startBattle();
         break;
     }
        
     }
 }