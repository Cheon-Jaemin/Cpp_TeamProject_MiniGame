#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Maze::Maze(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    grid.resize(rows, string(cols, '#'));
    exitX = rows - 1;
    exitY = cols - 2;
}

void Maze::create()
{
    do {
        // 1. 랜덤으로 벽/길 생성
        for (int i = 1; i < rows - 1; i++)
        {
            for (int j = 1; j < cols - 1; j++)
            {
                if (rand() % 3 != 0)
                    grid[i][j] = ' ';
                else
                    grid[i][j] = '#';
            }
        }

        // 시작점과 출구
        grid[1][1] = ' ';
        grid[exitX][exitY] = 'E';

    } while (!hasPath());  // 출구까지 경로가 없으면 다시 생성
}

void Maze::print() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

bool Maze::isWall(int x, int y) const
{
    return grid[x][y] == '#';
}

bool Maze::isExit(int x, int y) const
{
    return (x == exitX && y == exitY);
}

bool Maze::hasPath()
{
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    return dfs(1, 1, visited);
}

bool Maze::dfs(int x, int y, vector<vector<bool>>& visited)
{
    if (x < 0 || x >= rows || y < 0 || y >= cols) return false;
    if (grid[x][y] == '#' || visited[x][y]) return false;
    if (x == exitX && y == exitY) return true;

    visited[x][y] = true;

    if (dfs(x + 1, y, visited)) return true;
    if (dfs(x - 1, y, visited)) return true;
    if (dfs(x, y + 1, visited)) return true;
    if (dfs(x, y - 1, visited)) return true;

    return false;
}