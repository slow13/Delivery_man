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

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void warningMessage(const int height)
{
	gotoxy(0, height + 1);
	cout << "Can't Move\n";
}

void successMessage(const int height)
{
	gotoxy(0, height + 1);
	cout << "SUCCESS!!\n";
}

void reachGoal(int &x, int &y, const int height, int &success)
{
	setcolor(14, 0);
	successMessage(height);
	x = 0;
	y = 0;
	success++;
}

void randomPosition(int &x, int &y, const int width, const int height, int(*position)[2], int &count)
{
	bool loop = true;
	while (loop)
	{
		srand((unsigned int)time(NULL));
		x = (rand() % (width - 4)) + 2;
		y = (rand() % (height - 4)) + 2;
		position[count][0] = x;
		position[count][1] = y;

		if (count)
		{
			for (int i = 0; i < count; i++)
			{
				if (position[i][0] == position[count][0] && position[i][1] == position[count][1])
				{
					loop = true;
					break;
				}
				else
					loop = false;
			}
		}
		else loop = false;
	}

	count++;
	gotoxy(x, y);
}

bool checkWall(int &x, int &y, const int width, const int height)
{
	if (x == 0 || y == 0 || x == width - 1 || y == height - 1) return true;
	else return false;
}

bool checkGoal(int &x, int &y, int(*position)[2])
{
	for (int i = NUMBER_OF_BOX; i < NUMBER_OF_BOX + NUMBER_OF_GOAL; i++)
	{
		if (position[i][0] == x && position[i][1] == y)
		{
			return true;
			break;
		}
		else if (i + 1 == NUMBER_OF_BOX + NUMBER_OF_GOAL) return false;
	}
}

bool checkBox(int &x, int &y, int(*position)[2], int arrow, const int width, const int height, int &success)
{
	for (int i = 0; i < NUMBER_OF_BOX; i++)
	{
		if (position[i][0] == x && position[i][1] == y)
		{
			switch (arrow)
			{
			case LEFT: 
				position[i][0]--;
				if (checkWall(position[i][0], position[i][1], width, height))
				{
					warningMessage(height);
					position[i][0]++;
					x++;
					return false;
				}
				else if (checkGoal(position[i][0], position[i][1], position))
				{
					reachGoal(position[i][0], position[i][1], height, success);
					return true;
				}
				else return true;
				break;
			case RIGHT:
				position[i][0]++;
				if (checkWall(position[i][0], position[i][1], width, height))
				{
					warningMessage(height);
					position[i][0]--;
					x--;
					return false;
				}
				else if (checkGoal(position[i][0], position[i][1], position))
				{
					reachGoal(position[i][0], position[i][1], height, success);
					return true;
				}
				else return true;
				break;
			case UP:
				position[i][1]--;
				if (checkWall(position[i][0], position[i][1], width, height))
				{
					warningMessage(height);
					position[i][1]++;
					y++;
					return false;
				}
				else if (checkGoal(position[i][0], position[i][1], position))
				{
					reachGoal(position[i][0], position[i][1], height, success);
					return true;
				}
				else return true;
				break;
			case DOWN:
				position[i][1]++;
				if (checkWall(position[i][0], position[i][1], width, height))
				{
					warningMessage(height);
					position[i][1]--;
					y--;
					return false;
				}
				else if (checkGoal(position[i][0], position[i][1], position))
				{
					reachGoal(position[i][0], position[i][1], height, success);
					return true;
				}
				else return true;
				break;
			default:
				return false;
			}
			break;
		}
		else if (i + 1 == NUMBER_OF_BOX) return false;
	}
}

void clearLine(const int height)
{
	gotoxy(0, height + 1);
	cout << "                               ";
}

void update(int &x, int &y, const int width, const int height, int(*position)[2], int &success)
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
				else if (checkGoal(x, y, position))
				{
					warningMessage(height);
					gotoxy(++x, y);
					break;
				}
				else if (checkBox(x, y, position, LEFT, width, height, success))
				{
					gotoxy(--x, y);
					cout << 'o';
					setcolor(15, 0);
					x += 2;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(--x, y);
					cout << 'p';
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
				else if (checkGoal(x, y, position))
				{
					warningMessage(height);
					gotoxy(--x, y);
					break;
				}
				else if (checkBox(x, y, position, RIGHT, width, height, success))
				{
					gotoxy(++x, y);
					cout << 'o';
					setcolor(15, 0);
					x -= 2;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(++x, y);
					cout << 'p';
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
				else if (checkGoal(x, y, position))
				{
					warningMessage(height);
					gotoxy(x, ++y);
					break;
				}
				else if (checkBox(x, y, position, UP, width, height, success))
				{
					gotoxy(x, --y);
					cout << 'o';
					setcolor(15, 0);
					y += 2;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(x, --y);
					cout << 'p';
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
				else if (checkGoal(x, y, position))
				{
					warningMessage(height);
					gotoxy(x, --y);
					break;
				}
				else if (checkBox(x, y, position, DOWN, width, height, success))
				{
					gotoxy(x, ++y);
					cout << 'o';
					setcolor(15, 0);
					y -= 2;
					gotoxy(x, y);
					cout << ' ';
					gotoxy(x, ++y);
					cout << 'p';
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

	if (success == NUMBER_OF_BOX)
	{
		gotoxy(0, height);
		cout << "CONGRATULATION!! YOU WIN!!\n";
	}
}

int main()
{
	int scanWidth, scanHeight;
	int *width = &scanWidth;
	int *height = &scanHeight;
	bool check_size = true;
	while (check_size)
	{
		cout << "Enter width & height ( >= 4) : ";
		cin >> scanWidth >> scanHeight;
		if (scanWidth < 4 || scanHeight < 4)
			cout << "Wrong size. please re-enter size\n";
		else check_size = false;
	}
	cout << "LOADING......";
	Sleep(1000);
	system("cls");
	drawMap(*width, *height);

	int playerX, playerY;
	int position[10][2] = { 0 };
	int count = 0;
	int success = 0;

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
	{
		update(playerX, playerY, *width, *height, position, success);
		if (success == NUMBER_OF_BOX) break;
	}

	return 0;
}