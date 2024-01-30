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

#define EASY_WIDTH 11
#define EASY_HEIGHT 7
#define NORMAL_WIDTH 17
#define NORMAL_HEIGHT 11
#define HARD_WIDTH 25
#define HARD_HEIGHT 15

char Title[5][39];
char EasyCard[EASY_HEIGHT][EASY_WIDTH];		// Easy 5 X 4
char NormalCard[NORMAL_HEIGHT][NORMAL_WIDTH];	// Normal 8 X 6
char HardCard[HARD_HEIGHT][HARD_WIDTH];				// Hard 12 X 8

// [EASY] �ΰ��� ũ�� : 5 x 4 / ���� ũ�� : 10 x 7
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO

// [NORMAL] �ΰ��� ũ�� : 8 x 6 / ���� ũ�� : 16 x 11
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO
//XXXXXXXXXXXXXXXX
//XOXOXOXOXOXOXOXO

// [HARD] �ΰ��� ũ�� : 12 x 8 / ���� ũ�� : 24 x 15
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

Color color;

void CreateCard(enum Diff diff);
void Render(enum Diff diff);
int Point = 0;
int Check = 0;

void CursorView() // Ŀ�� Ȱ��ȭ ����
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };

	// Ŀ�� ���� (1 ~ 100)
	cursorInfo.dwSize = 1;

	// Ŀ�� Visible TRUE(����) FALSE(����)
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void GotoXY(int x, int y)
{
	// x, y ��ǥ ����
	COORD position = { x, y };

	// Ŀ�� �̵� �Լ�
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void Keyboard(SelectCard * Select)
{
	char key = 0;
	if (_kbhit()) // Ű���� �Է� Ȯ�� �Լ�(true / false)
	{
		key = _getch(); // key �Է��� �޾��ִ� �Լ�
		if (key == -32)
		{
			key = _getch();
		}
		switch (key)
		{
		case SPACE: if (Point == 0 && Select->x == 0) { diff = Easy; CreateCard(Easy); Point = 1; Select->y = 0; } //Ingame(Easy); }
				  else if (Point == 0 && Select->x == 26) { diff = Normal; CreateCard(Normal); Point = 1; Select->x = 0; Select->y = 0; } //Ingame(Normal); }
				  else if (Point == 0 && Select->x == 52) { diff = Hard; CreateCard(Hard); Point = 1; Select->x = 0; Select->y = 0; } //Ingame(Hard); }
				  /*else if (Point == 1 && EasyCard[Select->y][Select->x / 2 + 1] != 'x') { Select->shape = "��"; Check++; Ingame(Easy); }
				  else if (Point == 1 && NormalCard[Select->y][Select->x / 2 + 1] != 'x') { Check++; Ingame(Normal); }
				  else if (Point == 1 && HardCard[Select->y][Select->x / 2 + 1] != 'x') { Check++; Ingame(Hard); }*/

			break;

		case UP: if (Point == 1 && Select->y / 2 - 1 >= 0) { Select->y -= 2; }
			break;

		case DOWN: if (Point == 1 && diff == Easy && Select->y / 2 + 1 < EASY_HEIGHT / 2 + 1) { Select->y += 2; }
				 else if (Point == 1 && diff == Normal && Select->y / 2 + 1 < NORMAL_HEIGHT / 2 + 1) { Select->y += 2; }
				 else if (Point == 1 && diff == Hard && Select->y / 2 + 1 < HARD_HEIGHT / 2 + 1) { Select->y += 2; }
			break;

		case LEFT: if (Point == 0 && Select->x / 2 - 13 >= 0) { Select->x -= 26; }
				 else if (Point == 1 && Select->x / 4 - 1 >= 0) { Select->x -= 4; }
			break;

		case RIGHT: if (Point == 0 && Select->x / 2 + 13 <= 38) { Select->x += 26; }
				  else if (Point == 1 && diff == Easy && Select->x / 4 + 1 < EASY_WIDTH / 2) { Select->x += 4; }
				  else if (Point == 1 && diff == Normal && Select->x / 4 + 1 < NORMAL_WIDTH / 2) { Select->x += 4; }
				  else if (Point == 1 && diff == Hard && Select->x / 4 + 1 < HARD_WIDTH / 2) { Select->x += 4; }
			break;

		default:
			break;
		}
	}
}

// ī�� ��ġ
void CreateCard(enum Diff diff)
{
	srand(time(NULL));
	switch (diff)
	{
	case Easy: {
		int EasyPost[10] = { 0,0,0,0,0,0,0,0,0,0 };

		for (int i = 0; i < EASY_HEIGHT; i++)
		{
			for (int j = 0; j < EASY_WIDTH; j++)
			{
				if (i % 2 != 0)
				{
					while (j < EASY_WIDTH)
					{
						EasyCard[i][j] = 'x';
						j++;
					}
					break;
				}
				
				if (j % 2 == 0)
				{
					EasyCard[i][j] = 'x';
				}
				else
				{
					int Card = rand() % 10;
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
					
					case 8:	if (EasyPost[8] < 2)
					{
						EasyPost[7]++;
						EasyCard[i][j] = '8';
					}
						  else
					{
						j--;
					}
						  break;
					
					case 9:	if (EasyPost[9] < 2)
					{
						EasyPost[7]++;
						EasyCard[i][j] = '9';
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
	}
		break;
	
	case Normal: {
		int NormalPost[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		for (int i = 0; i < NORMAL_HEIGHT; i++)
		{
			for (int j = 0; j < NORMAL_WIDTH; j++)
			{
				if (i % 2 != 0)
				{
					while (j < NORMAL_WIDTH)
					{
						NormalCard[i][j] = 'x';
						j++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					NormalCard[i][j] = 'x';
				}
				else
				{
					int Card = rand() % 24;
					switch(Card)
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

						case 12:	if (NormalPost[12] < 2)
						{
							NormalPost[12]++;
							NormalCard[i][j] = 'C';
						}
							   else
						{
							j--;
						}
							   break;

						case 13:	if (NormalPost[13] < 2)
						{
							NormalPost[13]++;
							NormalCard[i][j] = 'D';
						}
							   else
						{
							j--;
						}
							   break;

						case 14:	if (NormalPost[14] < 2)
						{
							NormalPost[14]++;
							NormalCard[i][j] = 'E';
						}
							   else
						{
							j--;
						}
							   break;

						case 15:	if (NormalPost[15] < 2)
						{
							NormalPost[15]++;
							NormalCard[i][j] = 'F';
						}
							   else
						{
							j--;
						}
							   break;

						case 16:	if (NormalPost[16] < 2)
						{
							NormalPost[16]++;
							NormalCard[i][j] = 'G';
						}
							   else
						{
							j--;
						}
							   break;

						case 17:	if (NormalPost[17] < 2)
						{
							NormalPost[17]++;
							NormalCard[i][j] = 'H';
						}
							   else
						{
							j--;
						}
							   break;

						case 18:	if (NormalPost[18] < 2)
						{
							NormalPost[18]++;
							NormalCard[i][j] = 'I';
						}
							   else
						{
							j--;
						}
							   break;

						case 19:	if (NormalPost[19] < 2)
						{
							NormalPost[19]++;
							NormalCard[i][j] = 'J';
						}
							   else
						{
							j--;
						}
							   break;

						case 20:	if (NormalPost[20] < 2)
						{
							NormalPost[20]++;
							NormalCard[i][j] = 'K';
						}
							   else
						{
							j--;
						}
							   break;

						case 21:	if (NormalPost[21] < 2)
						{
							NormalPost[21]++;
							NormalCard[i][j] = 'L';
						}
							   else
						{
							j--;
						}
							   break;

						case 22:	if (NormalPost[22] < 2)
						{
							NormalPost[22]++;
							NormalCard[i][j] = 'M';
						}
							   else
						{
							j--;
						}
							   break;

						case 23:	if (NormalPost[23] < 2)
						{
							NormalPost[23]++;
							NormalCard[i][j] = 'N';
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
	}
		break;

	case Hard:	{
	int HardPost[48] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	for (int i = 0; i < HARD_HEIGHT; i++)
	{
		for (int j = 0; j < HARD_WIDTH; j++)
		{
			if (i % 2 != 0)
			{
				while (j < HARD_WIDTH)
				{
					HardCard[i][j] = 'x';
					j++;
				}
				break;
			}

			if (j % 2 == 0)
			{
				HardCard[i][j] = 'x';
			}
			else
			{
				int Card = rand() % 48;
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

					case 20:	if (HardPost[20] < 2)
					{
						HardPost[20]++;
						HardCard[i][j] = 'K';
					}
						   else
					{
						j--;
					}
						   break;

					case 21:	if (HardPost[21] < 2)
					{
						HardPost[21]++;
						HardCard[i][j] = 'L';
					}
						   else
					{
						j--;
					}
						   break;

					case 22:	if (HardPost[22] < 2)
					{
						HardPost[22]++;
						HardCard[i][j] = 'M';
					}
						   else
					{
						j--;
					}
						   break;

					case 23:	if (HardPost[23] < 2)
					{
						HardPost[23]++;
						HardCard[i][j] = 'N';
					}
						   else
					{
						j--;
					}
						   break;

					case 24:	if (HardPost[24] < 2)
					{
						HardPost[24]++;
						HardCard[i][j] = 'O';
					}
						   else
					{
						j--;
					}
						   break;

					case 25:	if (HardPost[25] < 2)
					{
						HardPost[25]++;
						HardCard[i][j] = 'P';
					}
						   else
					{
						j--;
					}
						   break;

					case 26:	if (HardPost[26] < 2)
					{
						HardPost[26]++;
						HardCard[i][j] = 'Q';
					}
						   else
					{
						j--;
					}
						   break;

					case 27:	if (HardPost[27] < 2)
					{
						HardPost[27]++;
						HardCard[i][j] = 'R';
					}
						   else
					{
						j--;
					}
						   break;

					case 28:	if (HardPost[28] < 2)
					{
						HardPost[28]++;
						HardCard[i][j] = 'S';
					}
						   else
					{
						j--;
					}
						   break;

					case 29:	if (HardPost[29] < 2)
					{
						HardPost[29]++;
						HardCard[i][j] = 'T';
					}
						   else
					{
						j--;
					}
						   break;

					case 30:	if (HardPost[30] < 2)
					{
						HardPost[30]++;
						HardCard[i][j] = 'U';
					}
						   else
					{
						j--;
					}
						   break;

					case 31:	if (HardPost[31] < 2)
					{
						HardPost[31]++;
						HardCard[i][j] = 'V';
					}
						   else
					{
						j--;
					}
						   break;

					case 32:	if (HardPost[32] < 2)
					{
						HardPost[32]++;
						HardCard[i][j] = 'W';
					}
						   else
					{
						j--;
					}
						   break;

					case 33:	if (HardPost[33] < 2)
					{
						HardPost[33]++;
						HardCard[i][j] = 'X';
					}
						   else
					{
						j--;
					}
						   break;

					case 34:	if (HardPost[34] < 2)
					{
						HardPost[34]++;
						HardCard[i][j] = 'Y';
					}
						   else
					{
						j--;
					}
						   break;

					case 35:	if (HardPost[35] < 2)
					{
						HardPost[35]++;
						HardCard[i][j] = 'Z';
					}
						   else
					{
						j--;
					}
						   break;

					case 36:	if (HardPost[36] < 2)
					{
						HardPost[36]++;
						HardCard[i][j] = 'a';
					}
						   else
					{
						j--;
					}
						   break;

					case 37:	if (HardPost[37] < 2)
					{
						HardPost[37]++;
						HardCard[i][j] = 'b';
					}
						   else
					{
						j--;
					}
						   break;

					case 38:	if (HardPost[38] < 2)
					{
						HardPost[38]++;
						HardCard[i][j] = 'c';
					}
						   else
					{
						j--;
					}
						   break;

					case 39:	if (HardPost[39] < 2)
					{
						HardPost[39]++;
						HardCard[i][j] = 'd';
					}
						   else
					{
						j--;
					}
						   break;

					case 40:	if (HardPost[40] < 2)
					{
						HardPost[40]++;
						HardCard[i][j] = 'e';
					}
						   else
					{
						j--;
					}
						   break;

					case 41:	if (HardPost[41] < 2)
					{
						HardPost[41]++;
						HardCard[i][j] = 'f';
					}
						   else
					{
						j--;
					}
						   break;

					case 42:	if (HardPost[42] < 2)
					{
						HardPost[42]++;
						HardCard[i][j] = 'g';
					}
						   else
					{
						j--;
					}
						   break;

					case 43:	if (HardPost[43] < 2)
					{
						HardPost[43]++;
						HardCard[i][j] = 'h';
					}
						   else
					{
						j--;
					}
						   break;

					case 44:	if (HardPost[44] < 2)
					{
						HardPost[44]++;
						HardCard[i][j] = 'i';
					}
						   else
					{
						j--;
					}
						   break;

					case 45:	if (HardPost[45] < 2)
					{
						HardPost[45]++;
						HardCard[i][j] = 'j';
					}
						   else
					{
						j--;
					}
						   break;

					case 46:	if (HardPost[46] < 2)
					{
						HardPost[46]++;
						HardCard[i][j] = 'k';
					}
						   else
					{
						j--;
					}
						   break;

					case 47:	if (HardPost[47] < 2)
					{
						HardPost[47]++;
						HardCard[i][j] = 'l';
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
	}
		break;

	default:
		break;
	}
}

// ī�� �̹��� ���� �� ���
void CardRender()
{
	switch (diff)
	{
	case Easy: for (int i = 0; i < EASY_HEIGHT; i++)
				{
					for (int j = 0; j < EASY_WIDTH; j++)
					{
						if (EasyCard[i][j] == 'x')
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
							printf("  ");
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						}
						
						if (EasyCard[i][j] == '0')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '1')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '2')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '3')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '4')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '5')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '6')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '7')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '8')
						{
							printf("��");
						}
						else if (EasyCard[i][j] == '9')
						{
							printf("��");
						}
					}
					printf("\n");
				}
		break;

	case Normal: for (int i = 0; i < NORMAL_HEIGHT; i++)
	{
		for (int j = 0; j < NORMAL_WIDTH; j++)
		{
			if (EasyCard[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (NormalCard[i][j] == '0')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '1')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '2')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '3')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '4')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '5')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '6')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '7')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '8')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == '9')
			{
				printf("��");
			}
			else if (NormalCard[i][j] == 'A')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'B')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'C')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'D')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'E')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'F')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'G')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'H')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'I')
			{
				printf("�"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'J')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'K')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'L')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'M')
			{
				printf("��"); // Ư������ ����
			}
			else if (NormalCard[i][j] == 'N')
			{
				printf("��"); // Ư������ ����
			}
		}
		printf("\n");
	}
		break;

	case Hard: for (int i = 0; i < HARD_HEIGHT; i++)
	{
		for (int j = 0; j < HARD_WIDTH; j++)
		{
			if (EasyCard[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (HardCard[i][j] == '0')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '1')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '2')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '3')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '4')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '5')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '6')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '7')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '8')
			{
				printf("��");
			}
			else if (HardCard[i][j] == '9')
			{
				printf("��");
			}
			else if (HardCard[i][j] == 'A')
			{
				printf("��"); // Ư������ ���� A
			}
			else if (HardCard[i][j] == 'B')
			{
				printf("��"); // Ư������ ���� B
			}
			else if (HardCard[i][j] == 'C')
			{
				printf("��"); // Ư������ ���� C
			}
			else if (HardCard[i][j] == 'D')
			{
				printf("��"); // Ư������ ���� D
			}
			else if (HardCard[i][j] == 'E')
			{
				printf("��"); // Ư������ ���� E
			}
			else if (HardCard[i][j] == 'F')
			{
				printf("��"); // Ư������ ���� F
			}
			else if (HardCard[i][j] == 'G')
			{
				printf("��"); // Ư������ ���� G
			}
			else if (HardCard[i][j] == 'H')
			{
				printf("��"); // Ư������ ���� H
			}
			else if (HardCard[i][j] == 'I')
			{
				printf("��"); // Ư������ ���� I
			}
			else if (HardCard[i][j] == 'J')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'K')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'L')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'M')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'N')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'O')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'P')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'Q')
			{
				printf("�"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'R')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'S')
			{
				printf("Ϣ"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'T')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'U')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'V')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'W')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'X')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'Y')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'Z')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'a')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'b')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'c')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'd')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'e')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'f')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'g')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'h')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'i')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'j')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'k')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (HardCard[i][j] == 'l')
			{
				printf("��"); // Ư������ ���� J
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
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
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
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
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
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
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
				printf("��");
			}
			else if (Title[i][j] == '2')
			{
				printf("��");
			}
			else if (Title[i][j] == 'E')
			{
				printf("��");
			}
			else if (Title[i][j] == 'A')
			{
				printf("��");
			}
			else if (Title[i][j] == 'S')
			{
				printf("��");
			}
			else if (Title[i][j] == 'Y')
			{
				printf("��");
			}
			else if (Title[i][j] == 'N')
			{
				printf("��");
			}
			else if (Title[i][j] == 'O')
			{
				printf("��");
			}
			else if (Title[i][j] == 'R')
			{
				printf("��");
			}
			else if (Title[i][j] == 'M')
			{
				printf("��");
			}
			else if (Title[i][j] == 'L')
			{
				printf("��");
			}
			else if (Title[i][j] == 'H')
			{
				printf("��");
			}
			else if (Title[i][j] == 'D')
			{
				printf("��");
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
	SelectCard Select = { 0, 2, "��" };
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
				printf("G O !!");
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