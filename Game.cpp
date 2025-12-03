#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void gotoxys(int row, int col)
{
    if (row < 0) row = 0;
    if (col < 0) col = 0;

    COORD pos = { (SHORT)col, (SHORT)row }; // X = column, Y = row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Game::Game(int rows, int cols, int timeLimit) : maze(rows, cols), player(1, 1)
{
    this->timeLimit = timeLimit;
}

bool Game::isTimeOver() const
{
    return difftime(time(nullptr), startTime) >= timeLimit;
}

void Game::start()
{
    maze.create();
    startTime = time(nullptr);

    system("cls");
    maze.print();

    // 안전하게 플레이어 표시
    int px = player.getX();
    int py = player.getY();
    if (px >= 0 && px < maze.getRows() && py >= 0 && py < maze.getCols())
    {
        gotoxys(px, py);
        cout << "P";
    }

    int mazeRows = maze.getRows();
    int mazeCols = maze.getCols();

    while (true)
    {
        // 남은 시간 표시
        int timeRow = mazeRows + 1;
        int timeCol = mazeCols / 2 - 10;
        gotoxys(timeRow, timeCol);
        int remaining = timeLimit - (int)difftime(time(nullptr), startTime);
        if (remaining < 0) remaining = 0;
        cout << "남은시간 : " << remaining << "초   ";

        if (_kbhit())
        {
            char c = _getch();
            int nextX = player.getX();
            int nextY = player.getY();

            switch (c)
            {
            case 'w': nextX--; break;
            case 's': nextX++; break;
            case 'a': nextY--; break;
            case 'd': nextY++; break;
            default: continue; // w,s,a,d 외 입력 무시
            }

            // 범위 체크 후 이동
            if (nextX >= 0 && nextX < mazeRows && nextY >= 0 && nextY < mazeCols)
            {
                if (!maze.isWall(nextX, nextY))
                {
                    // 이전 위치 지우기
                    gotoxys(player.getX(), player.getY());
                    cout << " ";

                    // 이동
                    player.move(c);

                    // 새 위치 표시
                    gotoxys(player.getX(), player.getY());
                    cout << "P";
                }
            }
        }

        // 탈출 체크
        px = player.getX();
        py = player.getY();

        if (px >= 0 && px < mazeRows && py >= 0 && py < mazeCols)
        {
            if (maze.isExit(px, py))
            {

                gotoxys(mazeRows + 3, mazeCols / 2 - 10);
                cout <<endl<< "★ 탈출 성공!! ★" << endl << "코인 3개를 드립니다!!" << endl;
                coin += 3;
                cout << "현재 코인: " << coin << endl;
                Sleep(3000);
                break;;
            }
        }


        // 시간 초과 체크
        if (isTimeOver())
        {
            gotoxys(mazeRows + 3, mazeCols / 2 - 10);
            cout << "제한 시간 초과! 탈출 실패..." << endl;
            break;
        }

        Sleep(30);
    }
}