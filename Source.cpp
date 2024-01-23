#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 57

#define BUFFER_SIZE 10000

#define EASY_WIDTH 4
#define EASY_NORMAL_HEIGHT 4
#define NORMAL_WIDTH 6
#define HARD_WIDTH 8
#define HARD_HEIGHT 5

char EasyCard[EASY_NORMAL_HEIGHT][EASY_WIDTH];		// Easy 4 X 4
char NormalCard[EASY_NORMAL_HEIGHT][NORMAL_WIDTH];	// Normal 6 X 4
char HardCard[HARD_HEIGHT][HARD_WIDTH];				// Hard 8 X 5

char key = 0;
void How();
void Postion(Diff diff);

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

enum Diff
{
	Easy,
	Normal,
	Hard
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

//void LoadFile(const char* fileName)
//{
//	// data.txt ������ �����ְ� r(�б�) ���� �����մϴ�.
//	FILE* file = fopen(fileName, "r");
//
//	char buffer[BUFFER_SIZE] = { 0, };
//
//	fread(buffer, 1, BUFFER_SIZE, file); // ��ü ������ �о��ִ� �Լ�
//
//	printf("%s", buffer);
//
//	fclose(file);
//}

void KeyboardMain(SelectCard* Select)
{
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
					case GameStart: SelectDiff(Select);
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

void SelectDiff(SelectCard* Select) // ���̵� ����
{
	Diff diff;
	switch (key)
	{
	case SPACE: CreateCard(diff);
		break;

	default:
		break;
	}
}

void How() // ���� ��� ����
{
	printf("1. ī�� �� ���� ���� �� �ٸ� ī�� �� ���� �����մϴ�.\n    * ó�� ������ ī�带 �ٽ� �����ϸ� ����ϰ� �ٸ� ī�带 ������ �� �ֽ��ϴ�.\n2. �� ���� ī���� ����� ���� ���ٸ� ī��� ������� �ٸ��ٸ� ������� �ʽ��ϴ�.");
}

// ī�� ��ġ
void CreateCard(enum Diff diff)
{
	switch (diff)
	{
	case Easy: Postion(diff);
		break;
		
	case Normal: Postion(diff);
		break;

	case Hard: Postion(diff);
		break;

	default:
		break;
	}
}

void Position(Diff diff)
{
	srand(time(NULL));
	switch (diff)
	{
	case Easy:		int EasyPost[8] = { 0,0,0,0,0,0,0,0 };

					for (int i = 0; i < EASY_NORMAL_HEIGHT; i++)
					{
						for (int j = 0; j < EASY_WIDTH; j++)
						{
							int Card = rand() % 8;
							switch (Card)
							{
							case 0: if (EasyPost[0] < 2)
									{
										EasyPost[0]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 1: if (EasyPost[1] < 2)
									{
										EasyPost[1]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 2: if (EasyPost[2] < 2)
									{
										EasyPost[2]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 3:	if (EasyPost[3] < 2)
									{
										EasyPost[3]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 4:	if (EasyPost[4] < 2)
									{
										EasyPost[4]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 5:	if (EasyPost[5] < 2)
									{
										EasyPost[5]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 6:	if (EasyPost[6] < 2)
									{
										EasyPost[6]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							case 7:	if (EasyPost[7] < 2)
									{
										EasyPost[7]++;
										EasyCard[i][j] = Card;
									}
									else
									{
										j--;
									}
								break;

							default:
								break;

							}
						}
					}
		break;

	case Normal:	int NormalPost[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

				   for (int i = 0; i < EASY_NORMAL_HEIGHT; i++)
				   {
					   for (int j = 0; j < NORMAL_WIDTH; j++)
					   {
						   int Card = rand() % 12;
						   switch (Card)
						   {
						   case 0: if (NormalPost[0] < 2)
						   {
							   NormalPost[0]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 1: if (NormalPost[1] < 2)
						   {
							   NormalPost[1]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 2: if (NormalPost[2] < 2)
						   {
							   NormalPost[2]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 3:	if (NormalPost[3] < 2)
						   {
							   NormalPost[3]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 4:	if (NormalPost[4] < 2)
						   {
							   NormalPost[4]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 5:	if (NormalPost[5] < 2)
						   {
							   NormalPost[5]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 6:	if (NormalPost[6] < 2)
						   {
							   NormalPost[6]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 7:	if (NormalPost[7] < 2)
						   {
							   NormalPost[7]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 8:	if (NormalPost[8] < 2)
						   {
							   NormalPost[8]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 9:	if (NormalPost[9] < 2)
						   {
							   NormalPost[9]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 10:	if (NormalPost[10] < 2)
						   {
							   NormalPost[10]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 11:	if (NormalPost[11] < 2)
						   {
							   NormalPost[11]++;
							   NormalCard[i][j] = Card;
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   default:
							   break;

						   }
					   }
				   }
			break;

	case Hard:
		break;

	default:
		break;
	}
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
	Diff diff;
	// LoadFile(".txt");
	KeyboardMain(&Select);
	CreateCard(diff);
	while (1)
	{
		Render();
	}
	return 0;
}