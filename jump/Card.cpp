#include "Card.h"

Card::Card(string mark, string rank, int value)
{
	this->mark = mark;
	this->rank = rank;
	this->value = value;
}

string Card::toString() const
{
	return mark + " " + rank;
}

int Card::getValue() const
{
	return value;
}

string Card::getRank() const
{
	return rank;
}