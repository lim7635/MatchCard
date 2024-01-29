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
#define SPACE 32

#define BUFFER_SIZE 10000

#define EASY_WIDTH 4
#define EASY_NORMAL_HEIGHT 4
#define NORMAL_WIDTH 6
#define HARD_WIDTH 8
#define HARD_HEIGHT 5

char Title[5][39];
char EasyCard[EASY_NORMAL_HEIGHT][EASY_WIDTH];		// Easy 4 X 4
char NormalCard[EASY_NORMAL_HEIGHT][NORMAL_WIDTH];	// Normal 6 X 4
char HardCard[HARD_HEIGHT][HARD_WIDTH];				// Hard 8 X 5

// [EASY] 인게임 크기 : 5 x 4 / 실제 크기 : 10 x 8
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//
// [NORMAL] 인게임 크기 : 8 x 5 / 실제 크기 : 16 x 10
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//
// [HARD] 인게임 크기 : 12 x 8 / 실제 크기 : 24 x 16
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXOXOXOXOXO

typedef struct SelectCard
{
	int x;
	int y;
	const char* shape;
}SelectCard;

enum Diff
{
	Easy,
	Normal,
	Hard
};

Diff diff;

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

Color color;

void CreateCard(enum Diff diff);
void Render(enum Diff diff);
int Point = 0;

void CursorView() // 커서 활성화 여부
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };

	// 커서 굵기 (1 ~ 100)
	cursorInfo.dwSize = 1;

	// 커서 Visible TRUE(보임) FALSE(숨김)
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void GotoXY(int x, int y)
{
	// x, y 좌표 설정
	COORD position = { x, y };

	// 커서 이동 함수
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void Keyboard(SelectCard * Select)
{
	char key = 0;
	if (_kbhit()) // 키보드 입력 확인 함수(true / false)
	{
		key = _getch(); // key 입력을 받아주는 함수
		if (key == -32)
		{
			key = _getch();
		}
		switch (key)
		{
		case SPACE: if (Point == 0 && Select->x == 0) { diff = Easy; CreateCard(Easy); Point = 1; Select->y = 0; Select->shape = "■";} //Ingame(Easy); }
				  else if (Point == 0 && Select->x == 26) { diff = Normal; CreateCard(Normal); Point = 1; Select->x = 0; Select->y = 0; Select->shape = "■";} //Ingame(Normal); }
				  else if (Point == 0 && Select->x == 52) { diff = Hard; CreateCard(Hard); Point = 1; Select->x = 0; Select->y = 0; Select->shape = "■";} //Ingame(Hard); }
				  /*else if (Point == 1 && EasyCard[Select->y][Select->x] != NULL)
				  else if (Point == 1 && NormalCard[Select->y][Select->x] != NULL)
				  else if (Point == 1 && HardCard[Select->y][Select->x] != NULL)*/

			break;

		case UP: if (Point == 1 && Select->y - 1 >= 0) { Select->y--; }
			break;

		case DOWN: if (Point == 1 && diff == Easy && Select->y + 1 < EASY_NORMAL_HEIGHT) { Select->y++; }
				 else if (Point == 1 && diff == Normal && Select->y + 1 < EASY_NORMAL_HEIGHT) { Select->y++; }
				 else if (Point == 1 && diff == Hard && Select->y + 1 < HARD_HEIGHT) { Select->y++; }
			break;

		case LEFT: if (Point == 0 && Select->x / 2 - 13 >= 0) { Select->x -= 26; }
				 else if (Point == 1 && Select->x / 2 - 1 >= 0) { Select->x -= 2; }
			break;

		case RIGHT: if (Point == 0 && Select->x / 2 + 13 <= 38) { Select->x += 26; }
				  else if (Point == 1 && diff == Easy && Select->x / 2 + 1 < EASY_WIDTH) { Select->x += 2; }
				  else if (Point == 1 && diff == Normal && Select->x / 2 + 1 < NORMAL_WIDTH) { Select->x += 2; }
				  else if (Point == 1 && diff == Hard && Select->x / 2 + 1 < HARD_WIDTH) { Select->x += 2; }
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
	case Easy: {
		int EasyPost[8] = { 0,0,0,0,0,0,0,0 };

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
	}
		break;
	
	case Normal: {
		int NormalPost[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

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
	}
		break;

	case Hard:	{
	int HardPost[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

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
	}
		break;

	default:
		break;
	}
}

// 카드 이미지 변경 후 출력
void CardRender()
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
			if (HardCard[i][j] == '0')
			{

				printf("★");
			}
			else if (HardCard[i][j] == '1')
			{
				printf("●");
			}
			else if (HardCard[i][j] == '2')
			{
				printf("♣");
			}
			else if (HardCard[i][j] == '3')
			{
				printf("♥");
			}
			else if (HardCard[i][j] == '4')
			{
				printf("■");
			}
			else if (HardCard[i][j] == '5')
			{
				printf("◆");
			}
			else if (HardCard[i][j] == '6')
			{
				printf("▲");
			}
			else if (HardCard[i][j] == '7')
			{
				printf("◎");
			}
			else if (HardCard[i][j] == '8')
			{
				printf("♣");
			}
			else if (HardCard[i][j] == '9')
			{
				printf("※");
			}
			else if (HardCard[i][j] == 'A')
			{
				printf("Ａ"); // 특수문자 형태 A
			}
			else if (HardCard[i][j] == 'B')
			{
				printf("Ｂ"); // 특수문자 형태 B
			}
			else if (HardCard[i][j] == 'C')
			{
				printf("Ｃ"); // 특수문자 형태 C
			}
			else if (HardCard[i][j] == 'D')
			{
				printf("Ｄ"); // 특수문자 형태 D
			}
			else if (HardCard[i][j] == 'E')
			{
				printf("Ｅ"); // 특수문자 형태 E
			}
			else if (HardCard[i][j] == 'F')
			{
				printf("Ｆ"); // 특수문자 형태 F
			}
			else if (HardCard[i][j] == 'G')
			{
				printf("Ｇ"); // 특수문자 형태 G
			}
			else if (HardCard[i][j] == 'H')
			{
				printf("Ｈ"); // 특수문자 형태 H
			}
			else if (HardCard[i][j] == 'I')
			{
				printf("Ｉ"); // 특수문자 형태 I
			}
			else if (HardCard[i][j] == 'J')
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

void CreateTitle()
{
	strcpy(Title[0], "01111111111000111111111100011111111110");
	strcpy(Title[1], "02000000002000200000000200020000000020");
	strcpy(Title[2], "0200EASY00200020NORMAL02000200HARD0020");
	strcpy(Title[3], "02000000002000200000000200020000000020");
	strcpy(Title[4], "01111111111000111111111100011111111110");
}

void TitleRender()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 39; j++)
		{
			if (j > 0 && j <= 10)
			{
				if (Title[i][j] == '1' || Title[i][j] == '2')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				}
				else if (Title[i][j] == 'E' || Title[i][j] == 'A' || Title[i][j] == 'S' || Title[i][j] == 'Y')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (j > 13 && j <= 23)
			{
				if (Title[i][j] == '1' || Title[i][j] == '2')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
				}
				else if (Title[i][j] == 'N' || Title[i][j] == 'O' || Title[i][j] == 'R' || Title[i][j] == 'M' || Title[i][j] == 'A' || Title[i][j] == 'L')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (j > 26 && j <= 36)
			{
				if (Title[i][j] == '1' || Title[i][j] == '2')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				}
				else if (Title[i][j] == 'H' || Title[i][j] == 'A' || Title[i][j] == 'R' || Title[i][j] == 'D')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (Title[i][j] == '0')
			{
				printf("  ");
			}
			else if (Title[i][j] == '1')
			{
				printf("―");
			}
			else if (Title[i][j] == '2')
			{
				printf("｜");
			}
			else if (Title[i][j] == 'E')
			{
				printf("Ｅ");
			}
			else if (Title[i][j] == 'A')
			{
				printf("Ａ");
			}
			else if (Title[i][j] == 'S')
			{
				printf("Ｓ");
			}
			else if (Title[i][j] == 'Y')
			{
				printf("Ｙ");
			}
			else if (Title[i][j] == 'N')
			{
				printf("Ｎ");
			}
			else if (Title[i][j] == 'O')
			{
				printf("Ｏ");
			}
			else if (Title[i][j] == 'R')
			{
				printf("Ｒ");
			}
			else if (Title[i][j] == 'M')
			{
				printf("Ｍ");
			}
			else if (Title[i][j] == 'L')
			{
				printf("Ｌ");
			}
			else if (Title[i][j] == 'H')
			{
				printf("Ｈ");
			}
			else if (Title[i][j] == 'D')
			{
				printf("Ｄ");
			}
		}
		printf("\n");
	}
}

void Ingame(enum Diff diff)
{
	switch (diff)
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
}

int main()
{
	CursorView();
	SelectCard Select = { 0, 2, "☞" };
	CreateTitle();
	while (1)
	{
		while (Point == 0)
		{
			TitleRender();
			Keyboard(&Select);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);

			Sleep(100);
			system("cls");
		}

		for (int i = 3; i >= 0; i--)
		{
			if (i >= 1)
			{
				printf("%d", i);
				Sleep(1000);
				system("cls");
			}
			else
			{
				printf("G O");
				Sleep(500);
				system("cls");
			}
		}

		while (Point == 1 || Point == 2)
		{
			CardRender();
			Keyboard(&Select);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);

			Sleep(100);
			system("cls");
		}
	}

	return 0;
}