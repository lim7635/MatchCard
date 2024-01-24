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
void CreateCard();
void Render(Diff diff);

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
	HowToPlay,
	InGame
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
	Diff diff;
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
					case GameStart: system("cls");
									SelectDiff();
						break;

					case HowToPlay: system("cls");
									How();
						break;

					case InGame:	system("cls");
									KeyboardInGame(Select);
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

void KeyboardInGame(SelectCard* Select)
{
	Diff diff;
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
		case SPACE: switch (diff)
					{
					case Easy: 
						break;

					case Normal:
						break;

					case Hard:
						break;

					default:
						break;
					}
			break;

		case UP: if (Select->y - 1 >= 0) { Select->y--; }
			   break;

		case DOWN: Select->y++;
			break;

		case LEFT: if (Select->x / 2 - 1 >= 0) { Select->x -= 2; }
				 break;

		case RIGHT: Select->x += 2;
			break;

		default:
			break;
		}
	}
}

void How() // 게임 방법 설명
{
	printf("1. 카드 한 장을 선택 후 다른 카드 한 장을 선택합니다.\n    * 처음 선택한 카드를 다시 선택하면 취소하고 다른 카드를 선택할 수 있습니다.\n2. 두 장의 카드의 모양이 서로 같다면 카드는 사라지고 다르다면 사라지지 않습니다.");
}

// 난이도 선택
void SelectDiff()
{
	Diff diff;
	SelectCard Select;
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
		case SPACE: switch (diff)
					{
					case Easy:	system("cls");
								CreateCard(Easy);
						break;

					case Normal:system("cls");
								CreateCard(Normal);
						break;

					case Hard:	system("cls");
								CreateCard(Hard);
						break;

					default:
						break;
					}
			break;

		case UP: Select.y--;
			break;

		case DOWN: Select.y++;
			break;

		default:
			break;
		}
	}
}

// 카드 배치
void CreateCard(enum Diff diff)
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
										EasyCard[i][j] = '0';
									}
									else
									{
										j--;
									}
								break;

							case 1: if (EasyPost[1] < 2)
									{
										EasyPost[1]++;
										EasyCard[i][j] = '1';
									}
									else
									{
										j--;
									}
								break;

							case 2: if (EasyPost[2] < 2)
									{
										EasyPost[2]++;
										EasyCard[i][j] = '2';
									}
									else
									{
										j--;
									}
								break;

							case 3:	if (EasyPost[3] < 2)
									{
										EasyPost[3]++;
										EasyCard[i][j] = '3';
									}
									else
									{
										j--;
									}
								break;

							case 4:	if (EasyPost[4] < 2)
									{
										EasyPost[4]++;
										EasyCard[i][j] = '4';
									}
									else
									{
										j--;
									}
								break;

							case 5:	if (EasyPost[5] < 2)
									{
										EasyPost[5]++;
										EasyCard[i][j] = '5';
									}
									else
									{
										j--;
									}
								break;

							case 6:	if (EasyPost[6] < 2)
									{
										EasyPost[6]++;
										EasyCard[i][j] = '6';
									}
									else
									{
										j--;
									}
								break;

							case 7:	if (EasyPost[7] < 2)
									{
										EasyPost[7]++;
										EasyCard[i][j] = '7';
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
							   NormalCard[i][j] = '0';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 1: if (NormalPost[1] < 2)
						   {
							   NormalPost[1]++;
							   NormalCard[i][j] = '1';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 2: if (NormalPost[2] < 2)
						   {
							   NormalPost[2]++;
							   NormalCard[i][j] = '2';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 3:	if (NormalPost[3] < 2)
						   {
							   NormalPost[3]++;
							   NormalCard[i][j] = '3';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 4:	if (NormalPost[4] < 2)
						   {
							   NormalPost[4]++;
							   NormalCard[i][j] = '4';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 5:	if (NormalPost[5] < 2)
						   {
							   NormalPost[5]++;
							   NormalCard[i][j] = '5';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 6:	if (NormalPost[6] < 2)
						   {
							   NormalPost[6]++;
							   NormalCard[i][j] = '6';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 7:	if (NormalPost[7] < 2)
						   {
							   NormalPost[7]++;
							   NormalCard[i][j] = '7';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 8:	if (NormalPost[8] < 2)
						   {
							   NormalPost[8]++;
							   NormalCard[i][j] = '8';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 9:	if (NormalPost[9] < 2)
						   {
							   NormalPost[9]++;
							   NormalCard[i][j] = '9';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 10:	if (NormalPost[10] < 2)
						   {
							   NormalPost[10]++;
							   NormalCard[i][j] = 'A';
						   }
								 else
						   {
							   j--;
						   }
								 break;

						   case 11:	if (NormalPost[11] < 2)
						   {
							   NormalPost[11]++;
							   NormalCard[i][j] = 'B';
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

	case Hard:		int HardPost[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

				 for (int i = 0; i < HARD_HEIGHT; i++)
				 {
					 for (int j = 0; j < HARD_WIDTH; j++)
					 {
						 int Card = rand() % 20;
						 switch (Card)
						 {
						 case 0: if (HardPost[0] < 2)
						 {
							 HardPost[0]++;
							 HardCard[i][j] = '0';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 1: if (HardPost[1] < 2)
						 {
							 HardPost[1]++;
							 HardCard[i][j] = '1';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 2: if (HardPost[2] < 2)
						 {
							 HardPost[2]++;
							 HardCard[i][j] = '2';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 3:	if (HardPost[3] < 2)
						 {
							 HardPost[3]++;
							 HardCard[i][j] = '3';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 4:	if (HardPost[4] < 2)
						 {
							 HardPost[4]++;
							 HardCard[i][j] = '4';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 5:	if (HardPost[5] < 2)
						 {
							 HardPost[5]++;
							 HardCard[i][j] = '5';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 6:	if (HardPost[6] < 2)
						 {
							 HardPost[6]++;
							 HardCard[i][j] = '6';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 7:	if (HardPost[7] < 2)
						 {
							 HardPost[7]++;
							 HardCard[i][j] = '7';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 8:	if (HardPost[8] < 2)
						 {
							 HardPost[8]++;
							 HardCard[i][j] = '8';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 9:	if (HardPost[9] < 2)
						 {
							 HardPost[9]++;
							 HardCard[i][j] = '9';
						 }
							   else
						 {
							 j--;
						 }
							   break;

						 case 10:	if (HardPost[10] < 2)
						 {
							 HardPost[10]++;
							 HardCard[i][j] = 'A';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 11:	if (HardPost[11] < 2)
						 {
							 HardPost[11]++;
							 HardCard[i][j] = 'B';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 12:	if (HardPost[12] < 2)
						 {
							 HardPost[12]++;
							 HardCard[i][j] = 'C';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 13:	if (HardPost[13] < 2)
						 {
							 HardPost[13]++;
							 HardCard[i][j] = 'D';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 14:	if (HardPost[14] < 2)
						 {
							 HardPost[14]++;
							 HardCard[i][j] = 'E';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 15:	if (HardPost[15] < 2)
						 {
							 HardPost[15]++;
							 HardCard[i][j] = 'F';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 16:	if (HardPost[16] < 2)
						 {
							 HardPost[16]++;
							 HardCard[i][j] = 'G';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 17:	if (HardPost[17] < 2)
						 {
							 HardPost[17]++;
							 HardCard[i][j] = 'H';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 18:	if (HardPost[18] < 2)
						 {
							 HardPost[18]++;
							 HardCard[i][j] = 'I';
						 }
								else
						 {
							 j--;
						 }
								break;

						 case 19:	if (HardPost[19] < 2)
						 {
							 HardPost[19]++;
							 HardCard[i][j] = 'J';
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

	default:
		break;
	}
}

// 카드 이미지 변경 후 출력
void Render(Diff diff)
{
	switch (diff)
	{
	case Easy: for (int i = 0; i < EASY_NORMAL_HEIGHT; i++)
				{
					for (int j = 0; j < EASY_WIDTH; j++)
					{
						if (EasyCard[i][j] == '0')
						{

							printf("★");
						}
						else if (EasyCard[i][j] == '1')
						{
							printf("●");
						}
						else if (EasyCard[i][j] == '2')
						{
							printf("♣");
						}
						else if (EasyCard[i][j] == '3')
						{
							printf("♥");
						}
						else if (EasyCard[i][j] == '4')
						{
							printf("■");
						}
						else if (EasyCard[i][j] == '5')
						{
							printf("◆");
						}
						else if (EasyCard[i][j] == '6')
						{
							printf("▲");
						}
						else if (EasyCard[i][j] == '7')
						{
							printf("◎");
						}
					}
					printf("\n");
				}
		break;

	case Normal: for (int i = 0; i < EASY_NORMAL_HEIGHT; i++)
	{
		for (int j = 0; j < NORMAL_WIDTH; j++)
		{
			if (NormalCard[i][j] == '0')
			{

				printf("★");
			}
			else if (NormalCard[i][j] == '1')
			{
				printf("●");
			}
			else if (NormalCard[i][j] == '2')
			{
				printf("♣");
			}
			else if (NormalCard[i][j] == '3')
			{
				printf("♥");
			}
			else if (NormalCard[i][j] == '4')
			{
				printf("■");
			}
			else if (NormalCard[i][j] == '5')
			{
				printf("◆");
			}
			else if (NormalCard[i][j] == '6')
			{
				printf("▲");
			}
			else if (NormalCard[i][j] == '7')
			{
				printf("◎");
			}
			else if (NormalCard[i][j] == '8')
			{
				printf("♣");
			}
			else if (NormalCard[i][j] == '9')
			{
				printf("※");
			}
			else if (NormalCard[i][j] == 'A')
			{
				printf("Ａ"); // 특수문자 형태 A
			}
			else if (NormalCard[i][j] == 'B')
			{
				printf("Ｂ"); // 특수문자 형태 B
			}
		}
		printf("\n");
	}
		break;

	case Hard: for (int i = 0; i < HARD_HEIGHT; i++)
	{
		for (int j = 0; j < HARD_WIDTH; j++)
		{
			if (NormalCard[i][j] == '0')
			{

				printf("★");
			}
			else if (NormalCard[i][j] == '1')
			{
				printf("●");
			}
			else if (NormalCard[i][j] == '2')
			{
				printf("♣");
			}
			else if (NormalCard[i][j] == '3')
			{
				printf("♥");
			}
			else if (NormalCard[i][j] == '4')
			{
				printf("■");
			}
			else if (NormalCard[i][j] == '5')
			{
				printf("◆");
			}
			else if (NormalCard[i][j] == '6')
			{
				printf("▲");
			}
			else if (NormalCard[i][j] == '7')
			{
				printf("◎");
			}
			else if (NormalCard[i][j] == '8')
			{
				printf("♣");
			}
			else if (NormalCard[i][j] == '9')
			{
				printf("※");
			}
			else if (NormalCard[i][j] == 'A')
			{
				printf("Ａ"); // 특수문자 형태 A
			}
			else if (NormalCard[i][j] == 'B')
			{
				printf("Ｂ"); // 특수문자 형태 B
			}
			else if (NormalCard[i][j] == 'C')
			{
				printf("Ｃ"); // 특수문자 형태 C
			}
			else if (NormalCard[i][j] == 'D')
			{
				printf("Ｄ"); // 특수문자 형태 D
			}
			else if (NormalCard[i][j] == 'E')
			{
				printf("Ｅ"); // 특수문자 형태 E
			}
			else if (NormalCard[i][j] == 'F')
			{
				printf("Ｆ"); // 특수문자 형태 F
			}
			else if (NormalCard[i][j] == 'G')
			{
				printf("Ｇ"); // 특수문자 형태 G
			}
			else if (NormalCard[i][j] == 'H')
			{
				printf("Ｈ"); // 특수문자 형태 H
			}
			else if (NormalCard[i][j] == 'I')
			{
				printf("Ｉ"); // 특수문자 형태 I
			}
			else if (NormalCard[i][j] == 'J')
			{
				printf("Ｊ"); // 특수문자 형태 J
			}
		}
		printf("\n");
	}
		break;

	default:
		break;
	}
	
}

int main()
{
	SelectCard Select = { 10, 10, "□" };
	Diff diff;
	// LoadFile(".txt");
	KeyboardMain(&Select);
	CreateCard();
	while (1)
	{
		Render(diff);

		KeyboardInGame(&Select);

		GotoXY(Select.x, Select.y);
	}
	return 0;
}