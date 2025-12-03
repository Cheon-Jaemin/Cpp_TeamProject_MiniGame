#pragma once
#include "Deck.h"
#include "bPlayer.h"

extern int coin;

class Blackjack
{
	Deck deck;
	bPlayer COM;
	bPlayer player;

public:
	
	bool playGame();
};