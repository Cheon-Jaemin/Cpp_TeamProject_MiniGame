#pragma once
#include "MemoryBoard.h"

extern int coin;

class MemoryGame {
private:
	MemoryBoard board;
	int turnCount;

public:
	MemoryGame();
	void run();
	
};