#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {
private:
	int x, y;
public:
	void SetCoordinate(int, int);
	bool IsEqual(Coordinate);
	int GetX();
	int GetY();
};
#endif