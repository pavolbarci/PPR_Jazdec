#include "stdafx.h"
#include "Coordinate.h"
#include <list>

using namespace std;

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

void Coordinate::SetNextList(list<Coordinate> coordinatesList)
{
	this->nextCoordinatesList = coordinatesList;
}

//vrati x a y suradnicu
int Coordinate::GetX() { return x; }
int Coordinate::GetY() { return y; }
list<Coordinate> Coordinate::GetNextCoordinates() { return this->nextCoordinatesList; }