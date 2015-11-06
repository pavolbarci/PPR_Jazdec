#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Coordinate.h"
#include <list>

using namespace std;

//object ktory ma v sebe pociatocnu konfiguraciu
class Configuration {
private:
	int chessBoardSize, numberOfChessPieces;
	Coordinate horse;
	list<Coordinate> chessPieces; //tu budu suradnice kde su panacikovia
public:
	void SetChessBoard(int, int);
	string SetHorseCoordinate(int, int);
	string AddChessPieceLocation(string);
	void PrintConfigurationToFile();	
	int GetChessBoardSize();
	int GetNumberOfChessPieces();
	Coordinate GetHorseCoordinate();
	list<Coordinate> GetChessPiecesCoordinates();
};
#endif