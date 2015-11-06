#include "stdafx.h"
#include "Coordinate.h"
#include "CoordinateWithValue.h"
#include <list>

using namespace std;

void CoordinateWithValue::SetCoordinate(Coordinate coordinate)
{
	this->coordinate = coordinate;
}

void CoordinateWithValue::SetValue(int value)
{
	this->value = value;
}

void CoordinateWithValue::SetNextList(list<CoordinateWithValue> coordinatesList)
{
	this->nextCoordinatesList = coordinatesList;
}

Coordinate CoordinateWithValue::GetCoordinate() { return this->coordinate; }
int CoordinateWithValue::GetValue() { return this->value; }
list<CoordinateWithValue> CoordinateWithValue::GetNextCoordinates() { return this->nextCoordinatesList; }