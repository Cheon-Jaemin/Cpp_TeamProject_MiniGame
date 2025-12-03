#pragma once
#include <vector>
#include <string>
using namespace std;

extern int coin;

class Maze
{
private:
    int rows, cols;
    vector<string> grid;
    int exitX, exitY;

public:
    Maze(int rows, int cols);
    void create();
    void print() const;            // ¸Ê¸¸ Ãâ·Â
    bool isWall(int x, int y) const;
    bool isExit(int x, int y) const;
    int getRows() const { return rows; };
    int getCols() const { return cols; };
    bool hasPath();
    bool dfs(int x, int y, vector<vector<bool>>& visited);
};