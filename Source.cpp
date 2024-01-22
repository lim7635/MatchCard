#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 57

#define BUFFER_SIZE 10000

typedef struct SelectCard
{
	int x;
	int y;
	const char* shape;
}SelectCard;

// ������ �����ϴ� �Լ�
enum Color
{
	Black,  // 0 = ����
	Blue,   // 1 = �Ķ�
	Green,  // 2 = �ʷ�
	Sky,    // 3 = ����
	Red,    // 4 = ����
	Purple, // 5 = ����
	Yellow, // 6 = ���
	White,  // 7 = ���
	Gray    // 8 = ȸ��
};

enum Button
{
	GameStart,
	HowToPlay
};

// ������ ũ��
int width = 100;
int height = 60;

// ���۸� �����մϴ�.
HANDLE screen[2];
// screen[0] front buffer
// screen[1] back buffer

// HANDLE �ε����� �����ؼ� ���۸� ��ü��Ű�� ����
int screenIndex = 0;

void GotoXY(int x, int y)
{
	// x, y ��ǥ ����
	COORD position = { x, y };

	// Ŀ�� �̵� �Լ�
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// ���۸� �ʱ�ȭ�ϴ� �Լ�
void InitScreen()
{
	CONSOLE_CURSOR_INFO cursor;

	// ������ ���� ������, ���� ������
	COORD size = { width, height };

	// left, top, right, bottom
	SMALL_RECT rect{ 0,0, width - 1, height - 1 };

	// ȭ�� 2���� �����մϴ�.
	// front buffer
	screen[0] = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	// ������ ����� �����ϴ� �Լ�
	SetConsoleScreenBufferSize(screen[0], size);

	SetConsoleWindowInfo(screen[0], TRUE, &rect);

	// back buffer
	screen[1] = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	// ������ ����� �����ϴ� �Լ�
	SetConsoleScreenBufferSize(screen[1], size);

	SetConsoleWindowInfo(screen[1], TRUE, &rect);

	// Ŀ���� Ȱ��ȭ ����
	// false : ���� (0)
	// true  : �� (1)

	cursor.bVisible = false;

	SetConsoleCursorInfo(screen[0], &cursor);
	SetConsoleCursorInfo(screen[1], &cursor);
}

// ���۸� ��ü�ϴ� �Լ�
void FlipScreen()
{
	// ���۴� �ϳ��� Ȱ��ȭ��ų �� �ֽ��ϴ�.
	SetConsoleActiveScreenBuffer(screen[screenIndex]);

	screenIndex = !screenIndex;
}

// ��ü�� ���۸� �����ִ� �Լ�
void ClearScreen()
{
	COORD coord = { 0,0 };

	DWORD dw;

	FillConsoleOutputCharacter
	(
		screen[screenIndex],
		' ',
		width * height,
		coord,
		&dw
	);
}

// ���۸� �����ϴ� �Լ�
void ReleaseScreen()
{
	CloseHandle(screen[0]);
	CloseHandle(screen[1]);
}

// ���� ���۸��� ����ؼ� ����ϴ� �Լ�
void PrintfScreen(int x, int y, const char* string)
{
	COORD cursorPosition = { x, y };

	DWORD dw;

	SetConsoleCursorPosition(screen[screenIndex], cursorPosition);

	WriteFile
	(
		screen[screenIndex],
		string,
		strlen(string),
		&dw,
		NULL
	);
}

void LoadFile(const char* fileName)
{
	// data.txt ������ �����ְ� r(�б�) ���� �����մϴ�.
	FILE* file = fopen(fileName, "r");

	char buffer[BUFFER_SIZE] = { 0, };

	fread(buffer, 1, BUFFER_SIZE, file); // ��ü ������ �о��ִ� �Լ�

	printf("%s", buffer);

	fclose(file);
}

void KeyboardMain(SelectCard* Select)
{
	char key = 0;
	Button button;
	if (_kbhit()) // Ű���� �Է� Ȯ�� �Լ�(true / false)
	{
		key = _getch(); // key �Է��� �޾��ִ� �Լ�
		system("cls");

		if (key == -32)
		{
			key = _getch();
		}

		switch (key)
		{
		case SPACE: switch (button)
					{
					case GameStart: SelectDiff();
						break;
					case HowToPlay: How();
						break;
					default:
						break;
					}
			break;

		case UP: Select->y--;
			break;

		case DOWN: Select->y++;
			break;

		default:
			break;
		}
	}
}

void SelectDiff() // ���̵� ����
{
	printf("���̵� ����\n");
	printf("Easy\tNormal\tHard");

}

void How() // ���� ��� ����
{
	printf("1. ī�� �� ���� ���� �� �ٸ� ī�� �� ���� �����մϴ�.\n    * ó�� ������ ī�带 �ٽ� �����Ѵٸ� ó�� ������ ī�带 �ٽ� ������ �� �ֽ��ϴ�.\n2. �� ���� ī���� ����� ���� ���ٸ� ī��� ������� �ٸ��ٸ� ������� �ʽ��ϴ�.")
}

// ī�� ��ġ
void CreateCard()
{
	strcpy(Card[0], "0123456789");
	strcpy(Card[1], "0123456789");
	strcpy(Card[2], "ABCDEFGHIJ");
	strcpy(Card[3], "ABCDEFGHIJ");
}

void Render()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Card[i][j] == '0')
			{

				printf("  ");
			}
			else if (Card[i][j] == '1')
			{
				printf("��");
			}
			else if (Card[i][j] == '2')
			{
				printf("��");
			}
		}
		printf("\n");
	}
}

int main()
{
	SelectCard Select = { 10, 10, "��" };

	LoadFile(".txt");
	KeyboardMain(&Select);

	while (1)
	{

	}
	return 0;
}