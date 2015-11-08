// ppr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Coordinate.h"
#include "Configuration.h"
#include "CoordinateWithValue.h"
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//globalna premenna konfiguracie (modelu)
Configuration m_configuration;
list<CoordinateWithValue> m_coordinatesWithValue;
int m_visited[100][100];

//nastavi pociatocnu konfiguraciu zo suboru
string InicializeConfiguration()
{
	ifstream file("input.txt");
	//ifstream file("tempinput.txt");
	//ifstream file("100input.txt");
	string size, pieces, str, errorMessage;
	getline(file, size);	//prvy riadok je velkost plochy
	getline(file, pieces);	//druhy riadok je pocet panacikov

	m_configuration.SetChessBoard(atoi(size.c_str()), atoi(pieces.c_str()));
	getline(file, str);	//treti riadok je pozicia konika, kde x a y su oddelene ciarkou

	size_t pos = str.find(",");
	Coordinate horseCoordinate;
	horseCoordinate.SetCoordinate(atoi(str.substr(0, pos).c_str()), atoi(str.substr(pos + 1).c_str()));
	errorMessage = m_configuration.SetHorseCoordinate(horseCoordinate);
	if (!errorMessage.empty())//pokial sa vrati chybova hlaska, vyhodi ju do mainu, stopne sa program a hlaska sa vypise
	{
		return errorMessage;
	}

	for (int i = 0; i < m_configuration.GetNumberOfChessPieces(); i++)//prechadza zvysne riadky iba do poctu panacikov a uklada do onfiguracie suradnice
	{
		getline(file, str);
		errorMessage = m_configuration.AddChessPieceLocation(str);
		if (!errorMessage.empty())//pokial sa vrati chybova hlaska, vyhodi ju do mainu, stopne sa program a hlaska sa vypise
		{
			return errorMessage;
		}
	}

	if (m_configuration.GetNumberOfChessPieces() != m_configuration.GetChessPiecesCoordinates().size())
	{
		ostringstream oss;
		oss << "You have declared " << m_configuration.GetNumberOfChessPieces() << " but only " << m_configuration.GetChessPiecesCoordinates().size() << " coordinates." << endl;
		return oss.str();
	}

	return "";
}

bool IsNextValid(int nextX, int nextY)
{
	if (nextX < m_configuration.GetChessBoardSize() 
		&& nextY < m_configuration.GetChessBoardSize() 
		&& nextX > -1 && nextY > -1)
	{
		return true;
	}
	return false;
}

int val(Coordinate c)
{
	list<Coordinate> pieces = m_configuration.GetChessPiecesCoordinates();

	for each (Coordinate coordinate in pieces)
	{
		if (coordinate.IsEqual(c))
		{
			return 1;
		}
	}
	return 0;
}

list<CoordinateWithValue> NextStep(Coordinate* coordinate)
{
	list<CoordinateWithValue> nextCoordinatesList;
	int x, y;
	if (coordinate == NULL)
	{
		x = m_configuration.GetHorseCoordinate().GetX();
		y = m_configuration.GetHorseCoordinate().GetY();
	}
	else
	{
		x = coordinate->GetX();
		y = coordinate->GetY();
	}

	int size = m_configuration.GetChessBoardSize();
	CoordinateWithValue coordinateWithValue;
	Coordinate nextCoordinate;


	if (IsNextValid(x + 2, y + 1))
	{
		nextCoordinate.SetCoordinate(x + 2, y + 1);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x + 2, y - 1))
	{
		nextCoordinate.SetCoordinate(x + 2, y - 1);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x - 2, y + 1))
	{
		nextCoordinate.SetCoordinate(x - 2, y + 1);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x - 2, y - 1))
	{
		nextCoordinate.SetCoordinate(x - 2, y - 1);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x + 1, y + 2))
	{
		nextCoordinate.SetCoordinate(x + 1, y + 2);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x - 1, y + 2))
	{
		nextCoordinate.SetCoordinate(x - 1, y + 2);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x - 1, y - 2))
	{
		nextCoordinate.SetCoordinate(x - 1, y - 2);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	if (IsNextValid(x + 1, y - 2))
	{
		nextCoordinate.SetCoordinate(x + 1, y - 2);
		coordinateWithValue.SetCoordinate(nextCoordinate);
		coordinateWithValue.SetValue(val(nextCoordinate));
		nextCoordinatesList.push_back(coordinateWithValue);
	}

	return nextCoordinatesList;
}

//vyhlada a vrati coordinate najlepsieho mozneho kroku
Coordinate FindBestWay()
{
	int bestValue = 0;
	int leastVisited = 0;
	Coordinate leastVisitedC;
	//pokial nenajde, vrati prvy (toto nejako treba zmenit, lebo sa zacykli)

	Coordinate coordinate = (m_coordinatesWithValue.begin())->GetCoordinate();
	if (m_visited[m_coordinatesWithValue.begin()->GetCoordinate().GetX()][m_coordinatesWithValue.begin()->GetCoordinate().GetY()]==0)
	{
		Coordinate coordinate = (m_coordinatesWithValue.begin())->GetCoordinate();
	}
	//prechadza X
	for (list<CoordinateWithValue>::iterator it1 = m_coordinatesWithValue.begin(); it1 != m_coordinatesWithValue.end(); it1++)
	{
		//musel som to vytiahnut
		list<CoordinateWithValue> listCoo = it1->GetNextCoordinates();

		//prechadza next(x)
		for (list<CoordinateWithValue>::iterator it2 = listCoo.begin(); it2 != listCoo.end(); it2++)
		{
			//vypocita value
			int actualValue = 8 * it1->GetValue() + it2->GetValue();
			if (actualValue > bestValue)
			{
				//ak je value vacsia ako best, best nastavu na actual a nastavi aktualny X coordinate
				bestValue = actualValue;
				coordinate = (*it1).GetCoordinate();
				if (bestValue == 9)
				{
					m_visited[coordinate.GetX()][coordinate.GetY()]++;
					cout << coordinate.GetX();
					cout << ",";
					cout << coordinate.GetY() << endl;
					return coordinate;
				}
			}
		}
	}
	leastVisitedC = coordinate;

	if (m_visited[coordinate.GetX()][coordinate.GetY()] != 0 && bestValue == 0)
	{
		leastVisited = m_visited[coordinate.GetX()][coordinate.GetY()];

		for (list<CoordinateWithValue>::iterator it3 = m_coordinatesWithValue.begin(); it3 != m_coordinatesWithValue.end(); it3++)
		{
			if (m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()] == 0)
			{

				m_visited[coordinate.GetX()][coordinate.GetY()]++;
				coordinate = (*it3).GetCoordinate();
			}
			else
			{
				if (leastVisited > m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()])
				{
					leastVisited = m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()];
					leastVisitedC = it3->GetCoordinate();
				}
			}
		}

	}

		m_visited[leastVisitedC.GetX()][leastVisitedC.GetY()]++;
		cout << leastVisitedC.GetX();
		cout << ",";
		cout << leastVisitedC.GetY() << endl;
		return leastVisitedC;
	
}

void Jump()
{
	m_configuration.SetHorseCoordinate(FindBestWay());
	m_configuration.RemoveChessPiece();
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

	int i=0;

	while (i<m_configuration.GetChessBoardSize()*m_configuration.GetChessBoardSize() && m_configuration.GetChessPiecesCoordinates().size() !=0)
	{
		i++;
		m_coordinatesWithValue = NextStep(NULL);

		for (list<CoordinateWithValue>::iterator it = m_coordinatesWithValue.begin(); it != m_coordinatesWithValue.end(); it++)
		{
			(*it).SetNextList(NextStep(&(*it).GetCoordinate()));
		}
		Jump();
	}
	printf("schluss");
	return 0;
}