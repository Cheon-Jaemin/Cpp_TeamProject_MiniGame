#pragma once
#include "Deck.h"
#include "bPlayer.h"

class Blackjack
{
	Deck deck;
	bPlayer COM;
	bPlayer player;

public:
	bool playGame();
};