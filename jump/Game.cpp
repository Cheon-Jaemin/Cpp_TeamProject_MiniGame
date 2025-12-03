#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void gotoxy(int row, int col)
{
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
    maze.print();                    // 맵 전체 출력
    gotoxy(player.getY(), player.getX());
    cout << "P";                     // 플레이어 표시

    int mazeRows = maze.getRows();
    int mazeCols = maze.getCols();

    while (true)
    {
        int timeRow = mazeRows + 1;
        int timeCol = mazeCols / 2 - 10;
        gotoxy(timeRow, timeCol);
        cout << "남은시간 : " << timeLimit - (int)difftime(time(nullptr), startTime) << "초   ";

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
            }

            if (!maze.isWall(nextX, nextY))
            {
                // 이전 위치 지우기
                gotoxy(player.getX(), player.getY());
                cout << " ";

                // 이동
                player.move(c);

                // 새 위치 표시
                gotoxy(player.getX(), player.getY());
                cout << "P";
            }
        }

        if (maze.isExit(player.getX(), player.getY()))
        {
            gotoxy(mazeRows + 3, mazeCols / 2 - 10);
            cout << "★ 탈출 성공!! ★" << endl<<"코인 3개를 드립니다!!"<<endl;
            coin += 3;
            cout << "현재 코인: " << coin << endl;
            Sleep(3000);
            break;
        }

        if (isTimeOver())
        {
            gotoxy(mazeRows + 3, mazeCols / 2 - 10);
            cout << "제한 시간 초과! 탈출 실패..." << endl;
            break;
        }
        Sleep(30);
    }
}