#pragma once
#include <vector>
#include "Card.h"

class Deck
{
	vector<Card> createDeck;
	int index;

public:
	Deck();
	void shuffle();
	Card drawCard();
};