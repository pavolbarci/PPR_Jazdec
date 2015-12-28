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
#include <mpi.h>

#define LENGTH 10000
#define CHECK_MSG_AMOUNT 100
#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

using namespace std;

#pragma region
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
#pragma endregion Classes

#pragma region
Configuration m_configuration;
list<CoordinateWithValue> m_coordinatesWithValue;
int m_lowerLimit;
int m_upperLimit;
list<CoordinateWithValue> m_bestSolution;
CoordinateWithValue** m_chessBoard;
list<CoordinateWithValue> actualSolution;
int MY_RANK;
int PROCESSORS;
#pragma endregion Parameters

#pragma region
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

Coordinate RemoveChessPiece(list<Coordinate>* chessPieces, CoordinateWithValue horse)
{
	Coordinate co;
	co.SetCoordinate(0, 0);
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
#pragma endregion Methods

string PackActualSolution(list<CoordinateWithValue>* sendSolution)
{
	ostringstream oss;

	for (auto coordinate : *sendSolution)
	{
		oss << coordinate.GetCoordinate().GetX() << "," << coordinate.GetCoordinate().GetY() << "-";

		for (auto next : coordinate.GetNextCoordinates())
		{
			oss << next.GetCoordinate().GetX() << "," << next.GetCoordinate().GetY() << "," << next.GetValue() << "/";

			for (auto nextNext : next.GetNextCoordinates())
			{
				oss << nextNext.GetCoordinate().GetX() << "," << nextNext.GetCoordinate().GetY() << "," << nextNext.GetValue() << ";";
			};

			oss << "*";
		};

		oss << "|";
	};

	return oss.str();
}

vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

void SetCoor(Coordinate* coordinate, string indexes)
{
	coordinate->SetCoordinate(atoi((indexes.substr(0, 1).c_str())), atoi((indexes.substr(2, 1).c_str())));
}

list<CoordinateWithValue> UnpackActualSolution(string packedString)
{
	vector<string> steps = split(packedString, '|');

	list<CoordinateWithValue> actual;


	for (auto step : steps)
	{
		list<CoordinateWithValue> nextList;
		list<CoordinateWithValue> nextNextList;

		vector<string> ACTNEXT = split(step, '-');
		CoordinateWithValue coor;
		Coordinate coo;

		SetCoor(&coo, ACTNEXT[0]);
		coor.SetCoordinate(coo);

		if (ACTNEXT.size() > 1)
		{
			vector<string> nextSteps = split(ACTNEXT[1], '*');


			for (auto next : nextSteps)
			{
				vector<string> temp = split(next, '/');


				CoordinateWithValue nextCoor;
				Coordinate nextCoo;

				SetCoor(&nextCoo, temp[0]);
				nextCoor.SetCoordinate(nextCoo);
				nextCoor.SetValue(atoi((temp[0].substr(4, 1).c_str())));

				vector<string> nextNextSteps = split(temp[1], ';');
				for (auto nextNext : nextNextSteps)
				{
					CoordinateWithValue nextNextCoor;
					Coordinate nextNextCoo;

					SetCoor(&nextNextCoo, nextNext);
					nextNextCoor.SetValue(atoi((nextNext.substr(4, 1).c_str())));
					nextNextCoor.SetCoordinate(nextNextCoo);

					nextNextList.push_back(nextNextCoor);
				};
				nextCoor.SetNextList(nextNextList);
				nextNextList.clear();
				nextList.push_back(nextCoor);
			};

			coor.SetNextList(nextList);
			nextList.clear();

		}

		actual.push_back(coor);
	};

	return actual;
}

int len;

void SendActualSolution(int processNumber)
{
	//copy of actual solution
	list<CoordinateWithValue> send = actualSolution;
	//temp obsahuje vsetky mozne skoky a teda sa bude delit
	list<CoordinateWithValue> temp = actualSolution.back().GetNextCoordinates();
	int size = temp.size();

	//list ktory bude novy pre send.back.next
	list<CoordinateWithValue> sendNext;


	for (int i = 0; i < size / 2; i++)
	{
		//do noveho listu pridame prvy zo vsetkych a zo vsetkych prvy odstranime
		sendNext.push_back(temp.front());
		temp.pop_front();
	}

	//na posielaci solution nastavime novy list a na stary zvysok
	send.back().SetNextList(sendNext);
	actualSolution.back().SetNextList(temp);


	string sendMessage = PackActualSolution(&send);

	char message[10000];
	strcpy(message, sendMessage.c_str());
	MPI_Request request;
	MPI_Isend(message, strlen(message) + 1, MPI_CHAR, processNumber, MSG_WORK_SENT, MPI_COMM_WORLD, &request);
}

void CheckForMessage()
{
	int flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	char message[LENGTH];

	if (flag)
	{
		//prisla zprava, je treba ji obslouzit
		//v promenne status je tag (status.MPI_TAG), cislo odesilatele (status.MPI_SOURCE)
		//a pripadne cislo chyby (status.MPI_ERROR)
		if (status.MPI_TAG == MSG_WORK_REQUEST)
		{ // zadost o praci, prijmout a dopovedet
		  // zaslat rozdeleny zasobnik a nebo odmitnuti MSG_WORK_NOWORK

		}
		else if (status.MPI_TAG == MSG_WORK_SENT)
		{
			// prisel rozdeleny zasobnik, prijmout
			// deserializovat a spustit vypocet
			MPI_Recv(message, LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			string packedActual(message);
			UnpackActualSolution(packedActual);
			cout << "process two has received and unpacked solution" << endl;
		}
		else if (status.MPI_TAG == MSG_WORK_NOWORK)
		{// odmitnuti zadosti o praci
							  // zkusit jiny proces
							  // a nebo se prepnout do pasivniho stavu a cekat na token

		}
		else if (status.MPI_TAG == MSG_TOKEN)
		{//ukoncovaci token, prijmout a nasledne preposlat
						// - bily nebo cerny v zavislosti na stavu procesu

		}
		else if (status.MPI_TAG == MSG_FINISH)
		{
			//konec vypoctu - proces 0 pomoci tokenu zjistil, ze jiz nikdo nema praci
			//a rozeslal zpravu ukoncujici vypocet
			//mam-li reseni, odeslu procesu 0
			//nasledne ukoncim spoji cinnost
			//jestlize se meri cas, nezapomen zavolat koncovou barieru MPI_Barrier (MPI_COMM_WORLD)
		}
		else
		{

			//chyba("neznamy typ zpravy"); 
		}
	}

}

void DoStep(list<Coordinate> *chessPiecesPositions, int* moveCounter)
{
	int minimalnaCena = m_upperLimit;
	list<CoordinateWithValue> nextSteps;


	(*moveCounter)++;

	while (actualSolution.size() > 0 && actualSolution.back().GetNextCoordinates().size() == 0)
	{
		StepBack(chessPiecesPositions);
		(*moveCounter)--;
	}
	if (actualSolution.size() == 0)
	{
		return;
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
	RemoveChessPiece(chessPiecesPositions, actualSolution.back());

}

list<Coordinate> FirstStep()
{

	list<CoordinateWithValue> firstSteps = NextStep(NULL);
	list<CoordinateWithValue> nextSteps = firstSteps;

	CoordinateWithValue konik;
	konik.SetCoordinate(m_configuration.GetHorseCoordinate());

	for (list<CoordinateWithValue>::iterator it = nextSteps.begin(); it != nextSteps.end(); it++)
	{
		(*it).SetNextList(NextStep(&(*it)));
	};
	nextSteps.sort([](CoordinateWithValue & a, CoordinateWithValue & b) { return a.GetValue() > b.GetValue(); });

	konik.SetNextList(nextSteps);
	actualSolution.push_back(konik);
	m_chessBoard[konik.GetCoordinate().GetX()][konik.GetCoordinate().GetY()].visited = 1;

	list<Coordinate> chessPiecesPositions = m_configuration.GetChessPiecesCoordinates();

	return chessPiecesPositions;
}

list<Coordinate> GetChessPieces()
{
	list<Coordinate> chessPiecesPositions = m_configuration.GetChessPiecesCoordinates();

	for (auto coor : actualSolution)
	{

		m_chessBoard[coor.GetCoordinate().GetX()][coor.GetCoordinate().GetY()].visited = 1;
		if (coor.GetCoordinate().IsEqual(chessPiecesPositions.front()))
		{
			m_chessBoard[coor.GetCoordinate().GetX()][coor.GetCoordinate().GetY()].SetValue(0);
			chessPiecesPositions.pop_front();
		}
	};

	return chessPiecesPositions;
}

int blabla = 0;
void ExpandSolution(list<Coordinate> *chessPiecesPositions, int* moveCounter, int* minimalnaCena)
{
	int x = (*minimalnaCena) - actualSolution.size();
	int chessPieces = (*chessPiecesPositions).size();
	if (x > 2)
	{
		x = 0;
		chessPieces = 5;
	}
	while ((((*moveCounter) < (*minimalnaCena)) && (*chessPiecesPositions).size() != 0)
		&& ((x + 2) < chessPieces))
	{
		MPI_Bcast(minimalnaCena, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (actualSolution.size() > (*minimalnaCena))
		{
			int actSize = actualSolution.size();
			for (int i = 0; i <= (actSize - (*minimalnaCena)); i++)
			{
				(*moveCounter)--;
				StepBack(chessPiecesPositions);
				if (m_bestSolution.size() > (*minimalnaCena))
				{
					m_bestSolution.pop_back();
				}
			}
		}

		DoStep(chessPiecesPositions, moveCounter);
	}

	if ((*chessPiecesPositions).size() == 0 && (*moveCounter) < (*minimalnaCena))
	{
		MPI_Bcast(minimalnaCena, 1, MPI_INT, 0, MPI_COMM_WORLD);
		(*minimalnaCena) = (*moveCounter);
		m_bestSolution = actualSolution;
		cout << "process " << MY_RANK << " found solution" << endl;

		MPI_Request request;
		if ((*minimalnaCena) == m_configuration.GetNumberOfChessPieces())
		{
			for (int i = 0; i < PROCESSORS; i++)
			{
				//if (i != MY_RANK)
				{
					MPI_Isend(minimalnaCena, 1, MPI_INT, i, MSG_FINISH, MPI_COMM_WORLD, &request);
				}
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
	}
	//cout << "cyklus " << blabla << endl;
	blabla++;
	StepBack(chessPiecesPositions);
	(*moveCounter)--;
}

void SendFirstSolution(int processNumber)
{
	//copy of actual solution
	list<CoordinateWithValue> send1 = actualSolution;
	list<CoordinateWithValue> send2 = actualSolution;
	list<CoordinateWithValue> send3 = actualSolution;
	//temp obsahuje vsetky mozne skoky a teda sa bude delit
	list<CoordinateWithValue> temp = actualSolution.back().GetNextCoordinates();
	int size = temp.size();

	//list ktory bude novy pre send.back.next
	list<CoordinateWithValue> sendNext1;
	list<CoordinateWithValue> sendNext2;
	list<CoordinateWithValue> sendNext3;

	sendNext1.push_back(temp.front());
	temp.pop_front();
	sendNext2.push_back(temp.front());
	temp.pop_front();
	sendNext3.push_back(temp.front());
	temp.pop_front();

	for (int i = 0; i < size; i++)
	{
		//do noveho listu pridame prvy zo vsetkych a zo vsetkych prvy odstranime
	}

	//na posielaci solution nastavime novy list a na stary zvysok
	send1.back().SetNextList(sendNext1);
	send2.back().SetNextList(sendNext2);
	send3.back().SetNextList(sendNext3);
	actualSolution.back().SetNextList(temp);


	string sendMessage = PackActualSolution(&send1);

	char message[10000];
	strcpy(message, sendMessage.c_str());
	MPI_Request request;
	MPI_Isend(message, strlen(message) + 1, MPI_CHAR, 1, MSG_WORK_SENT, MPI_COMM_WORLD, &request);

	sendMessage = PackActualSolution(&send2);

	char message1[10000];
	strcpy(message1, sendMessage.c_str());
	MPI_Isend(message1, strlen(message1) + 1, MPI_CHAR, 2, MSG_WORK_SENT, MPI_COMM_WORLD, &request);

	sendMessage = PackActualSolution(&send3);

	char message2[10000];
	strcpy(message2, sendMessage.c_str());
	MPI_Isend(message2, strlen(message2) + 1, MPI_CHAR, 3, MSG_WORK_SENT, MPI_COMM_WORLD, &request);
}

#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	string errorMessage = InicializeConfiguration();

	if (!errorMessage.empty())
	{
		return 0;
	}

	int minimalnaCena = m_upperLimit;
	int flag;
	MPI_Status status;
	int dest;
	int tag = 1;
	int source;
	char message[1000];
	int m = 100;

	int token = 1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MY_RANK);
	MPI_Comm_size(MPI_COMM_WORLD, &PROCESSORS);


	int moveCounter = 0;
	int citac = 0;
	list<Coordinate> chessPiecesPositions;
	double t1, t2;
	t1 = MPI_Wtime();

	if (MY_RANK == 0) {

		moveCounter = 0;
		citac = 0;
		MPI_Request request;
		chessPiecesPositions = FirstStep();
		SendFirstSolution(1);
		cout << "sent first work" << endl;
		//SendActualSolution(1);
		int noWorkCounter = 0;
		int noWork = 0;
		int workReq = 1;
		while (actualSolution.size() != 0 || noWork)
		{
			citac++;
			if ((citac % CHECK_MSG_AMOUNT) == 0)
			{
				int flag;
				MPI_Status status;
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
				char message[LENGTH];

				if (flag)
				{
					if (status.MPI_TAG == MSG_WORK_REQUEST)
					{
						noWorkCounter = 0;
						int source;
						MPI_Recv(&source, LENGTH, MPI_INT, status.MPI_SOURCE, MSG_WORK_REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						cout << "no work from process " << source << endl;
						if (actualSolution.back().GetNextCoordinates().size() < 2)
						{
							MPI_Isend(&noWork, 1, MPI_INT, source, MSG_WORK_NOWORK, MPI_COMM_WORLD, &request);
							cout << "process 0 sent no work" << endl;
						}
						else
						{
							SendActualSolution(source);
							cout << "process 0 sent work to" << status.MPI_SOURCE << endl;
						}
					}
					else if (status.MPI_TAG == MSG_WORK_SENT)
					{
						MPI_Recv(message, LENGTH, MPI_CHAR, workReq, MSG_WORK_SENT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						cout << "process 0 accepted work" << endl;
						noWork = 0;
						string packedActual(message);
						actualSolution = UnpackActualSolution(packedActual);

						chessPiecesPositions = GetChessPieces();
						moveCounter = actualSolution.size();
					}
					else if (status.MPI_TAG == MSG_WORK_NOWORK)
					{
						if (noWorkCounter == (PROCESSORS - 1))
						{

							noWorkCounter = 0;
							cout << " sleeping " << endl;
							this_thread::sleep_for(chrono::milliseconds(100));
							MPI_Send(&MY_RANK, 1, MPI_INT, 1, MSG_WORK_REQUEST, MPI_COMM_WORLD);

							//token = 1;
							//cout << " sleeping " << endl;
							//this_thread::sleep_for(chrono::milliseconds(100));
							//cout << "aftersleep " << MY_RANK << " sent token;" << endl;



//							MPI_Isend(&token, 1, MPI_INT, 1, MSG_TOKEN, MPI_COMM_WORLD, &request);
							//cout << "aftersleep " << MY_RANK << " sent token;" << endl;

							/*t2 = MPI_Wtime();
							printf("%d: Elapsed time is %f.\n", MY_RANK, t2 - t1);
							MPI_Finalize();
							exit(0);*/
						}
						else
						{
							noWorkCounter++;
							cout << "process 0 no work" << endl;
							workReq = status.MPI_SOURCE + 1;
							if (workReq >= PROCESSORS)
							{
								workReq = 1;
							}

							MPI_Send(&MY_RANK, 1, MPI_INT, workReq, MSG_WORK_REQUEST, MPI_COMM_WORLD);
						}
					}
					else if (status.MPI_TAG == MSG_TOKEN)
					{
						MPI_Recv(&token, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						cout << "process " << MY_RANK << " receive token;" << endl;

						if (token == 0)
						{
							if (actualSolution.size() == 0)
							{
								token = 1;
							}
							else
							{
								token = 0;
							}
							MPI_Isend(&token, 1, MPI_INT, 1, MSG_TOKEN, MPI_COMM_WORLD, &request);
							cout << "process " << MY_RANK << " sent token from token;" << endl;
						}
						else if (token == 1)
						{
							cout << "process " << MY_RANK << " finish work;" << endl;
							for (int i = 0; i < PROCESSORS; i++)
							{
								MPI_Isend(&minimalnaCena, 1, MPI_INT, i, MSG_FINISH, MPI_COMM_WORLD, &request);

							}
						}
						else
						{
						}
					}
					else if (status.MPI_TAG == MSG_FINISH)
					{
						MPI_Barrier(MPI_COMM_WORLD);
						t2 = MPI_Wtime();
						printf("%d: Elapsed time is %f.\n", MY_RANK, t2 - t1);
						MPI_Finalize();
						exit(0);
					}
					else
					{
						//chyba("neznamy typ zpravy"); 
					}

				}
			}
			ExpandSolution(&chessPiecesPositions, &moveCounter, &minimalnaCena);
		}
	}
	else {
		moveCounter = 0;
		citac = 0;
		//SendActualSolution();
		MPI_Recv(message, LENGTH, MPI_CHAR, 0, MSG_WORK_SENT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		string packedActual(message);
		cout << "process received work " << MY_RANK << endl;
		actualSolution = UnpackActualSolution(packedActual);
		chessPiecesPositions = GetChessPieces();
		moveCounter = actualSolution.size();
		int noWorkCounter = 0;
		int noWork = 0;
		int workReq = 0;
		MPI_Request request;

		while (actualSolution.size() != 0 || noWork)
		{
			citac++;
			if ((citac % CHECK_MSG_AMOUNT) == 0)
			{
				int flag;
				MPI_Status status;
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
				char message[LENGTH];

				if (flag)
				{
					if (status.MPI_TAG == MSG_WORK_REQUEST)
					{
						int source;
						MPI_Recv(&source, LENGTH, MPI_INT, status.MPI_SOURCE, MSG_WORK_REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						cout << "no work from process " << source << endl;
						if (actualSolution.back().GetNextCoordinates().size() < 2)
						{
							MPI_Isend(&noWork, 1, MPI_INT, source, MSG_WORK_NOWORK, MPI_COMM_WORLD, &request);
						}
						else
						{
							SendActualSolution(status.MPI_SOURCE);
						}
						cout << "process " << MY_RANK << " sent work;" << endl;
					}
					else if (status.MPI_TAG == MSG_WORK_SENT)
					{
						noWorkCounter = 0;
						MPI_Recv(message, LENGTH, MPI_CHAR, workReq, MSG_WORK_SENT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						cout << "process " << MY_RANK << " received work" << endl;
						noWork = 0;
						string packedActual(message);
						actualSolution = UnpackActualSolution(packedActual);
						chessPiecesPositions = GetChessPieces();
						moveCounter = actualSolution.size();
					}
					else if (status.MPI_TAG == MSG_WORK_NOWORK)
					{
						if (noWorkCounter == (PROCESSORS - 1))
						{
							noWorkCounter = 0;
							cout << " sleeping " << endl;
							this_thread::sleep_for(chrono::milliseconds(100));
							MPI_Send(&MY_RANK, 1, MPI_INT, 0, MSG_WORK_REQUEST, MPI_COMM_WORLD);


						}
						else
						{
							noWorkCounter++;
							workReq = status.MPI_SOURCE + 1;
							if (workReq == MY_RANK)
							{
								workReq++;
							}
							if (workReq >= PROCESSORS)
							{
								workReq = 0;
							}

							MPI_Send(&MY_RANK, 1, MPI_INT, workReq, MSG_WORK_REQUEST, MPI_COMM_WORLD);
							cout << "process " << MY_RANK << " no work" << endl;
						}
					}
					else if (status.MPI_TAG == MSG_TOKEN)
					{
						MPI_Recv(&token, 1, MPI_INT, MPI_ANY_SOURCE, MSG_TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						if(token == 0 && actualSolution.size() != 0)
						{
							token == 1;
						}
						if ((MY_RANK + 1) == PROCESSORS)
						{

							MPI_Isend(&token, 1, MPI_INT, 0, MSG_TOKEN, MPI_COMM_WORLD, &request);

						}
						else
						{
							MPI_Isend(&token, 1, MPI_INT, MY_RANK + 1, MSG_TOKEN, MPI_COMM_WORLD, &request);
						}

/*

						if (actualSolution.size() == 0)
						{
							token = 1;
						}
						else
						{
							token = 0;
						}*/
						
						cout << "process " << MY_RANK << " sent token from token;" << endl;

					}
					else if (status.MPI_TAG == MSG_FINISH)
					{
						MPI_Barrier(MPI_COMM_WORLD);
						t2 = MPI_Wtime();
						printf("%d: Elapsed time is %f.\n", MY_RANK, t2 - t1);
						MPI_Finalize();	
						exit(0);
					}
					else
					{
						//chyba("neznamy typ zpravy"); 
					}
				}
			}
			ExpandSolution(&chessPiecesPositions, &moveCounter, &minimalnaCena);
		}
	}

	t2 = MPI_Wtime();
	printf("%d: Elapsed time is %f.\n", MY_RANK, t2 - t1);
	cout << "Count is " << citac << endl;
	MPI_Finalize();
	return 0;
}