#include<iostream>
#include<fstream>
#include<Windows.h>
#include<conio.h>
#include<ctime>
#include<time.h>
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define UP 4
using namespace std;

struct Position
{
	int ri;
	int ci;
};

struct Snake
{
	Position* Ps;
	char hsym;
	char sym;
	int size;
	int direction;
	int score;
	char LKey, DKey, RKey, UKey;
};

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void InitSnake(Snake*& S, const int noofsnakes)
{
	int r = 0, c = 0;
	for (int i = 0; i < noofsnakes; i++)
	{
		if (i == 0)
		{
			S[i].hsym = 1;
			S[i].sym = -37;
			S[i].direction = UP;
			S[i].score = 0;
			S[i].size = 3;
			S[i].LKey = 'a', S[i].RKey = 'd', S[i].DKey = 's', S[i].UKey = 'w';
			r = 75, c = 4;
			S[i].Ps = new Position[S[i].size];
			for (int j = 0; j < S[i].size; j++)
			{
				S[i].Ps[j].ri = r + j;
				S[i].Ps[j].ci = c;
			}
		}
		if (i == 1)
		{
			S[i].hsym = 6;
			S[i].sym = -37;
			S[i].direction = UP;
			S[i].score = 0;
			S[i].size = 3;
			S[i].LKey = 75, S[i].RKey = 77, S[i].DKey = 80, S[i].UKey = 72;
			r = 75, c = 98;
			S[i].Ps = new Position[S[i].size];
			for (int j = 0; j < S[i].size; j++)
			{
				S[i].Ps[j].ri = r + j;
				S[i].Ps[j].ci = c;
			}
		}
	}
}



void Init(Snake*& S, int& noofsnakes, Position& fpos, char& fsym)
{
	bool check = false;
	do
	{
		if (check)
		{
			cout << "Wrong Input.\n";
		}
		check = true;
		cout << "How Many Players? 1 or 2?\n";
		cin >> noofsnakes;
	} while (noofsnakes != 1 && noofsnakes != 2);
	S = new Snake[noofsnakes];
	InitSnake(S, noofsnakes);
	fpos.ri = 40;
	fpos.ci = 50;
	fsym = '*';
}

void ChangeDirection(Snake& S, const char ch)
{
	if (ch == S.LKey && S.direction != RIGHT)
	{
		S.direction = LEFT;
	}
	if (ch == S.RKey && S.direction != LEFT)
	{
		S.direction = RIGHT;
	}
	if (ch == S.DKey && S.direction != UP)
	{
		S.direction = DOWN;
	}
	if (ch == S.UKey && S.direction != DOWN)
	{
		S.direction = UP;
	}
}

void MoveSnake(Snake* S, const int noofsnakes, int Rows, int Cols)
{
	for (int j = 0; j < noofsnakes; j++)
	{
		for (int i = S[j].size - 1; i - 1 >= 0; i--)
		{
			S[j].Ps[i] = S[j].Ps[i - 1];
		}

		switch (S[j].direction)
		{
		case UP:
			S[j].Ps[0].ri--;
			break;
		case DOWN:
			S[j].Ps[0].ri++;
			break;
		case LEFT:
			S[j].Ps[0].ci--;
			break;
		case RIGHT:
			S[j].Ps[0].ci++;
			break;
		}
	}
}

void DisplaySnake(Snake*& S, const int noofsnakes)
{
	for (int j = 0; j < noofsnakes; j++)
	{
		gotoRowCol(S[j].Ps[0].ri, S[j].Ps[0].ci);
		cout << S[j].hsym;
		for (int i = 1; i < S[j].size; i++)
		{
			gotoRowCol(S[j].Ps[i].ri, S[j].Ps[i].ci);
			cout << S[j].sym;
		}
	}
}

void EraseSnake(Snake*& S, const int noofsnakes)
{
	for (int j = 0; j < noofsnakes; j++)
	{
		for (int i = 0; i < S[j].size; i++)
		{
			gotoRowCol(S[j].Ps[i].ri, S[j].Ps[i].ci);
			cout << " ";
		}
	}
}

void FoodDisplay(const char fsym, Position& fpos)
{
	gotoRowCol(fpos.ri, fpos.ci);
	cout << fsym;
}

bool FoodCapture(Snake*& S, const int noofsnakes, const Position& fpos, int&t)
{
	for (int i = 0; i < noofsnakes; i++)
	{
		if (S[i].Ps[0].ri == fpos.ri && S[i].Ps[0].ci == fpos.ci)
		{
			t = i;
			return true;
		}
	}
	return false;
}

void SnakeExpand(Snake& S)
{
	Position* HA = new Position[S.size + 1];
	for (int i = 0; i < S.size; i++)
	{
		HA[i] = S.Ps[i];
	}
	HA[S.size] = HA[S.size - 1];
	S.size++;
	delete[] S.Ps;
	S.Ps = HA;
}

bool DoesSnakeKill(Snake*& S, const int noofsnakes, int& s)
{
	for (int c = 0; c < noofsnakes; c++)
	{
		for (int i = 1; i < S[c].size; i++)
		{
			if (S[c].Ps[0].ri == S[c].Ps[i].ri && S[c].Ps[0].ci == S[c].Ps[i].ci)
			{
				s = c;
				return true;
			}
		}
	}
	return false;
}

bool DoesSnakeCollide(Snake*& S, const int noofsnakes, int& s)
{
	for (int c = 0; c < noofsnakes; c++)
	{
		for (int j = 1; j < S[c].size; j++)
		{
			if (S[c].Ps[0].ri == S[!c].Ps[j].ri && S[c].Ps[0].ci == S[!c].Ps[j].ci)
			{
				s = c;
				return true;
			}
		}
	}
	return false;
}

bool DoSnakesCollideHeadOn(Snake*& S)
{
	return (S[0].Ps[0].ri == S[1].Ps[0].ri && S[0].Ps[0].ci == S[1].Ps[0].ci);
}

bool DoesSnakeTouchBoundaries(Snake*& S, const int noofsnakes, const int Rows, const int Cols, int& s)
{
	for (int c = 0; c < noofsnakes; c++)
	{
		for (int i = 0; i < Cols; i++)
		{
			if (S[c].Ps[0].ri == 0 && S[c].Ps[0].ci == i)
			{
				s = c;
				return true;
			}
		}
		for (int i = 0; i < Rows; i++)
		{
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == Cols)
			{
				s = c;
				return true;
			}
		}
		for (int i = Cols; i >= 0; i--)
		{
			if (S[c].Ps[0].ri == Rows && S[c].Ps[0].ci == i)
			{
				s = c;
				return true;
			}
		}
		for (int i = Rows; i >= 0; i--)
		{
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == 0)
			{
				s = c;
				return true;
			}
		}
	}
	return false;
}

bool DoesSnakeTouchWalls(Snake*& S, const int noofsnakes, int& s)
{
	for (int c = 0; c < noofsnakes; c++)
	{
		for (int i = 35; i < 65; i++)
		{
			if (S[c].Ps[0].ri == 30 && S[c].Ps[0].ci == i)
			{
				s = c;
				return true;
			}
			if (S[c].Ps[0].ri == 50 && S[c].Ps[0].ci == i)
			{
				s = c;
				return true;
			}
		}
		for (int i = 30; i < 34; i++)
		{
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == 35)
			{
				s = c;
				return true;
			}
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == 65)
			{
				s = c;
				return true;
			}
		}
		for (int i = 50; i > 46; i--)
		{
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == 35)
			{
				s = c;
				return true;
			}
			if (S[c].Ps[0].ri == i && S[c].Ps[0].ci == 65)
			{
				s = c;
				return true;
			}
		}
	}
	return false;
}

bool ValidFood(Snake*& S, const int noofsnakes, const Position fpos)
{
	for (int j = 0; j < noofsnakes; j++)
	{
		for (int i = 0; i < S[j].size; i++)
		{
			if (S[j].Ps[i].ri == fpos.ri && S[j].Ps[i].ci == fpos.ci)
			{
				return false;
			}
		}
	}
	return true;
}

void FoodRegenerate(const char fsym, Position& fpos, const int Rows, const int Cols, Snake*& S, const int noofsnakes)
{
	bool check = true;
	do
	{
		fpos.ri = rand() % Rows;
		if (fpos.ri == 0)
		{
			fpos.ri += 1;
		}
		fpos.ci = rand() % Cols;
		if (fpos.ci == 0)
		{
			fpos.ci += 1;
		}
		if (S[0].score > 50 || (S[1].score > 50 && noofsnakes == 2))
		{
			for (int i = 35; i < 65; i++)
			{
				if ((fpos.ri == 30 && fpos.ci == i) || (fpos.ri == 50 && fpos.ci == i))
				{
					check = false;
				}
			}
			for (int i = 30; i < 34; i++)
			{
				if ((fpos.ri == i && fpos.ci == 35) || (fpos.ri == i && fpos.ci == 65))
				{
					check = false;
				}
			}
			for (int i = 50; i > 46; i--)
			{
				if ((fpos.ri == i && fpos.ci == 35) || (fpos.ri == i && fpos.ci == 65))
				{
					check = false;
				}
			}
		}
	} while (!ValidFood(S, noofsnakes, fpos) || !check);
}

void DisplayScore(Snake*& S, const int noofsnakes)
{
	int c = 3;
	for (int i = 0; i < noofsnakes; i++)
	{
		gotoRowCol(83, c);
		cout << "Score" << i+1 << " = " << S[i].score;
		c = 91;
	}
}

void EraseScore(Snake*& S, const int noofsnakes)
{
	int c = 3;
	for (int i = 0; i < noofsnakes; i++)
	{
		gotoRowCol(83, c);
		cout << "          ";
		c = 91;
	}
}

void UpdateScore(Snake*& S, int& i, const int noofsnakes, const Position& fpos)
{
	if (FoodCapture(S, noofsnakes, fpos, i))
	{
		S[i].score += 5;
		return;
	}
	if (DoesSnakeCollide(S, noofsnakes, i))
	{
		S[i].score -= 3;
		return;
	}
}

void DisplayBoundaries(const int Rows, const int Cols)
{
	char c = -37;
	for (int j = 0; j < Cols; j++)
	{
		gotoRowCol(0, j);
		cout << c;
	}
	for (int j = 0; j < Rows; j++)
	{
		gotoRowCol(j, Cols);
		cout << c;
	}
	for (int j = Cols; j >= 0; j--)
	{
		gotoRowCol(Rows, j);
		cout << c;
	}
	for (int j = Rows; j >= 0; j--)
	{
		gotoRowCol(j, 0);
		cout << c;
	}
}

void DisplayWalls()
{
	char c = -37;
	for (int i = 35; i < 65; i++)
	{
		gotoRowCol(30, i);
		cout << c;
		gotoRowCol(50, i);
		cout << c;
	}
	for (int i = 30; i < 34; i++)
	{
		gotoRowCol(i, 35);
		cout << c;
		gotoRowCol(i, 65);
		cout << c;
	}
	for (int i = 50; i > 46; i--)
	{
		gotoRowCol(i, 35);
		cout << c;
		gotoRowCol(i, 65);
		cout << c;
	}
}


int main()
{
	Snake* S = nullptr;
	Position fpos;
	char fsym;
	int ch;
	int noofsnakes;
	int Rows = 80;
	int Cols = 100;
	time_t starttime = time(0);
	time_t Time = 0;
	cout << "Welcome To Snake Xenzia!!!\n";
	Init(S, noofsnakes, fpos, fsym);
	int choice;
	bool mode, breaker = false;
	cout << "Select A Mode:\n"
		<< "1. Classic Mode\n"
		<< "2. Time Mode\n";
	cin >> choice;
	mode = choice - 1;
	system("cls");
	DisplayBoundaries(Rows, Cols);
	FoodDisplay(fsym, fpos);
	DisplayScore(S, noofsnakes);
	while (true)
	{
		gotoRowCol(83, 45);
		cout << "Game Time: " << Time;
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 224)
			{
				ch = _getch();
				ChangeDirection(S[1], ch);
			}
			else
			{
				ChangeDirection(S[0], ch);
			}
		}
		EraseSnake(S, noofsnakes);
		MoveSnake(S, noofsnakes, Rows, Cols);
		DisplaySnake(S, noofsnakes);
		int i = 2, q = 2; 
		if (FoodCapture(S, noofsnakes, fpos, i))
		{
			EraseScore(S, noofsnakes);
			UpdateScore(S, i, noofsnakes, fpos);
			DisplayScore(S, noofsnakes);
			SnakeExpand(S[i]);
			FoodRegenerate(fsym, fpos, Rows, Cols, S, noofsnakes);
			FoodDisplay(fsym, fpos);
		}
		bool count = true;
		if ((S[0].score > 50 && count) || (S[1].score > 50 && noofsnakes == 2 && count))
		{
			count = false;
			DisplayWalls();
		}
		if (DoesSnakeKill(S, noofsnakes, q))
		{
			gotoRowCol(85, 0);
			if (noofsnakes == 2)
				cout << "Player " << (!q) + 1 << " wins!\n";
			else
				cout << "Game Over!\n";
			break;
		}
		if (DoesSnakeTouchBoundaries(S, noofsnakes, Rows, Cols, q))
		{
			gotoRowCol(85, 0);
			if (noofsnakes == 2)
				cout << "Player " << (!q) + 1 << " wins!\n";
			else
				cout << "Game Over!\n";
			break;
		}
		if ((S[0].score > 50 && count) || (S[1].score > 50 && noofsnakes == 2))
		{
			if (DoesSnakeTouchWalls(S, noofsnakes, q))
			{
				gotoRowCol(85, 0);
				if (noofsnakes == 2)
					cout << "Player " << (!q) + 1 << " wins!\n";
				else
					cout << "Game Over!\n";
				break;
			}
		}
		if (noofsnakes == 2)
		{
			if (DoSnakesCollideHeadOn(S))
			{
				if (S[0].score > S[1].score)
				{
					gotoRowCol(85, 0);
					cout << "Player 1 wins!\n";
					break;
				}
				if (S[0].score < S[1].score)
				{
					gotoRowCol(85, 0);
					cout << "Player 2 wins!\n";
					break;
				}
				gotoRowCol(85, 0);
				cout << "The Game Ends In A Draw.\n";
				break;
			}
			if (DoesSnakeCollide(S, noofsnakes, q))
			{
				EraseScore(S, noofsnakes);
				UpdateScore(S, q, noofsnakes, fpos);
				DisplayScore(S, noofsnakes);
			}
		}
		Sleep(100);
		time_t endtime = time(0);
		Time = endtime - starttime;
		breaker = false;
		if (Time == 30 && mode)
		{
			breaker = true;
			break;
		}
	}
	if (breaker == true)
	{
		if (noofsnakes == 1)
		{
			gotoRowCol(85, 0);
			cout << "Final Score: " << S[0].score << "\n";
		}
		if (noofsnakes == 2)
		{
			if (S[0].score > S[1].score)
			{
				gotoRowCol(85, 0);
				cout << "Player 1 is the Winner!\n";
			}
			if (S[0].score < S[1].score)
			{
				gotoRowCol(85, 0);
				cout << "Player 2 is the Winner!\n";
			}
			if(S[0].score == S[1].score)
			{
				gotoRowCol(85, 0);
				cout << "The Game is Drawn.\n";
			}
		}
	}
	return 0;
}