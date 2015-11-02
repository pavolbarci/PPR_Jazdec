// ppr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <list>
#include <string>
#include <iostream>

using namespace std;

class Coordinate {
private:
	int x, y;
public:
	void SetCoordinate(int, int);
	bool IsEqual(Coordinate); //porovna 2 suradnice ci sa rovnaju a teda, ci uz je na suradnici panacik
};

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

//object ktory ma v sebe konfiguraciu
class Configuration {
private:
	int chessBoardSize, numberOfChessPieces;
	Coordinate horse;
	list<Coordinate> chessPieces; //tu budu suradnice kde su panacikovia
public:
	void SetChessBoard(int, int);
	void SetHorseCoordinate(int, int);
	string AddChessPieceLocation(string);
	//zapise pociatocnu konfiguraciu do suboru
	//void PrintConfigurationToFile();
};

void Configuration::SetChessBoard(int chessBoardSize, int numberOfChessPieces) {
	this->chessBoardSize = chessBoardSize;
	this->numberOfChessPieces = numberOfChessPieces;
}

void Configuration::SetHorseCoordinate(int x, int y) {
	//TODO: doplnit testovanie, ci to nie je out of bounds
	this->horse.SetCoordinate(x, y);
}

//vstupom je riadok zo suboru, nastavi podla neho suradnicu a porovna vsetky existujuce suradnice
//ak sa rovnaju tak vyhodi string, inak vyhodi empty
string Configuration::AddChessPieceLocation(string str) {
	Coordinate newCooordinate;
	int x = 2;//TODO spravit parsovanie riadku a nech x a y zada d suradnici
	int y = 4;
	newCooordinate.SetCoordinate(x, y);
	for each (Coordinate coordinate in chessPieces)
	{
		if (coordinate.IsEqual(newCooordinate))
		{
			return "There is allready a chesspiece on this place";
		}
	}
	
	//TODO checknut, ci nie je out of bounds
	chessPieces.push_back(newCooordinate);
	return "";
}

Configuration m_configuration;

//nastavi pociatocnu konfiguraciu zo suboru
void InicializeConfiguration( )
{
	ifstream file("input.txt");
	string size, pieces, str;
	getline(file, size);	//prvy riadok je velkost plochy
	getline(file, pieces);	//druhy riadok je pocet panacikov

	m_configuration.SetChessBoard(atoi(size.c_str()), atoi(pieces.c_str()));
	getline(file, str);//treti riadok je pozicia konika
	//TODO parsovanie suradnice
	m_configuration.SetHorseCoordinate(2,5);

	while (getline(file, str))//prechadza zvysne riadky a uklada do onfiguracie suradnice
	{
		string message = m_configuration.AddChessPieceLocation(str);
		if (!message.empty())//pokial teda vrati sa string, vypise ho a zastavi sa to, inak pokracuje
		{
			cout << message;
			break;
		}
	}
}

int main()
{
	InicializeConfiguration();
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