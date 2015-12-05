// ppr.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
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

void Coordinate::SetNextList(list<Coordinate> coordinatesList)
{
	this->nextCoordinatesList = coordinatesList;
}

//vrati x a y suradnicu
int Coordinate::GetX() { return x; }
int Coordinate::GetY() { return y; }
list<Coordinate> Coordinate::GetNextCoordinates() { return this->nextCoordinatesList; }

class CoordinateWithValue {
private:
	int value;
	Coordinate coordinate;
	list<CoordinateWithValue> next;

public:
	int visited = 0;
	void SetCoordinate(Coordinate);
	void SetValue(int);
	void SetNextList(list<CoordinateWithValue>);
	Coordinate GetCoordinate();
	int GetValue();
	list<CoordinateWithValue> GetNextCoordinates();
};


void CoordinateWithValue::SetCoordinate(Coordinate coordinate)
{
	this->coordinate = coordinate;
}

void CoordinateWithValue::SetValue(int value)
{
	this->value = value;
}

void CoordinateWithValue::SetNextList(list<CoordinateWithValue> coordinatesList)
{
	this->next = coordinatesList;
}

Coordinate CoordinateWithValue::GetCoordinate() { return this->coordinate; }
int CoordinateWithValue::GetValue() { return this->value; }
list<CoordinateWithValue> CoordinateWithValue::GetNextCoordinates() { return this->next; }



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
		};
	};

	for (auto coordinate : chessPieces)
	{
		chessBoard[coordinate.GetX()][coordinate.GetY()].SetValue(1);
	};
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

	for (auto coordinate : chessPieces)
	{
		if (coordinate.IsEqual(newCooordinate))
		{
			ostringstream oss;
			oss << "There is allready a chesspiece on coordinate x = " << x << ", y = " << y << endl;
			return oss.str();
		}
	};

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
	for (auto coordinate : this->chessPieces)
	{
		output << "\t" << counter << ": x = " << coordinate.GetX() << ", y = " << coordinate.GetY() << endl;
		counter++;
	};
	output.close();
}

//gettre na konfiguracne parametre
int Configuration::GetChessBoardSize() { return this->chessBoardSize; }
int Configuration::GetNumberOfChessPieces() { return this->numberOfChessPieces; }
Coordinate Configuration::GetHorseCoordinate() { return this->horse; }
list<Coordinate> Configuration::GetChessPiecesCoordinates() { return this->chessPieces; }
CoordinateWithValue Configuration::GetCoordinationWithValue(int x, int y) { return this->chessBoard[x][y]; }


CoordinateWithValue** Configuration::CopyChessboard() { return this->chessBoard; }


int Configuration::GetValue(Coordinate coordinate) { return this->chessBoard[coordinate.GetX()][coordinate.GetY()].GetValue(); }

bool Configuration::WasThereChessPiece(Coordinate coordinate)
{
	if (this->chessBoard[coordinate.GetX()][coordinate.GetY()].GetValue() == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

















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
	};

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
		};
	};
	for (auto coordinate : m_configuration.GetChessPiecesCoordinates())
	{
		m_chessBoard[coordinate.GetX()][coordinate.GetY()].SetValue(1);
	};
	m_lowerLimit = m_configuration.GetNumberOfChessPieces();
	m_upperLimit = (m_configuration.GetChessBoardSize() * m_configuration.GetChessBoardSize()) - 1;
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
	};
	return co;
}

void SortCoordinates(list<CoordinateWithValue>* coordinates)
{
	coordinates->sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() < b.GetValue(); });
}

void UpdateDuplicates(list<CoordinateWithValue> nextSteps, CoordinateWithValue* coordinate)
{
	for (list<CoordinateWithValue>::iterator it = actualSolution.end(); it != actualSolution.begin(); )
	{
		--it;
		if ((*it).GetCoordinate().IsEqual((*coordinate).GetCoordinate()))
		{
			list<CoordinateWithValue> subList = it->GetNextCoordinates();
			(*it).visited = (*coordinate).visited;
		}
	};
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
			return newList;
		}
	};

	return  coordinate->GetNextCoordinates();
}

void StepBack(list<Coordinate>* chessPiecesPositions)
{
	if (actualSolution.size() != 0)
	{
		Coordinate previous = actualSolution.back().GetCoordinate();
		if (chessPiecesPositions->size() < m_configuration.GetNumberOfChessPieces())
		{
			int temp = m_chessBoard[previous.GetX()][previous.GetY()].visited;
			if (m_configuration.WasThereChessPiece(previous))
			{
				chessPiecesPositions->push_back(previous);
				m_chessBoard[previous.GetX()][previous.GetY()].SetValue(1);
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
	};
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
			&& ((x + 2) < chessPieces))
		{
			moveCounter++;

			while (actualSolution.size() > 0 && actualSolution.back().GetNextCoordinates().size() == 0)
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
				};
			}

			m_chessBoard[newCoordinate.GetCoordinate().GetX()][newCoordinate.GetCoordinate().GetY()].visited = 1;

			nextSteps = (&newCoordinate)->GetNextCoordinates();
			nextSteps = FindDuplicate(&newCoordinate);


			for (list<CoordinateWithValue>::iterator it = nextSteps.begin(); it != nextSteps.end(); it++)
			{
				(*it).SetNextList(NextStep(&(*it)));
			};

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
		cout << "cena " << minimalnaCena << " moves " << moveCounter << " id " << i++ << " chesspieces " << temp << endl;
		moveCounter--;
	} while (actualSolution.size() != 0);
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

	FindBestWay();

	printf("schluss");
	return 0;
}