#include "stdafx.h"
#include "Coordinate.h"

//nastavi suradnice
void Coordinate::SetCoordinate(int x, int y) {
	this->x = x;
	this->y = y;
}

//porovna 2 suradnice ci sa rovnaju a teda, ci uz je na suradnici panacik
bool Coordinate::IsEqual(Coordinate coordinate) {
	if (x == coordinate.x && y == coordinate.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//vrati x a y suradnicu
int Coordinate::GetX() { return x; }
int Coordinate::GetY() { return y; }