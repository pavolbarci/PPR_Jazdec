// ppr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//objekt reprezentujuci suradnicu
class Coordinate {
private:
	int x, y;
public:
	void SetCoordinate(int, int);
	bool IsEqual(Coordinate); 
	int GetX();
	int GetY();
};

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
	//gettre na konfiguracne parametre, ale asi netreba
	//int GetChessBoardSize();
	//int GetNumberOfChessPieces();
	//Coordinate GetHorseCoordinate();
	//list<Coordinate> GetChessPiecesCoordinates();
};

//globalna premenna konfiguracie (modelu)
Configuration m_configuration;

//gettre na konfiguracne parametre, ale asi netreba
//int Configuration::GetChessBoardSize() { return m_configuration.chessBoardSize; }
//int Configuration::GetNumberOfChessPieces() { return m_configuration.numberOfChessPieces; }
//Coordinate Configuration::GetHorseCoordinate() { return m_configuration.horse; }
//list<Coordinate> Configuration::GetChessPiecesCoordinates() { return m_configuration.chessPieces; }

//nastavi zakladne parametre, velkost a pocet panacikov
void Configuration::SetChessBoard(int chessBoardSize, int numberOfChessPieces) {
	this->chessBoardSize = chessBoardSize;
	this->numberOfChessPieces = numberOfChessPieces;
}

//nastavi konove suradnice
string Configuration::SetHorseCoordinate(int x, int y) {
	if (x >= m_configuration.chessBoardSize && x >= m_configuration.chessBoardSize)
	{
		return "Coordinates are out of bounds";
	}
	this->horse.SetCoordinate(x, y);

	return "";
}

//vstupom je riadok zo suboru, nastavi podla neho suradnicu a porovna vsetky existujuce suradnice
//ak sa rovnaju tak vyhodi string, inak vyhodi empty
string Configuration::AddChessPieceLocation(string str) {
	Coordinate newCooordinate;
	size_t pos = str.find(",");
	int x = atoi(str.substr(0, pos).c_str());
	int y = atoi(str.substr(pos + 1).c_str());

	if (x >= m_configuration.chessBoardSize && x >= m_configuration.chessBoardSize)
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

	output << "Chessboard size is set to: " << m_configuration.chessBoardSize << endl;
	output << "Number of chesspieces on chessboard is: " << m_configuration.numberOfChessPieces << endl;
	output << "Horse location coordinates are: x = " << m_configuration.horse.GetX() << ", y = " << m_configuration.horse.GetY() << endl;
	output << "Chesspieces coordinates are: " << endl;

	int counter = 1;
	for each (Coordinate coordinate in m_configuration.chessPieces)
	{
		output << "\t" << counter << ": x = " << coordinate.GetX() << ", y = " << coordinate.GetY() << endl;
		counter++;
	}
	output.close();
}

//nastavi pociatocnu konfiguraciu zo suboru
string InicializeConfiguration( )
{
	ifstream file("input.txt");
	string size, pieces, str, errorMessage;
	getline(file, size);	//prvy riadok je velkost plochy
	getline(file, pieces);	//druhy riadok je pocet panacikov

	m_configuration.SetChessBoard(atoi(size.c_str()), atoi(pieces.c_str()));
	getline(file, str);	//treti riadok je pozicia konika, kde x a y su oddelene ciarkou
	
	size_t pos = str.find(",");
	errorMessage = m_configuration.SetHorseCoordinate(atoi(str.substr(0, pos).c_str()), atoi(str.substr(pos + 1).c_str()));
	if (!errorMessage.empty())//pokial sa vrati chybova hlaska, vyhodi ju do mainu, stopne sa program a hlaska sa vypise
	{
		return errorMessage;
	}

	while (getline(file, str))//prechadza zvysne riadky a uklada do onfiguracie suradnice
	{
		errorMessage = m_configuration.AddChessPieceLocation(str);
		if (!errorMessage.empty())//pokial sa vrati chybova hlaska, vyhodi ju do mainu, stopne sa program a hlaska sa vypise
		{
			return errorMessage;
		}
	}

	return "";
}

int main()
{
	string errorMessage = InicializeConfiguration();
	
	if (errorMessage.empty())
	{
		m_configuration.PrintConfigurationToFile();
	}
	else
	{
		cout << errorMessage << endl;
	}
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