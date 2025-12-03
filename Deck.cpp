#include "Deck.h"
#include <algorithm>
#include <random>
#include <ctime>

Deck::Deck()
{
	string marks[] = { "¢¼", "¢¾", "¡ß", "¢À" };
	string ranks[] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	int values[] = { 11,2,3,4,5,6,7,8,9,10,10,10,10 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			createDeck.push_back(Card(marks[i], ranks[j], values[j]));
		}
	}
	index = 0;
	shuffle();
}

void Deck::shuffle()
{
	srand((unsigned)time(0));

	for (int i = 0; i < createDeck.size() - 1; i++)
	{
		int j = rand() % (i + 1);
		Card temp = createDeck[i];
		createDeck[i] = createDeck[j];
		createDeck[j] = temp;
	}

	index = 0;
}

Card Deck::drawCard()
{
	if (index >= createDeck.size())
	{
		shuffle();
	}
	return createDeck[index++];
}

