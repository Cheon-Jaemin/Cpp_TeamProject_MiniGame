#include "Blackjack.h"
#include <iostream>
using namespace std;

bool Blackjack::playGame()
{
	COM.clearHand();
	player.clearHand();

	player.addCard(deck.drawCard());
	COM.addCard(deck.drawCard());
	player.addCard(deck.drawCard());
	COM.addCard(deck.drawCard());

	cout << "컴퓨터 핸드: " << endl;
	COM.showHand(true);
	cout << endl;

	cout << "플레이어 핸드: " << endl;
	player.showHand();
	cout << endl;

	while (true)
	{
		if (player.isBust())
		{
			cout << "플레이어 버스트! 패배했습니다." << endl;
			return false;
		}
		cout << "Hit (h) or Stand (s)?";
		char choice;
		cin >> choice;
		if (choice == 'h' || choice == 'H')
		{
			player.addCard(deck.drawCard());
			cout << "\n 플레이어 핸드 : " << endl;
			player.showHand();
		}
		else if (choice == 's' || choice == 'S')
		{
			break;
		}
	}

	cout << "\nCOM 턴 : " << endl;
	COM.showHand();
	while (COM.getScore() < 17)
	{
		COM.addCard(deck.drawCard());
		COM.showHand();
	}

	if (COM.isBust())
	{
		cout << "COM 버스트! 승리하셨습니다. ^~^" << endl;
		return true;
	}
	else
	{
		int playerScore = player.getScore();
		int comScore = COM.getScore();
		if (playerScore > comScore)
		{
			cout << "플레이어 승리!" << endl;
			return true;
		}
		else if (playerScore < comScore)
		{
			cout << "COM 승리!" << endl;
			return false;
		}
		else
		{
			cout << "비겼습니다!" << endl;
			return false;
		}
	}
}