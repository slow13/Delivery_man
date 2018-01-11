#include <iostream>
#include <cstdlib>
#include <string>
#include <Windows.h>
#include <ctime>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define NUMBER_OF_BOX 2
#define NUMBER_OF_GOAL 2

using namespace std;

void drawMap(int width, int height)
{
	for (int i = 0; i < width; i++)
		cout << '#';
	cout << '\n';
	for (int i = 0; i < height - 2; i++)
	{
		cout << '#';
		for (int j = 0; j < width - 2; j++)
			cout << ' ';
		cout << "#\n";
	}
	for (int i = 0; i < width; i++)
		cout << '#';
	cout << '\n';
}

void gotoxy(short x, short y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void randomPosition(short &x, short &y, const int width, const int height, int(*position)[2], int &count)
{
	bool loop = true;
	while (loop)
	{
		srand((unsigned int)time(NULL));
		x = (rand() % (width - 2)) + 1;
		y = (rand() % (height - 2)) + 2;
		position[count][0] = x;
		position[count][1] = y;

		for (int i = -1; i < count; i++)
		{
			if (position[i][0] == position[count][0] && position[i][0] == position[count][1])
			{
				loop = true;
				break;
			}
			else
				loop = false;
		}
	}

	count++;
	gotoxy(x, y);
}

bool checkWall(short &x, short &y, const int width, const int height)
{
	if (x == 0 || y == 1 || x == width - 1 || y == height) return true;
	else return false;
}

bool checkBox(short &x, short &y, int(*position)[2], int arrow)
{
	for (int i = 0; i < NUMBER_OF_BOX; i++)
	{
		if (position[i][0] == x && position[i][1] == y)
		{
			switch (arrow)
			{
			case LEFT: position[i][0]--;
			case RIGHT: position[i][0]++;
			case UP: position[i][1]--;
			case DOWN: position[i][1]++;
			}
			return true;
			break;
		}
		else return false;
	}
}

bool checkGoal(short &x, short &y, int(*position)[2])
{
	for (int i = NUMBER_OF_BOX; i < NUMBER_OF_BOX + NUMBER_OF_GOAL; i++)
	{
		if (position[i][0] == x && position[i][1] == y)
		{
			return true;
			break;
		}
		else return false;
	}
}

void warningMessage(const int height)
{
	gotoxy(0, height + 1);
	cout << "Can't Move\n";
}

void clearLine(const int height)
{
	gotoxy(0, height + 1);
	cout << "                               ";
}

void update(short &x, short &y, const int width, const int height, int(*position)[2])
{
	char c;

	if (!_kbhit())
	{
		c = _getch();
		if (c == -32)
		{
			c = _getch();
			clearLine(height);
			gotoxy(x, y);
			switch (c)
			{
			case LEFT:
				--x;
				if (checkWall(x, y, width, height))
				{
					warningMessage(height);
					gotoxy(++x, y);
					break;
				}
				else if (checkBox(x, y, position, LEFT))
				{
					gotoxy(--x, y);
					cout << 'o';
					x += 2;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(--x, y);
					cout << 'p';
					break;
				}
				else if (checkGoal(x, y, position))
				{
					warningMessage(height);
					gotoxy(++x, y);
					break;
				}
				else
				{
					++x;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(--x, y);
					cout << 'p';
					break;
				}
			case RIGHT:
				++x;
				if (checkWall(x, y, width, height))
				{
					warningMessage(height);
					gotoxy(--x, y);
					break;
				}
				else
				{
					--x;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(++x, y);
					cout << 'p';
					break;
				}
			case UP:
				--y;
				if (checkWall(x, y, width, height))
				{
					warningMessage(height);
					gotoxy(x, ++y);
					break;
				}
				else
				{
					++y;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(x, --y);
					cout << 'p';
					break;
				}
			case DOWN:
				++y;
				if (checkWall(x, y, width, height))
				{
					warningMessage(height);
					gotoxy(x, --y);
					break;
				}
				else
				{
					--y;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(x, ++y);
					cout << 'p';
					break;
				}
			default: break;
			}
		}
	}
}

int main()
{
	int scanWidth, scanHeight;
	int *width = &scanWidth;
	int *height = &scanHeight;
	cout << "Enter width & height : ";
	cin >> scanWidth >> scanHeight;
	drawMap(*width, *height);

	short playerX, playerY;
	int position[10][2] = { 0 };
	int count = 0;

	for (int i = 0; i < NUMBER_OF_BOX; i++)
	{
		randomPosition(playerX, playerY, *width, *height, position, count);
		cout << 'o';
		Sleep(1000);
	}

	for (int i = 0; i < NUMBER_OF_GOAL; i++)
	{
		randomPosition(playerX, playerY, *width, *height, position, count);
		cout << '.';
		Sleep(1000);
	}

	randomPosition(playerX, playerY, *width, *height, position, count);
	cout << 'p';

	while (true)
		update(playerX, playerY, *width, *height, position);

	gotoxy(0, *height + 1);

	return 0;
}