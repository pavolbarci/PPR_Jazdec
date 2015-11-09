#include "stdafx.h"
#include "Configuration.h"
#include "Coordinate.h"
#include "CoordinateWithValue.h"
#include <sstream>
#include <fstream>

using namespace std;

//nastavi zakladne parametre, velkost a pocet panacikov
void Configuration::SetChessBoard(int chessBoardSize, int numberOfChessPieces) {
	this->chessBoardSize = chessBoardSize;
	this->numberOfChessPieces = numberOfChessPieces;
}

//nastavi konove suradnice
string Configuration::SetHorseCoordinate(Coordinate coordinate) { 
	if (coordinate.GetX() >= this->chessBoardSize && coordinate.GetY() >= this->chessBoardSize)
	{
		return "Coordinates are out of bounds";
	}
	this->horse = coordinate;

	return "";
}

void Configuration::InicializeChessBoard()
{
	chessBoard = new CoordinateWithValue*[chessBoardSize];
	for (int x = 0; x < chessBoardSize; x++)
	{
		chessBoard[x] = new CoordinateWithValue[chessBoardSize];
		for (int y = 0; y < chessBoardSize; y++)
		{
			chessBoard[x][y].SetValue(0);
		}
	}

	for each (Coordinate coordinate in chessPieces)
	{ 
		chessBoard[coordinate.GetX()][coordinate.GetY()].SetValue(1);
	}
}

void Configuration::SetChessboardsCoordinateValue(Coordinate coordinate)
{
	this->chessBoard[coordinate.GetX()][coordinate.GetY()].SetValue(0);
}

//vstupom je riadok zo suboru, nastavi podla neho suradnicu a porovna vsetky existujuce suradnice
//ak sa rovnaju tak vyhodi string, inak vyhodi empty
string Configuration::AddChessPieceLocation(string str) {
	Coordinate newCooordinate;
	size_t pos = str.find(",");
	int x = atoi(str.substr(0, pos).c_str());
	int y = atoi(str.substr(pos + 1).c_str());

	if (x >= this->chessBoardSize && x >= this->chessBoardSize)
	{
		ostringstream oss;
		oss << "Coordinate x = " << x << ", y = " << y << " is out of bounds" << endl;
		return oss.str();
	}

	newCooordinate.SetCoordinate(x, y);

	for each (Coordinate coordinate in chessPieces)
	{
		if (coordinate.IsEqual(newCooordinate))
		{
			ostringstream oss;
			oss << "There is allready a chesspiece on coordinate x = " << x << ", y = " << y << endl;
			return oss.str();
		}
	}

	chessPieces.push_back(newCooordinate);
	return "";
}

//zapise pociatocnu konfiguraciu do suboru
void Configuration::PrintConfigurationToFile()
{
	ofstream output;
	output.open("output_configuration.txt");

	output << "Chessboard size is set to: " << this->chessBoardSize << endl;
	output << "Number of chesspieces on chessboard is: " << this->numberOfChessPieces << endl;
	output << "Horse location coordinates are: x = " << this->horse.GetX() << ", y = " << this->horse.GetY() << endl;
	output << "Chesspieces coordinates are: " << endl;

	int counter = 1;
	for each (Coordinate coordinate in this->chessPieces)
	{
		output << "\t" << counter << ": x = " << coordinate.GetX() << ", y = " << coordinate.GetY() << endl;
		counter++;
	}
	output.close();
}

//pokial sa na pozicii konika nachadza figurka, odstrani ju zo zoznamu figurok
//void Configuration::RemoveChessPiece()
//{
//	for (list<Coordinate>::iterator it = chessPieces.begin(); it != chessPieces.end(); it++)
//	{
//		if (it->IsEqual(horse))
//		{
//			chessPieces.erase(it);
//			break;
//		}
//	}
//}

//gettre na konfiguracne parametre
int Configuration::GetChessBoardSize() { return this->chessBoardSize; }
int Configuration::GetNumberOfChessPieces() { return this->numberOfChessPieces; }
Coordinate Configuration::GetHorseCoordinate() { return this->horse; }
list<Coordinate> Configuration::GetChessPiecesCoordinates() { return this->chessPieces; }
CoordinateWithValue Configuration::GetCoordinationWithValue(int x, int y) { return this->chessBoard[x][y]; }




int Configuration::GetValue(Coordinate coordinate) { return this->chessBoard[coordinate.GetX()][coordinate.GetY()].GetValue(); }