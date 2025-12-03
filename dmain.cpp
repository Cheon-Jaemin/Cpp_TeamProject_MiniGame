#include <iostream>
#include <conio.h>   // _getch()
#include <windows.h> // gotoxy용
#include "Dungeon.h"
Map dungeon(120, 30);

int DungeonRun()
{
    try {

        dungeon.generateLinearDungeon(5);

        // 커서 숨기기
        dungeon.hideCursor();

        // 플레이어 시작 위치: 첫 번째 방 중앙
        auto rooms = dungeon.getRooms();
        int px = rooms[0].area.x + rooms[0].area.w / 2;
        int py = rooms[0].area.y + rooms[0].area.h / 2;


        vector<Monster> monsters;

        for (int i = 0; i < rooms.size(); i++) {
            int cx = rooms[i].area.x + rooms[i].area.w / 2;
            int cy = rooms[i].area.y + rooms[i].area.h / 2;
            if (rooms[i].kind == RoomKind::MiniGame) {
                monsters.push_back(Monster(cx, cy, (int)i, (int)i + 1, "●"));
            }
            else if (rooms[i].kind == RoomKind::Boss) {
                monsters.push_back(Monster(cx, cy, (int)i, 999, "★"));
            }
            dungeon.at(cx, cy).walkable = false;
        }

        // 맵 한 번만 출력
        dungeon.drawMap(dungeon);

        for (int i = 0; i < monsters.size(); i++)
        {
            monsters[i].drawMonster();
        }

        // 플레이어 초기 위치 표시
        dungeon.gotoxy(px, py);
        cout << '@';

        while (true) {
            char key = _getch();
            if (key == 27 || key == 'q' || key == 'Q') break;

            int nx = px, ny = py;
            if (key == 'w') ny--;
            else if (key == 's') ny++;
            else if (key == 'a') nx--;
            else if (key == 'd') nx++;
            if (key == 32) {
                for (int i = 0; i < monsters.size(); i++) {
                    if (monsters[i].isNearPlayer(px, py))
                        monsters[i].interact(dungeon);
                }
            }

            if (dungeon.in(nx, ny) && dungeon.at(nx, ny).walkable) {
                // 이전 위치 복원
                dungeon.gotoxy(px, py);
                cout << dungeon.at(px, py).glyph;

                // 새 위치에 플레이어 표시
                px = nx;
                py = ny;
                dungeon.gotoxy(px, py);
                cout << '@';
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
int main() {
   
}
