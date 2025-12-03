#pragma once
#include <stdio.h>
#include <string>
using namespace std;

class Card
{
	string mark;
	string rank;
	int value;

public : 
	Card(string mark, string rank, int value);
	string toString() const;
	int getValue() const;
	string getRank() const;
};
