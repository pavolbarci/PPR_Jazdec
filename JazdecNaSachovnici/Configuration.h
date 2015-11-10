#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Coordinate.h"
#include "CoordinateWithValue.h"
#include <list>

using namespace std;

//object ktory ma v sebe pociatocnu konfiguraciu
class Configuration {
private:
	int chessBoardSize, numberOfChessPieces;
	Coordinate horse;
	list<Coordinate> chessPieces; //tu budu suradnice kde su panacikovia
public:
	CoordinateWithValue **chessBoard;
	void SetChessBoard(int, int);
	void InicializeChessBoard();
	string SetHorseCoordinate(Coordinate);
	string AddChessPieceLocation(string);
	void PrintConfigurationToFile();
	//void RemoveChessPiece();

	int GetChessBoardSize();
	int GetNumberOfChessPieces();
	Coordinate GetHorseCoordinate();
	CoordinateWithValue GetCoordinationWithValue(int, int);
	list<Coordinate> GetChessPiecesCoordinates();
	bool WasThereChessPiece(Coordinate);
	CoordinateWithValue** CopyChessboard();

	int GetValue(Coordinate);
};
#endif