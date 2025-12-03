#include "bPlayer.h"
#include <iostream>
using namespace std;

void bPlayer::addCard(Card card)
{
	handCard.push_back(card);
}

int bPlayer::getScore() const
{
	int score = 0;
	int ace = 0;

	for (int i = 0; i < handCard.size(); i++)
	{
		score = score + handCard[i].getValue();
		if (handCard[i].getRank() == "A")
		{
			ace++;
		}
	}

	while (score > 21 && ace > 0)
	{
		score -= 10;
		ace--;
	}

	return score;
}

void bPlayer::showHand(bool hide) const
{
	for (int i = 0; i < handCard.size(); i++)
	{
		if (i == 0 && hide)
		{
			cout << "[Card]" << endl;
		}
		else
		{
			cout << handCard[i].toString() << endl;
		}
	}
	if (!hide)
	{
		cout << "Score : " << getScore() << endl;
	}
}

bool bPlayer::isBust() const
{
	return getScore() > 21;
}

void bPlayer::clearHand()
{
	handCard.clear();
}

