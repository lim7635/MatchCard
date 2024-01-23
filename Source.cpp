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

// 색상을 결정하는 함수
enum Color
{
	Black,  // 0 = 검정
	Blue,   // 1 = 파랑
	Green,  // 2 = 초록
	Sky,    // 3 = 옥색
	Red,    // 4 = 빨강
	Purple, // 5 = 자주
	Yellow, // 6 = 노랑
	White,  // 7 = 흰색
	Gray    // 8 = 회색
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

// 버퍼의 크기
int width = 100;
int height = 60;

// 버퍼를 생성합니다.
HANDLE screen[2];
// screen[0] front buffer
// screen[1] back buffer

// HANDLE 인덱스에 접근해서 버퍼를 교체시키는 변수
int screenIndex = 0;

void GotoXY(int x, int y)
{
	// x, y 좌표 설정
	COORD position = { x, y };

	// 커서 이동 함수
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// 버퍼를 초기화하는 함수
void InitScreen()
{
	CONSOLE_CURSOR_INFO cursor;

	// 버퍼의 가로 사이즈, 세로 사이즈
	COORD size = { width, height };

	// left, top, right, bottom
	SMALL_RECT rect{ 0,0, width - 1, height - 1 };

	// 화면 2개를 생성합니다.
	// front buffer
	screen[0] = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	// 버퍼의 사이즈를 설정하는 함수
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

	// 버퍼의 사이즈를 설정하는 함수
	SetConsoleScreenBufferSize(screen[1], size);

	SetConsoleWindowInfo(screen[1], TRUE, &rect);

	// 커서의 활성화 여부
	// false : 거짓 (0)
	// true  : 참 (1)

	cursor.bVisible = false;

	SetConsoleCursorInfo(screen[0], &cursor);
	SetConsoleCursorInfo(screen[1], &cursor);
}

// 버퍼를 교체하는 함수
void FlipScreen()
{
	// 버퍼는 하나만 활성화시킬 수 있습니다.
	SetConsoleActiveScreenBuffer(screen[screenIndex]);

	screenIndex = !screenIndex;
}

// 교체된 버퍼를 지워주는 함수
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

// 버퍼를 해제하는 함수
void ReleaseScreen()
{
	CloseHandle(screen[0]);
	CloseHandle(screen[1]);
}

// 더블 버퍼링을 사용해서 출력하는 함수
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
//	// data.txt 파일을 열어주고 r(읽기) 모드로 설정합니다.
//	FILE* file = fopen(fileName, "r");
//
//	char buffer[BUFFER_SIZE] = { 0, };
//
//	fread(buffer, 1, BUFFER_SIZE, file); // 전체 파일을 읽어주는 함수
//
//	printf("%s", buffer);
//
//	fclose(file);
//}

void KeyboardMain(SelectCard* Select)
{
	Button button;
	if (_kbhit()) // 키보드 입력 확인 함수(true / false)
	{
		key = _getch(); // key 입력을 받아주는 함수
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

void SelectDiff(SelectCard* Select) // 난이도 선택
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

void How() // 게임 방법 설명
{
	printf("1. 카드 한 장을 선택 후 다른 카드 한 장을 선택합니다.\n    * 처음 선택한 카드를 다시 선택하면 취소하고 다른 카드를 선택할 수 있습니다.\n2. 두 장의 카드의 모양이 서로 같다면 카드는 사라지고 다르다면 사라지지 않습니다.");
}

// 카드 배치
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
				printf("▥");
			}
			else if (Card[i][j] == '2')
			{
				printf("◎");
			}
		}
		printf("\n");
	}
}

int main()
{
	SelectCard Select = { 10, 10, "□" };
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