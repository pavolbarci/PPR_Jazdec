#ifndef COORDINATEWITHVALUE_H
#define COORDINATEWITHVALUE_H

#include "Coordinate.h"
#include <list>

using namespace std;

class CoordinateWithValue {
private:
	int value;
	Coordinate coordinate;
	list<CoordinateWithValue> nextCoordinatesList;
public:
	void SetCoordinate(Coordinate);
	void SetValue(int);
	void SetNextList(list<CoordinateWithValue>);
	Coordinate GetCoordinate();
	int GetValue();
	list<CoordinateWithValue> GetNextCoordinates();
};
#endif