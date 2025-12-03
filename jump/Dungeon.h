#pragma once
#include <vector>
#include <stdexcept>
using namespace std;
//extern Map dungeon;

// ----- 타일 -----
struct Tile {
    char glyph;        // '#': 벽, '.': 바닥
    bool walkable;

    Tile() : glyph('#'), walkable(false) {}
};


// ----- 직사각형(방 영역) -----
struct Rect { int x, y, w, h; };

enum class RoomKind {
    Start,
    MiniGame,
    Boss
};

struct Room {
    Rect area;
    RoomKind kind;
    int miniGameId;

    Room() : area{ 0,0,0,0 }, kind(RoomKind::Start), miniGameId(-1) {}
    Room(Rect r, RoomKind k, int id = -1) : area(r), kind(k), miniGameId(id) {}
};

// ----- 맵 -----
class Map {
public:
    Map(int w, int h);

    inline int width()  const { return W; }
    inline int height() const { return H; }

    bool in(int x, int y) const;
    Tile& at(int x, int y);
    const Tile& at(int x, int y) const;

    void generateLinearDungeon(int numMiniRooms = 5);
    const vector<Room>& getRooms() const { return rooms; }

    void drawMap(const Map& m);

    static void gotoxy(int x, int y);
    static void hideCursor();

private:
    int W, H;
    vector<Tile> tiles;
    vector<Room> rooms;

    void fillWall();
    void carveRoom(const Rect& r);
    void carveHCorridor(int x1, int x2, int y);
    void carveVCorridor(int y1, int y2, int x);

    inline int idx(int x, int y) const { return y * W + x; }
};

class Monster {
public:
    int X, Y;
    int roomId;
    string symbol;
    int minigameId;
    bool isActive;


public:
    Monster(int x, int y, int roomId, int minigameId, string symbol = "○")
        : X(x), Y(y), roomId(roomId), minigameId(minigameId), symbol(symbol), isActive(true) {
    }

    void drawMonster();
    bool isNearPlayer(int px, int py);
    void interact(Map& dungeon);
    void startMiniGame();
    int getX() const { return X; }
    int getY() const { return Y; }
};