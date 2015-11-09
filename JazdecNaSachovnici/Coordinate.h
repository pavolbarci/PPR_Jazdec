#ifndef COORDINATE_H
#define COORDINATE_H

#include <list>

using namespace std;

class Coordinate {
private:
	int x, y;
	list<Coordinate> nextCoordinatesList;
public:
	void SetCoordinate(int, int);
	bool IsEqual(Coordinate);
	int GetX();
	int GetY();
	void SetNextList(list<Coordinate>);
	list<Coordinate> GetNextCoordinates();
};
#endif