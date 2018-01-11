#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

using namespace std;

enum maze_object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_PLAYER,
	OBJ_PLAYER_ON_GOAL,

	OBJ_UNKNOWN,
};

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

bool check_clear()
{
	return true;
}

void draw_object(int width, int height, int arr_width[], int arr_height[], int number, int count, char a)
{
	for (int i = number; i < number + count; i++)
	{
		arr_width[i] = rand() % (width - 4) + 2;
		arr_height[i] = rand() % (height - 4) + 2;
		for (int j = 0; j < i; j++)
		{
			if (arr_width[j] == arr_width[i] && arr_height[j] == arr_height[i]) i--;
			else
			{
				gotoxy(arr_width[i], arr_height[i]);
				cout << a;
			}
		}
	}
}

void init_map(int width, int height, int arr_width[], int arr_height[])
{
	int temp_width = width - 2;
	int temp_height = 0;
	while (temp_height < height)
	{
		if (temp_height == 0 || temp_height == height - 1)
		{
			for (int i = 0; i < width; i++)
				cout << '#';
		}
		else
		{
			cout << '#';
			while (temp_width--) cout << ' ';
			cout << '#';

			temp_width = width - 2;
		}
		cout << '\n';
		temp_height++;
	}

	draw_object(width, height, arr_width, arr_height, 0, 2, 'o');
	draw_object(width, height, arr_width, arr_height, 2, 2, '.');
	draw_object(width, height, arr_width, arr_height, 4, 1, 'p');
}

void update(int *arr_width, int *arr_height)
{
	fflush(stdin);
	int key;
	while (check_clear)
	{
		key = getch();
		if (key == 224)
		{
			key = getch();
			switch (key)
			{
			case 72: // up
				gotoxy(arr_width[4], arr_height[4]);
				cout << ' ';
				gotoxy(arr_width[4], arr_height[4]--);
				cout << 'p';
				break;
			case 75: // left
				gotoxy(arr_width[4], arr_height[4]);
				cout << ' ';
				gotoxy(arr_width[4]--, arr_height[4]);
				cout << 'p';
				break;
			case 77: // right
				gotoxy(arr_width[4], arr_height[4]);
				cout << ' ';
				gotoxy(arr_width[4]++, arr_height[4]);
				cout << 'p';
				break;
			case 80: // down
				gotoxy(arr_width[4], arr_height[4]);
				cout << ' ';
				gotoxy(arr_width[4], arr_height[4]++);
				cout << 'p';
				break;
			default:
				break;
			}
		}
	}
}

int main()
{
	srand(time(NULL));
	const int width = rand() % 20 + 4;
	const int height = rand() % 20 + 4;

	int arr_width[10], arr_height[10];
	init_map(width, height, arr_width, arr_height);
	update(arr_width, arr_height);

	if(check_clear)	gotoxy(0, height);
	return 0;
}