#pragma once
#include "Blackjack.h"
#include <iostream>

class GameManager
{
	int playerCoin;

public:
	GameManager(int coin)
	{
		playerCoin = coin;
	}

	bool playRound(int bet)
	{
		if (bet > playerCoin)
		{
			std::cout << "코인이 부족합니다!" << std::endl;
			return false;
		}

		playerCoin -= bet;

		Blackjack game;
		bool playerWin = game.playGame();

		if (playerWin)
		{
			playerCoin += bet * 2;
			std::cout << "승리! 코인이 " << bet * 2 << " 만큼 증가했습니다." << std::endl;
		}
		else
		{
			std::cout << "패배." << std::endl;
		}

		std::cout << "현재 코인 : " << playerCoin << std::endl;
		return playerWin;
	}

	int getCoin() const
	{
		return playerCoin;
	}
};