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
#include <deque>
#include <iterator>

using namespace std;

//globalna premenna konfiguracie (modelu)
Configuration m_configuration;
list<CoordinateWithValue> m_coordinatesWithValue;
int m_lowerLimit;
int m_upperLimit;
list<CoordinateWithValue> m_bestSolution;
CoordinateWithValue** m_chessBoard;
list<CoordinateWithValue> actualSolution;

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

	m_configuration.InicializeChessBoard();
	m_chessBoard = new CoordinateWithValue*[m_configuration.GetChessBoardSize()];
	for (int x = 0; x < m_configuration.GetChessBoardSize(); x++)
	{
		m_chessBoard[x] = new CoordinateWithValue[m_configuration.GetChessBoardSize()];
		for (int y = 0; y < m_configuration.GetChessBoardSize(); y++)
		{
			m_chessBoard[x][y].SetValue(0);
		}
	}
	for each (Coordinate coordinate in m_configuration.GetChessPiecesCoordinates())
	{
		m_chessBoard[coordinate.GetX()][coordinate.GetY()].SetValue(1);
	}
	m_lowerLimit = m_configuration.GetNumberOfChessPieces();
	m_upperLimit = (m_configuration.GetChessBoardSize() * m_configuration.GetChessBoardSize()) - 1;
	//m_chessBoard = m_configuration.CopyChessboard();
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

//int val(Coordinate c)
//{
//	list<Coordinate> pieces = m_configuration.GetChessPiecesCoordinates();
//
//	for each (Coordinate coordinate in pieces)
//	{
//		if (coordinate.IsEqual(c))
//		{
//			return 1;
//		}
//	}
//	return 0;
//}

list<CoordinateWithValue> NextStep(CoordinateWithValue* coordinate)
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
		x = coordinate->GetCoordinate().GetX();
		y = coordinate->GetCoordinate().GetY();
	}

	int size = m_configuration.GetChessBoardSize();
	Coordinate nextCoordinate;
	CoordinateWithValue CWV;

	if (IsNextValid(x + 2, y + 1))
	{
		nextCoordinate.SetCoordinate(x + 2, y + 1);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x + 2, y - 1))
	{
		nextCoordinate.SetCoordinate(x + 2, y - 1);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x - 2, y + 1))
	{
		nextCoordinate.SetCoordinate(x - 2, y + 1);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x - 2, y - 1))
	{
		nextCoordinate.SetCoordinate(x - 2, y - 1);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x + 1, y + 2))
	{
		nextCoordinate.SetCoordinate(x + 1, y + 2);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x - 1, y + 2))
	{
		nextCoordinate.SetCoordinate(x - 1, y + 2);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x - 1, y - 2))
	{
		nextCoordinate.SetCoordinate(x - 1, y - 2);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}

	if (IsNextValid(x + 1, y - 2))
	{
		nextCoordinate.SetCoordinate(x + 1, y - 2);
		CWV.SetCoordinate(nextCoordinate);
		CWV.SetValue(m_chessBoard[nextCoordinate.GetX()][nextCoordinate.GetY()].GetValue());
		nextCoordinatesList.push_back(CWV);
	}
	nextCoordinatesList.sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() > b.GetValue(); });
	if (coordinate != NULL)
	{
		coordinate->SetValue(8 * m_chessBoard[coordinate->GetCoordinate().GetX()][coordinate->GetCoordinate().GetY()].GetValue() + nextCoordinatesList.front().GetValue());
	}

	return nextCoordinatesList;
}

//list<CoordinateWithValue> NextStep(Coordinate* coordinate)
//{
//	list<CoordinateWithValue> nextCoordinatesList;
//	int x, y;
//	if (coordinate == NULL)
//	{
//		x = m_configuration.GetHorseCoordinate().GetX();
//		y = m_configuration.GetHorseCoordinate().GetY();
//	}
//	else
//	{
//		x = coordinate->GetX();
//		y = coordinate->GetY();
//	}
//
//	int size = m_configuration.GetChessBoardSize();
//	CoordinateWithValue coordinateWithValue;
//	Coordinate nextCoordinate;
//
//
//	if (IsNextValid(x + 2, y + 1))
//	{
//		nextCoordinate.SetCoordinate(x + 2, y + 1);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x + 2, y + 1));
//	}
//
//	if (IsNextValid(x + 2, y - 1))
//	{
//		nextCoordinate.SetCoordinate(x + 2, y - 1);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x + 2, y - 1));
//	}
//
//	if (IsNextValid(x - 2, y + 1))
//	{
//		nextCoordinate.SetCoordinate(x - 2, y + 1);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x - 2, y + 1));
//	}
//
//	if (IsNextValid(x - 2, y - 1))
//	{
//		nextCoordinate.SetCoordinate(x - 2, y - 1);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x - 2, y - 1));
//	}
//
//	if (IsNextValid(x + 1, y + 2))
//	{
//		nextCoordinate.SetCoordinate(x + 1, y + 2);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x + 1, y + 2));
//	}
//
//	if (IsNextValid(x - 1, y + 2))
//	{
//		nextCoordinate.SetCoordinate(x - 1, y + 2);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x - 1, y + 2));
//	}
//
//	if (IsNextValid(x - 1, y - 2))
//	{
//		nextCoordinate.SetCoordinate(x - 1, y - 2);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x - 1, y - 2));
//	}
//
//	if (IsNextValid(x + 1, y - 2))
//	{
//		nextCoordinate.SetCoordinate(x + 1, y - 2);
//		//coordinateWithValue.SetCoordinate(nextCoordinate);
//		//coordinateWithValue.SetValue(val(nextCoordinate));
//		nextCoordinatesList.push_back(m_configuration.GetCoordinationWithValue(x + 1, y - 2));
//	}
//
//	return nextCoordinatesList;
//}

//vyhlada a vrati coordinate najlepsieho mozneho kroku
//Coordinate FindBestNextJump()
//{
//	int bestValue = 0;
//	int leastVisited = 0;
//	Coordinate leastVisitedC;
//	//pokial nenajde, vrati prvy (toto nejako treba zmenit, lebo sa zacykli)
//
//	Coordinate coordinate = (m_coordinatesWithValue.begin())->GetCoordinate();
//
//	if (m_visited[m_coordinatesWithValue.begin()->GetCoordinate().GetX()][m_coordinatesWithValue.begin()->GetCoordinate().GetY()] == 0)
//	{
//		Coordinate coordinate = (m_coordinatesWithValue.begin())->GetCoordinate();
//	}
//
//	//prechadza X
//	for (list<CoordinateWithValue>::iterator it1 = m_coordinatesWithValue.begin(); it1 != m_coordinatesWithValue.end(); it1++)
//	{
//		//musel som to vytiahnut
//		list<CoordinateWithValue> listCoo = it1->GetNextCoordinates();
//
//		//prechadza next(x)
//		for (list<CoordinateWithValue>::iterator it2 = listCoo.begin(); it2 != listCoo.end(); it2++)
//		{
//			//vypocita value
//			int actualValue = 8 * it1->GetValue() + it2->GetValue();
//			if (actualValue > bestValue)
//			{
//				//ak je value vacsia ako best, best nastavu na actual a nastavi aktualny X coordinate
//				bestValue = actualValue;
//				coordinate = (*it1).GetCoordinate();
//				if (bestValue == 9)
//				{
//					m_visited[coordinate.GetX()][coordinate.GetY()]++;
//					cout << coordinate.GetX();
//					cout << ",";
//					cout << coordinate.GetY() << endl;
//					return coordinate;
//				}
//			}
//		}
//	}
//	leastVisitedC = coordinate;
//
//	if (m_visited[coordinate.GetX()][coordinate.GetY()] != 0 && bestValue == 0)
//	{
//		leastVisited = m_visited[coordinate.GetX()][coordinate.GetY()];
//
//		for (list<CoordinateWithValue>::iterator it3 = m_coordinatesWithValue.begin(); it3 != m_coordinatesWithValue.end(); it3++)
//		{
//			if (m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()] == 0)
//			{
//
//				m_visited[coordinate.GetX()][coordinate.GetY()]++;
//				coordinate = (*it3).GetCoordinate();
//			}
//			else
//			{
//				if (leastVisited > m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()])
//				{
//					leastVisited = m_visited[it3->GetCoordinate().GetX()][it3->GetCoordinate().GetY()];
//					leastVisitedC = it3->GetCoordinate();
//				}
//			}
//		}
//
//	}
//
//		m_visited[leastVisitedC.GetX()][leastVisitedC.GetY()]++;
//		cout << leastVisitedC.GetX() << "," << leastVisitedC.GetY() << endl;
//		return leastVisitedC;
//	
//}

//list<CoordinateWithValue> SortCoordinatesByValue(list<Coordinate>* nextSteps)
//Coordinate SortCoordinatesByValue(list<Coordinate>* nextSteps)
//{
//	int bestValue = 0;
//	int leastVisited = 0;
//	Coordinate coordinate; Coordinate leastVisitedC;
//	list<CoordinateWithValue> coordinatesWithValue;
//
//	coordinate = *(nextSteps->begin());
//	
//
//	for (list<Coordinate>::iterator it1 = nextSteps->begin(); it1 != nextSteps->end(); it1++)
//	{
//		list<Coordinate> listCoo = it1->GetNextCoordinates();
//
//		int bestValue = 0;
//		CoordinateWithValue newCoordinate;
//		newCoordinate.SetCoordinate(*it1);
//
//		for (list<Coordinate>::iterator it2 = listCoo.begin(); it2 != listCoo.end(); it2++)
//		{
//
//			int actualValue = 8 * m_configuration.GetValue(*it1) + m_configuration.GetValue(*it2);
//			if (actualValue > bestValue)
//			{
//				//ak je value vacsia ako best, best nastavu na actual a nastavi aktualny X coordinate
//				bestValue = actualValue;
//				coordinate = (*it1);
//				if (bestValue == 9)
//				{
//					m_visited[coordinate.GetX()][coordinate.GetY()]++;
//					cout << coordinate.GetX();
//					cout << ",";
//					cout << coordinate.GetY() << endl;
//					return coordinate;
//				}
//			}
//
//			/*int actualValue = 8 * m_configuration.GetValue(*it1) + m_configuration.GetValue(*it2);
//
//			if (actualValue > bestValue)
//			{
//				bestValue = actualValue;
//				newCoordinate.SetValue(actualValue);
//			}*/
//		}
//		coordinatesWithValue.push_back(newCoordinate);
//	}
//
//	leastVisitedC = coordinate;
//
//	if (m_visited[coordinate.GetX()][coordinate.GetY()] != 0 && bestValue == 0)
//	{
//		leastVisited = m_visited[coordinate.GetX()][coordinate.GetY()];
//
//		for (list<Coordinate>::iterator it3 = nextSteps->begin(); it3 != nextSteps->end(); it3++)
//		{
//			if (m_visited[it3->GetX()][it3->GetY()] == 0)
//			{
//
//				m_visited[coordinate.GetX()][coordinate.GetY()]++;
//				coordinate = (*it3);
//			}
//			else
//			{
//				if (leastVisited > m_visited[it3->GetX()][it3->GetY()])
//				{
//					leastVisited = m_visited[it3->GetX()][it3->GetY()];
//					leastVisitedC = *it3;
//				}
//			}
//		}
//
//	}
//	m_visited[leastVisitedC.GetX()][leastVisitedC.GetY()]++;
//	cout << leastVisitedC.GetX() << "," << leastVisitedC.GetY() << endl;
//	//return leastVisitedC;
//	coordinatesWithValue.sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() > b.GetValue(); });
//
//	//return coordinatesWithValue;
//	return leastVisitedC;
//}

//Coordinate SortCoordinatesByValue(list<Coordinate>* nextSteps)
//{
//	int bestValue = 0;
//	int leastVisited = 0;
//	Coordinate coordinate; Coordinate leastVisitedC;
//	list<CoordinateWithValue> coordinatesWithValue;
//
//	coordinate = *(nextSteps->begin());
//
//
//	for (list<Coordinate>::iterator it1 = nextSteps->begin(); it1 != nextSteps->end(); it1++)
//	{
//		list<Coordinate> listCoo = it1->GetNextCoordinates();
//
//		int bestValue = 0;
//		CoordinateWithValue newCoordinate;
//		newCoordinate.SetCoordinate(*it1);
//
//		for (list<Coordinate>::iterator it2 = listCoo.begin(); it2 != listCoo.end(); it2++)
//		{
//
//			int actualValue = 8 * m_configuration.GetValue(*it1) + m_configuration.GetValue(*it2);
//			if (actualValue > bestValue)
//			{
//				//ak je value vacsia ako best, best nastavu na actual a nastavi aktualny X coordinate
//				bestValue = actualValue;
//				coordinate = (*it1);
//				if (bestValue == 9)
//				{
//					m_visited[coordinate.GetX()][coordinate.GetY()]++;
//					cout << coordinate.GetX();
//					cout << ",";
//					cout << coordinate.GetY() << endl;
//					return coordinate;
//				}
//			}
//
//			/*int actualValue = 8 * m_configuration.GetValue(*it1) + m_configuration.GetValue(*it2);
//
//			if (actualValue > bestValue)
//			{
//			bestValue = actualValue;
//			newCoordinate.SetValue(actualValue);
//			}*/
//		}
//		coordinatesWithValue.push_back(newCoordinate);
//	}
//
//	leastVisitedC = coordinate;
//
//	if (m_visited[coordinate.GetX()][coordinate.GetY()] != 0 && bestValue == 0)
//	{
//		leastVisited = m_visited[coordinate.GetX()][coordinate.GetY()];
//
//		for (list<Coordinate>::iterator it3 = nextSteps->begin(); it3 != nextSteps->end(); it3++)
//		{
//			if (m_visited[it3->GetX()][it3->GetY()] == 0)
//			{
//
//				m_visited[coordinate.GetX()][coordinate.GetY()]++;
//				coordinate = (*it3);
//			}
//			else
//			{
//				if (leastVisited > m_visited[it3->GetX()][it3->GetY()])
//				{
//					leastVisited = m_visited[it3->GetX()][it3->GetY()];
//					leastVisitedC = *it3;
//				}
//			}
//		}
//
//	}
//	m_visited[leastVisitedC.GetX()][leastVisitedC.GetY()]++;
//	cout << leastVisitedC.GetX() << "," << leastVisitedC.GetY() << endl;
//	//return leastVisitedC;
//	coordinatesWithValue.sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() > b.GetValue(); });
//
//	//return coordinatesWithValue;
//	return leastVisitedC;
//}

Coordinate FindBestNextJump(list<CoordinateWithValue>* li)
{
	Coordinate co;
	return co;
}

Coordinate RemoveChessPiece(list<Coordinate>* chessPieces, CoordinateWithValue horse)
{
	Coordinate co;

	for (list<Coordinate>::iterator it = (*chessPieces).begin(); it != (*chessPieces).end(); it++)
	{
		if (it->IsEqual(horse.GetCoordinate()))
		{
			co = *it;

			m_chessBoard[it->GetX()][it->GetY()].SetValue(0);
			(*chessPieces).erase(it);
			return co;
		}
	}
	return co;
}



//Coordinate Jump(list<Coordinate>* chessPiecesLocation, list<Coordinate>* possibleSteps, list<CoordinateWithValue>* nextSteps, Coordinate* lastCoordinate)
//{
//	//possibleSteps.sort([](const Coordinate & a, const Coordinate & b) { return m_configuration.GetValue(a) > m_configuration.GetValue(b); });
//
//	//*nextSteps = SortCoordinatesByValue(possibleSteps);
//	//Coordinate co = SortCoordinatesByValue(possibleSteps);
//
//	//Coordinate nextCoordinate = FindBestNextJump(nextSteps);	
//	//m_configuration.SetHorseCoordinate(nextCoordinate);
//	//	m_configuration.SetHorseCoordinate(nextSteps->front().GetCoordinate());
//	//m_configuration.SetHorseCoordinate(co);
//	//Coordinate co2 = RemoveChessPiece(chessPiecesLocation);
//
//	//if (lastCoordinate != NULL && lastCoordinate->IsEqual(nextSteps->front().GetCoordinate()))
//	//{
//	//	nextSteps->pop_front();
//	//}
//
//	return co;
//
//	//return nextSteps->front().GetCoordinate();
//}

void SortCoordinates(list<CoordinateWithValue>* coordinates)
{
	//for(list<CoordinateWithValue>::iterator)
	coordinates->sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() < b.GetValue(); });
}


//void RemoveFromStack()
//{
//	for (list<CoordinateWithValue>::iterator it = actualSolution.end(); it != actualSolution.begin(); )
//	{
//		--it;
//		if ((*it).GetNextCoordinates().size() == 0)
//		{
//			return actualSolution.remove((*it));
//		}
//	}
//}


void UpdateDuplicates(list<CoordinateWithValue> nextSteps, CoordinateWithValue* coordinate)
{
	for (list<CoordinateWithValue>::iterator it = actualSolution.end(); it != actualSolution.begin(); )
	{
		--it;
		if ((*it).GetCoordinate().IsEqual((*coordinate).GetCoordinate()))
		{
			list<CoordinateWithValue> subList = it->GetNextCoordinates();
			(*it).visited = (*coordinate).visited;
			/*for (list<CoordinateWithValue>::iterator it = subList.end(); it != subList.begin(); )
			{

			}*/
			//it->SetNextList(nextSteps);
		}
	}
}

list<CoordinateWithValue> FindDuplicate(CoordinateWithValue* coordinate)
{
	list<CoordinateWithValue> newList;
	for (list<CoordinateWithValue>::iterator it = actualSolution.end(); it != actualSolution.begin(); )
	{
		--it;
		if ((*it).GetCoordinate().IsEqual((*coordinate).GetCoordinate()))
		{
			list<CoordinateWithValue> temp = it->GetNextCoordinates();
			copy(temp.begin(), temp.end(), back_inserter(newList));
			//newList = (*it).GetNextCoordinates();
			return newList;
		}
	}

	return  coordinate->GetNextCoordinates();
}

void StepBack(list<Coordinate>* chessPiecesPositions)
{
	if (actualSolution.size() != 0)
	{
		Coordinate previous = actualSolution.back().GetCoordinate();
		if (chessPiecesPositions->size() < m_configuration.GetNumberOfChessPieces())
		{
			/*for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
					cout << m_chessBoard[i][j].visited;
				cout << endl;
			}*/

			int temp = m_chessBoard[previous.GetX()][previous.GetY()].visited;
			//if (m_configuration.WasThereChessPiece(previous) && temp == (actualSolution.size()-1))
			if (m_configuration.WasThereChessPiece(previous)) 
			{
				chessPiecesPositions->push_back(previous);
				m_chessBoard[previous.GetX()][previous.GetY()].SetValue(1);
				//m_chessBoard[previous.GetX()][previous.GetY()].visited = 0;
			}
		}

		m_chessBoard[previous.GetX()][previous.GetY()].visited = 0;
		actualSolution.pop_back();
	}
}

void FindBestWay()
{
		int i = 0;

	list<CoordinateWithValue> firstSteps = NextStep(NULL);
	list<CoordinateWithValue> nextSteps = firstSteps;
	list<CoordinateWithValue> listsss;
	int leastVisited = 0;
	Coordinate leastVisitedC;

	CoordinateWithValue konik;
	konik.SetCoordinate(m_configuration.GetHorseCoordinate());
	Coordinate lastCoordinate;

	for (list<CoordinateWithValue>::iterator it = nextSteps.begin(); it != nextSteps.end(); it++)
	{
		(*it).SetNextList(NextStep(&(*it)));
	}
	nextSteps.sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() > b.GetValue(); });
	int minimalnaCena = m_upperLimit;
	konik.SetNextList(nextSteps);
	bool first = true;
	actualSolution.push_back(konik);
	m_chessBoard[konik.GetCoordinate().GetX()][konik.GetCoordinate().GetY()].visited = 1;

	list<Coordinate> chessPiecesPositions = m_configuration.GetChessPiecesCoordinates();
	int moveCounter = 0;
	do
	{
		int x = minimalnaCena - actualSolution.size();
		int chessPieces = chessPiecesPositions.size();
		if (x > 2)
		{
			x = 0;
			chessPieces = 5;
		}
		while (((moveCounter < minimalnaCena) && chessPiecesPositions.size() != 0) 
			&& ( (x + 2) < chessPieces ))
		{
			moveCounter++;

			while(actualSolution.size() > 0 && actualSolution.back().GetNextCoordinates().size() == 0)
			{
				StepBack(&chessPiecesPositions);
				moveCounter--;
			}
			if (actualSolution.size() == 0)
			{
				break;
			}
			CoordinateWithValue newCoordinate = actualSolution.back().GetNextCoordinates().front();
			
			list<CoordinateWithValue> temp = actualSolution.back().GetNextCoordinates();
			if (m_chessBoard[newCoordinate.GetCoordinate().GetX()][newCoordinate.GetCoordinate().GetY()].visited == 1)
			{
				for (list<CoordinateWithValue>::iterator it = temp.begin(); it != temp.end(); it++)
				{
					int temp = m_chessBoard[(*it).GetCoordinate().GetX()][(*it).GetCoordinate().GetY()].visited;
					if (temp == 0)
					{
						newCoordinate = *it;
						break;
					}
				}
			}

			/*newCoordinate.SetCoordinate(actualSolution.back().GetNextCoordinates().front().GetCoordinate());
			newCoordinate.SetValue(actualSolution.back().GetNextCoordinates().front().GetValue());
			newCoordinate.SetNextList(actualSolution.back().GetNextCoordinates().front().GetNextCoordinates());*/

			m_chessBoard[newCoordinate.GetCoordinate().GetX()][newCoordinate.GetCoordinate().GetY()].visited = 1;

			/*if (m_chessBoard[newCoordinate.GetCoordinate().GetX()][newCoordinate.GetCoordinate().GetY()].GetValue() == 1)
			{
				m_chessBoard[newCoordinate.GetCoordinate().GetX()][newCoordinate.GetCoordinate().GetY()].visited = moveCounter;
			}*/
			nextSteps = (&newCoordinate)->GetNextCoordinates();
			nextSteps = FindDuplicate(&newCoordinate);
			//UpdateDuplicates(nextSteps, &newCoordinate);
			

			for (list<CoordinateWithValue>::iterator it = nextSteps.begin(); it != nextSteps.end(); it++)
			{
				(*it).SetNextList(NextStep(&(*it)));
			}
			
			newCoordinate.SetNextList(nextSteps);
			CoordinateWithValue *lastItem = &(actualSolution.back());
			list<CoordinateWithValue> lastItems = lastItem->GetNextCoordinates();

			lastItems.pop_front();
			lastItem->SetNextList(lastItems);
			actualSolution.push_back(newCoordinate);	
			RemoveChessPiece(&chessPiecesPositions, actualSolution.back());
		}
		if (chessPiecesPositions.size() == 0 && moveCounter < minimalnaCena)
		{
			minimalnaCena = moveCounter;
			m_bestSolution = actualSolution;
		}
		int temp = chessPiecesPositions.size();
		StepBack(&chessPiecesPositions);
		//StepBack(&chessPiecesPositions);
		/*if (actualSolution.back().GetValue() < 8)
		{
			StepBack(&chessPiecesPositions);
		}*/
		cout << "cena " << minimalnaCena << " moves " << moveCounter << " id " << i++ << " chesspieces " << temp << endl;
		moveCounter--;
	} while (actualSolution.size() != 0);
}

//void FindBestWay()
//{
//	int i = 0;
//
//	list<CoordinateWithValue> actualSolution;
//	list<CoordinateWithValue> nextSteps = NextStep(NULL);
//	list<CoordinateWithValue> listsss;
//	CoordinateWithValue konik;
//	konik.SetCoordinate(m_configuration.GetHorseCoordinate());
//
//	konik.SetNextList(nextSteps);
//
//	actualSolution.push_back(konik);
//	
//	while(actualSolution.size() != 0)
//	{
//		int moveCoutner = 0;
//		list<Coordinate> chessPiecesPositions = m_configuration.GetChessPiecesCoordinates();
//
//		while ((moveCoutner < m_upperLimit) && chessPiecesPositions.size() != 0)
//		{
//			moveCoutner++;
//
//			for (list<CoordinateWithValue>::iterator it = nextSteps.begin(); it != nextSteps.end(); it++)
//			{
//				(*it).SetNextList(NextStep(&(*it)));
//			}
//			actualSolution.push_back(nextSteps.front());
//			nextSteps = NextStep(&actualSolution.back());
////			SortCoordinates(&(&(actualSolution.back()))->GetNextCoordinates());
//
//
//		}
//		//actualSolution.pop_back();
//		//actualSolution.pop_back();
//	}
//}

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

	FindBestWay();

	printf("schluss");
	return 0;
}



//CoordinateWithValue nextPush = actualSolution.back().GetNextCoordinates().front();


////nextSteps = NextStep(&(actualSolution.back().GetNextCoordinates().front()));
////nextSteps = NextStep(&(nextPush));
//list<CoordinateWithValue>* nextSteps = &((&nextPush)->GetNextCoordinates());

//for (list<CoordinateWithValue>::iterator it = (*nextSteps).begin(); it != (*nextSteps).end(); it++)
//{
//	(*it).SetNextList(NextStep(&(*it)));
//}

//actualSolution.back().GetNextCoordinates().pop_front();
//actualSolution.push_back(nextPush);

//nextSteps.pop_front();

//nextSteps.pop_front();
//nextPush.SetNextList(nextSteps);
//(&actualSolution.back())->SetNextList(nextSteps);


//actualSolution.push_back(nextPush);
//((&actualSolution.back()->GetNextCoordinates())).pop_front();




//actualSolution.push_back(nextSteps.front());
//nextSteps.pop_front();

/*if (m_visited[nextSteps.front().GetCoordinate().GetX()][nextSteps.front().GetCoordinate().GetY()] == 0)
{
actualSolution.push_back(nextSteps.front());
m_visited[nextSteps.front().GetCoordinate().GetX()][nextSteps.front().GetCoordinate().GetY()]++;
}
else
{
actualSolution.push_back(nextSteps.back());
m_visited[nextSteps.front().GetCoordinate().GetX()][nextSteps.front().GetCoordinate().GetY()]++;
}*/