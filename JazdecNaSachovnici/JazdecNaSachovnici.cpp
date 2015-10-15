// ppr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"

class Coordinate {
private:
	int x, y;
public:
	void setCoordinate(int, int);
};

void Coordinate::setCoordinate(int x, int y) {
	this->x = x;
	this->y = y;
}


int main()
{
	//DINDU NUFFINS!
	Coordinate co;
	co.setCoordinate(1, 2);
	return 0;
}

int val(Coordinate c)
{
	if (1 == 1)
	{

		return 1;
	}
	return 0;
}