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
#define EASY_HEIGHT 8
#define NORMAL_WIDTH 17
#define NORMAL_HEIGHT 12
#define HARD_WIDTH 25
#define HARD_HEIGHT 16

char Title[5][39];								// 2���� ���ڿ�
char CardEasy[EASY_HEIGHT][EASY_WIDTH];			// Easy 5 X 4
char CardNormal[NORMAL_HEIGHT][NORMAL_WIDTH];	// Normal 8 X 6
char CardHard[HARD_HEIGHT][HARD_WIDTH];			// Hard 12 X 8

// [EASY] �ΰ��� ũ�� : 5 x 4 / ���� ũ�� : 10 x 8

// X = ��ĭ
// O = ī�� ��ġ

//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX
//XOXOXOXOXO
//XXXXXXXXXX

// [NORMAL] �ΰ��� ũ�� : 8 x 6 / ���� ũ�� : 16 x 12
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
//XXXXXXXXXXXXXXXX

// [HARD] �ΰ��� ũ�� : 12 x 8 / ���� ũ�� : 24 x 16
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
//XXXXXXXXXXXXXXXXXXXXXXXX

typedef struct SelectCard
{
	int x;
	int y;
	const char* shape;
}SelectCard;

typedef struct CurrentCard
{
	int x;
	int y;
	const char* text;
	char show;
}CurrentCard;

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

void CreateCard(enum Diff diff);
void Ingame(enum Diff diff);
char ShowCard(enum Diff diff, CurrentCard* Current);

int Point = 0; // Scene ������ Ȯ���ϴ� ����
int Check = 0; // ī�� ���� ���θ� Ȯ���ϴ� ����
char Memory[7] = "zzzzzz"; // [0] = ù��°�� ������ ������ ��� / [1] = ù��° y��ǥ / [2] = ù��° x��ǥ
						   // [3] = �ι�°�� ������ ������ ��� / [4] = �ι�° y��ǥ / [5] = �ι�° x��ǥ

int MatchEasy = 10; // Easy ���̵����� ������ϴ� ¦�� ������ Ȯ���ϴ� ����
int MatchNormal = 24; // Normal ���̵����� ������ϴ� ¦�� ������ Ȯ���ϴ� ����
int MatchHard = 48; // Hard ���̵����� ������ϴ� ¦�� ������ Ȯ���ϴ� ����

// Ŀ�� Ȱ��ȭ ���� �Լ�
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };

	// Ŀ�� ���� (1 ~ 100)
	cursorInfo.dwSize = 1;

	// Ŀ�� Visible TRUE(����) FALSE(����)
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// x, y, ��ǥ ���� �� Ŀ�� �̵� �Լ�
void GotoXY(int x, int y)
{
	COORD position = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// Ű���� �Լ�
void Keyboard(SelectCard* Select, CurrentCard * Current)
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
		case SPACE: if (Point == 0 && Select->x == 0) { diff = Easy; CreateCard(Easy); Point++; Select->y = 0; }
				  else if (Point == 0 && Select->x == 26) { diff = Normal; CreateCard(Normal); Point++; Select->x = 0; Select->y = 0; }
				  else if (Point == 0 && Select->x == 52) { diff = Hard; CreateCard(Hard); Point++; Select->x = 0; Select->y = 0; }
				  else if (Point == 1 && Check == 0 && diff == Easy && CardEasy[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardEasy[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Point == 1 && Check == 0 && diff == Normal && CardNormal[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardNormal[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Point == 1 && Check == 0 && diff == Hard && CardHard[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardHard[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Point == 1 && Check == 1 && diff == Easy && CardEasy[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardEasy[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Easy); }
				  else if (Point == 1 && Check == 1 && diff == Normal && CardNormal[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardNormal[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Normal); }
				  else if (Point == 1 && Check == 1 && diff == Hard && CardHard[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardHard[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Hard); }

			break;

		case UP: if (Point == 1 && Select->y / 2 - 1 >= 0) { Select->y -= 2; }
			break;

		case DOWN: if (Point == 1 && diff == Easy && Select->y / 2 + 1 < EASY_HEIGHT / 2) { Select->y += 2; }
				 else if (Point == 1 && diff == Normal && Select->y / 2 + 1 < NORMAL_HEIGHT / 2) { Select->y += 2; }
				 else if (Point == 1 && diff == Hard && Select->y / 2 + 1 < HARD_HEIGHT / 2) { Select->y += 2; }
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

// ī�� ��ġ �Լ�
void CreateCard(enum Diff diff)
{
	srand(time(NULL));
	switch (diff)
	{
	case Easy: 
		int EasyPost[10];
		for (int i = 0; i < 10; i++)
		{
			EasyPost[i] = 0;
		}

		for (int i = 0; i < EASY_HEIGHT; i++)
		{
			for (int j = 0; j < EASY_WIDTH; j++)
			{
				if (i % 2 != 0)
				{
					while (j < EASY_WIDTH)
					{
						CardEasy[i][j] = 'x';
						j++;
					}
					break;
				}
				
				if (j % 2 == 0)
				{
					CardEasy[i][j] = 'x';
				}
				else
				{
					int Card = rand() % 10;
					switch (Card)
					{
						case 0: if (EasyPost[0] < 2)
						{
							EasyPost[0]++;
							CardEasy[i][j] = '0';
						}
							  else
						{
							j--;
						}
							  break;

						case 1: if (EasyPost[1] < 2)
						{
							EasyPost[1]++;
							CardEasy[i][j] = '1';
						}
							  else
						{
							j--;
						}
							  break;

						case 2: if (EasyPost[2] < 2)
						{
							EasyPost[2]++;
							CardEasy[i][j] = '2';
						}
							  else
						{
							j--;
						}
							  break;

						case 3:	if (EasyPost[3] < 2)
						{
							EasyPost[3]++;
							CardEasy[i][j] = '3';
						}
							  else
						{
							j--;
						}
							  break;

						case 4:	if (EasyPost[4] < 2)
						{
							EasyPost[4]++;
							CardEasy[i][j] = '4';
						}
							  else
						{
							j--;
						}
							  break;

						case 5:	if (EasyPost[5] < 2)
						{
							EasyPost[5]++;
							CardEasy[i][j] = '5';
						}
							  else
						{
							j--;
						}
							  break;

						case 6:	if (EasyPost[6] < 2)
						{
							EasyPost[6]++;
							CardEasy[i][j] = '6';
						}
							  else
						{
							j--;
						}
							  break;

						case 7:	if (EasyPost[7] < 2)
						{
							EasyPost[7]++;
							CardEasy[i][j] = '7';
						}
							  else
						{
							j--;
						}
							  break;
						
						case 8:	if (EasyPost[8] < 2)
						{
							EasyPost[8]++;
							CardEasy[i][j] = '8';
						}
							  else
						{
							j--;
						}
							  break;
						
						case 9:	if (EasyPost[9] < 2)
						{
							EasyPost[9]++;
							CardEasy[i][j] = '9';
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
	
	case Normal: 
		int NormalPost[24];
		for (int i = 0; i < 24; i++)
		{
			NormalPost[i] = 0;
		}

		for (int i = 0; i < NORMAL_HEIGHT; i++)
		{
			for (int j = 0; j < NORMAL_WIDTH; j++)
			{
				if (i % 2 != 0)
				{
					while (j < NORMAL_WIDTH)
					{
						CardNormal[i][j] = 'x';
						j++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					CardNormal[i][j] = 'x';
				}
				else
				{
					int Card = rand() % 24;
					switch(Card)
					{
						case 0: if (NormalPost[0] < 2)
						{
							NormalPost[0]++;
							CardNormal[i][j] = '0';
						}
							  else
						{
							j--;
						}
							  break;

						case 1: if (NormalPost[1] < 2)
						{
							NormalPost[1]++;
							CardNormal[i][j] = '1';
						}
							  else
						{
							j--;
						}
							  break;

						case 2: if (NormalPost[2] < 2)
						{
							NormalPost[2]++;
							CardNormal[i][j] = '2';
						}
							  else
						{
							j--;
						}
							  break;

						case 3:	if (NormalPost[3] < 2)
						{
							NormalPost[3]++;
							CardNormal[i][j] = '3';
						}
							  else
						{
							j--;
						}
							  break;

						case 4:	if (NormalPost[4] < 2)
						{
							NormalPost[4]++;
							CardNormal[i][j] = '4';
						}
							  else
						{
							j--;
						}
							  break;

						case 5:	if (NormalPost[5] < 2)
						{
							NormalPost[5]++;
							CardNormal[i][j] = '5';
						}
							  else
						{
							j--;
						}
							  break;

						case 6:	if (NormalPost[6] < 2)
						{
							NormalPost[6]++;
							CardNormal[i][j] = '6';
						}
							  else
						{
							j--;
						}
							  break;

						case 7:	if (NormalPost[7] < 2)
						{
							NormalPost[7]++;
							CardNormal[i][j] = '7';
						}
							  else
						{
							j--;
						}
							  break;

						case 8:	if (NormalPost[8] < 2)
						{
							NormalPost[8]++;
							CardNormal[i][j] = '8';
						}
							  else
						{
							j--;
						}
							  break;

						case 9:	if (NormalPost[9] < 2)
						{
							NormalPost[9]++;
							CardNormal[i][j] = '9';
						}
							  else
						{
							j--;
						}
							  break;

						case 10:	if (NormalPost[10] < 2)
						{
							NormalPost[10]++;
							CardNormal[i][j] = 'A';
						}
							   else
						{
							j--;
						}
							   break;

						case 11:	if (NormalPost[11] < 2)
						{
							NormalPost[11]++;
							CardNormal[i][j] = 'B';
						}
							   else
						{
							j--;
						}
							   break;

						case 12:	if (NormalPost[12] < 2)
						{
							NormalPost[12]++;
							CardNormal[i][j] = 'C';
						}
							   else
						{
							j--;
						}
							   break;

						case 13:	if (NormalPost[13] < 2)
						{
							NormalPost[13]++;
							CardNormal[i][j] = 'D';
						}
							   else
						{
							j--;
						}
							   break;

						case 14:	if (NormalPost[14] < 2)
						{
							NormalPost[14]++;
							CardNormal[i][j] = 'E';
						}
							   else
						{
							j--;
						}
							   break;

						case 15:	if (NormalPost[15] < 2)
						{
							NormalPost[15]++;
							CardNormal[i][j] = 'F';
						}
							   else
						{
							j--;
						}
							   break;

						case 16:	if (NormalPost[16] < 2)
						{
							NormalPost[16]++;
							CardNormal[i][j] = 'G';
						}
							   else
						{
							j--;
						}
							   break;

						case 17:	if (NormalPost[17] < 2)
						{
							NormalPost[17]++;
							CardNormal[i][j] = 'H';
						}
							   else
						{
							j--;
						}
							   break;

						case 18:	if (NormalPost[18] < 2)
						{
							NormalPost[18]++;
							CardNormal[i][j] = 'I';
						}
							   else
						{
							j--;
						}
							   break;

						case 19:	if (NormalPost[19] < 2)
						{
							NormalPost[19]++;
							CardNormal[i][j] = 'J';
						}
							   else
						{
							j--;
						}
							   break;

						case 20:	if (NormalPost[20] < 2)
						{
							NormalPost[20]++;
							CardNormal[i][j] = 'K';
						}
							   else
						{
							j--;
						}
							   break;

						case 21:	if (NormalPost[21] < 2)
						{
							NormalPost[21]++;
							CardNormal[i][j] = 'L';
						}
							   else
						{
							j--;
						}
							   break;

						case 22:	if (NormalPost[22] < 2)
						{
							NormalPost[22]++;
							CardNormal[i][j] = 'M';
						}
							   else
						{
							j--;
						}
							   break;

						case 23:	if (NormalPost[23] < 2)
						{
							NormalPost[23]++;
							CardNormal[i][j] = 'N';
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

	case Hard:	
	int HardPost[48];
	for (int i = 0; i < 48; i++)
	{
		HardPost[i] = 0;
	}

	for (int i = 0; i < HARD_HEIGHT; i++)
	{
		for (int j = 0; j < HARD_WIDTH; j++)
		{
			if (i % 2 != 0)
			{
				while (j < HARD_WIDTH)
				{
					CardHard[i][j] = 'x';
					j++;
				}
				break;
			}

			if (j % 2 == 0)
			{
				CardHard[i][j] = 'x';
			}
			else
			{
				int Card = rand() % 48;
				switch (Card)
				{
					case 0: if (HardPost[0] < 2)
					{
						HardPost[0]++;
						CardHard[i][j] = '0';
					}
						  else
					{
						j--;
					}
						  break;

					case 1: if (HardPost[1] < 2)
					{
						HardPost[1]++;
						CardHard[i][j] = '1';
					}
						  else
					{
						j--;
					}
						  break;

					case 2: if (HardPost[2] < 2)
					{
						HardPost[2]++;
						CardHard[i][j] = '2';
					}
						  else
					{
						j--;
					}
						  break;

					case 3:	if (HardPost[3] < 2)
					{
						HardPost[3]++;
						CardHard[i][j] = '3';
					}
						  else
					{
						j--;
					}
						  break;

					case 4:	if (HardPost[4] < 2)
					{
						HardPost[4]++;
						CardHard[i][j] = '4';
					}
						  else
					{
						j--;
					}
						  break;

					case 5:	if (HardPost[5] < 2)
					{
						HardPost[5]++;
						CardHard[i][j] = '5';
					}
						  else
					{
						j--;
					}
						  break;

					case 6:	if (HardPost[6] < 2)
					{
						HardPost[6]++;
						CardHard[i][j] = '6';
					}
						  else
					{
						j--;
					}
						  break;

					case 7:	if (HardPost[7] < 2)
					{
						HardPost[7]++;
						CardHard[i][j] = '7';
					}
						  else
					{
						j--;
					}
						  break;

					case 8:	if (HardPost[8] < 2)
					{
						HardPost[8]++;
						CardHard[i][j] = '8';
					}
						  else
					{
						j--;
					}
						  break;

					case 9:	if (HardPost[9] < 2)
					{
						HardPost[9]++;
						CardHard[i][j] = '9';
					}
						  else
					{
						j--;
					}
						  break;

					case 10:	if (HardPost[10] < 2)
					{
						HardPost[10]++;
						CardHard[i][j] = 'A';
					}
						   else
					{
						j--;
					}
						   break;

					case 11:	if (HardPost[11] < 2)
					{
						HardPost[11]++;
						CardHard[i][j] = 'B';
					}
						   else
					{
						j--;
					}
						   break;

					case 12:	if (HardPost[12] < 2)
					{
						HardPost[12]++;
						CardHard[i][j] = 'C';
					}
						   else
					{
						j--;
					}
						   break;

					case 13:	if (HardPost[13] < 2)
					{
						HardPost[13]++;
						CardHard[i][j] = 'D';
					}
						   else
					{
						j--;
					}
						   break;

					case 14:	if (HardPost[14] < 2)
					{
						HardPost[14]++;
						CardHard[i][j] = 'E';
					}
						   else
					{
						j--;
					}
						   break;

					case 15:	if (HardPost[15] < 2)
					{
						HardPost[15]++;
						CardHard[i][j] = 'F';
					}
						   else
					{
						j--;
					}
						   break;

					case 16:	if (HardPost[16] < 2)
					{
						HardPost[16]++;
						CardHard[i][j] = 'G';
					}
						   else
					{
						j--;
					}
						   break;

					case 17:	if (HardPost[17] < 2)
					{
						HardPost[17]++;
						CardHard[i][j] = 'H';
					}
						   else
					{
						j--;
					}
						   break;

					case 18:	if (HardPost[18] < 2)
					{
						HardPost[18]++;
						CardHard[i][j] = 'I';
					}
						   else
					{
						j--;
					}
						   break;

					case 19:	if (HardPost[19] < 2)
					{
						HardPost[19]++;
						CardHard[i][j] = 'J';
					}
						   else
					{
						j--;
					}
						   break;

					case 20:	if (HardPost[20] < 2)
					{
						HardPost[20]++;
						CardHard[i][j] = 'K';
					}
						   else
					{
						j--;
					}
						   break;

					case 21:	if (HardPost[21] < 2)
					{
						HardPost[21]++;
						CardHard[i][j] = 'L';
					}
						   else
					{
						j--;
					}
						   break;

					case 22:	if (HardPost[22] < 2)
					{
						HardPost[22]++;
						CardHard[i][j] = 'M';
					}
						   else
					{
						j--;
					}
						   break;

					case 23:	if (HardPost[23] < 2)
					{
						HardPost[23]++;
						CardHard[i][j] = 'N';
					}
						   else
					{
						j--;
					}
						   break;

					case 24:	if (HardPost[24] < 2)
					{
						HardPost[24]++;
						CardHard[i][j] = 'O';
					}
						   else
					{
						j--;
					}
						   break;

					case 25:	if (HardPost[25] < 2)
					{
						HardPost[25]++;
						CardHard[i][j] = 'P';
					}
						   else
					{
						j--;
					}
						   break;

					case 26:	if (HardPost[26] < 2)
					{
						HardPost[26]++;
						CardHard[i][j] = 'Q';
					}
						   else
					{
						j--;
					}
						   break;

					case 27:	if (HardPost[27] < 2)
					{
						HardPost[27]++;
						CardHard[i][j] = 'R';
					}
						   else
					{
						j--;
					}
						   break;

					case 28:	if (HardPost[28] < 2)
					{
						HardPost[28]++;
						CardHard[i][j] = 'S';
					}
						   else
					{
						j--;
					}
						   break;

					case 29:	if (HardPost[29] < 2)
					{
						HardPost[29]++;
						CardHard[i][j] = 'T';
					}
						   else
					{
						j--;
					}
						   break;

					case 30:	if (HardPost[30] < 2)
					{
						HardPost[30]++;
						CardHard[i][j] = 'U';
					}
						   else
					{
						j--;
					}
						   break;

					case 31:	if (HardPost[31] < 2)
					{
						HardPost[31]++;
						CardHard[i][j] = 'V';
					}
						   else
					{
						j--;
					}
						   break;

					case 32:	if (HardPost[32] < 2)
					{
						HardPost[32]++;
						CardHard[i][j] = 'W';
					}
						   else
					{
						j--;
					}
						   break;

					case 33:	if (HardPost[33] < 2)
					{
						HardPost[33]++;
						CardHard[i][j] = 'X';
					}
						   else
					{
						j--;
					}
						   break;

					case 34:	if (HardPost[34] < 2)
					{
						HardPost[34]++;
						CardHard[i][j] = 'Y';
					}
						   else
					{
						j--;
					}
						   break;

					case 35:	if (HardPost[35] < 2)
					{
						HardPost[35]++;
						CardHard[i][j] = 'Z';
					}
						   else
					{
						j--;
					}
						   break;

					case 36:	if (HardPost[36] < 2)
					{
						HardPost[36]++;
						CardHard[i][j] = 'a';
					}
						   else
					{
						j--;
					}
						   break;

					case 37:	if (HardPost[37] < 2)
					{
						HardPost[37]++;
						CardHard[i][j] = 'b';
					}
						   else
					{
						j--;
					}
						   break;

					case 38:	if (HardPost[38] < 2)
					{
						HardPost[38]++;
						CardHard[i][j] = 'c';
					}
						   else
					{
						j--;
					}
						   break;

					case 39:	if (HardPost[39] < 2)
					{
						HardPost[39]++;
						CardHard[i][j] = 'd';
					}
						   else
					{
						j--;
					}
						   break;

					case 40:	if (HardPost[40] < 2)
					{
						HardPost[40]++;
						CardHard[i][j] = 'e';
					}
						   else
					{
						j--;
					}
						   break;

					case 41:	if (HardPost[41] < 2)
					{
						HardPost[41]++;
						CardHard[i][j] = 'f';
					}
						   else
					{
						j--;
					}
						   break;

					case 42:	if (HardPost[42] < 2)
					{
						HardPost[42]++;
						CardHard[i][j] = 'g';
					}
						   else
					{
						j--;
					}
						   break;

					case 43:	if (HardPost[43] < 2)
					{
						HardPost[43]++;
						CardHard[i][j] = 'h';
					}
						   else
					{
						j--;
					}
						   break;

					case 44:	if (HardPost[44] < 2)
					{
						HardPost[44]++;
						CardHard[i][j] = 'i';
					}
						   else
					{
						j--;
					}
						   break;

					case 45:	if (HardPost[45] < 2)
					{
						HardPost[45]++;
						CardHard[i][j] = 'j';
					}
						   else
					{
						j--;
					}
						   break;

					case 46:	if (HardPost[46] < 2)
					{
						HardPost[46]++;
						CardHard[i][j] = 'k';
					}
						   else
					{
						j--;
					}
						   break;

					case 47:	if (HardPost[47] < 2)
					{
						HardPost[47]++;
						CardHard[i][j] = 'l';
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

// ī���� �̹��� ���� �� ����ϴ� �Լ� (�� ĭ�� �����, ī�尡 �ִ� ĭ�� ������� ���)
void CardRender()
{
	switch (diff)
	{
	case Easy: for (int i = 0; i < EASY_HEIGHT; i++)
				{
					for (int j = 0; j < EASY_WIDTH; j++)
					{
						if (CardEasy[i][j] == 'x')
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
							printf("  ");
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						}
						
						if (CardEasy[i][j] == '0')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '1')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '2')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '3')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '4')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '5')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '6')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '7')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '8')
						{
							printf("��");
						}
						else if (CardEasy[i][j] == '9')
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
			if (CardNormal[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (CardNormal[i][j] == '0')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '1')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '2')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '3')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '4')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '5')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '6')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '7')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '8')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == '9')
			{
				printf("��");
			}
			else if (CardNormal[i][j] == 'A')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'B')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'C')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'D')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'E')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'F')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'G')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'H')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'I')
			{
				printf("�"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'J')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'K')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'L')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'M')
			{
				printf("��"); // Ư������ ����
			}
			else if (CardNormal[i][j] == 'N')
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
			if (CardHard[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (CardHard[i][j] == '0')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '1')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '2')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '3')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '4')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '5')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '6')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '7')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '8')
			{
				printf("��");
			}
			else if (CardHard[i][j] == '9')
			{
				printf("��");
			}
			else if (CardHard[i][j] == 'A')
			{
				printf("��"); // Ư������ ���� A
			}
			else if (CardHard[i][j] == 'B')
			{
				printf("��"); // Ư������ ���� B
			}
			else if (CardHard[i][j] == 'C')
			{
				printf("��"); // Ư������ ���� C
			}
			else if (CardHard[i][j] == 'D')
			{
				printf("��"); // Ư������ ���� D
			}
			else if (CardHard[i][j] == 'E')
			{
				printf("��"); // Ư������ ���� E
			}
			else if (CardHard[i][j] == 'F')
			{
				printf("��"); // Ư������ ���� F
			}
			else if (CardHard[i][j] == 'G')
			{
				printf("��"); // Ư������ ���� G
			}
			else if (CardHard[i][j] == 'H')
			{
				printf("��"); // Ư������ ���� H
			}
			else if (CardHard[i][j] == 'I')
			{
				printf("��"); // Ư������ ���� I
			}
			else if (CardHard[i][j] == 'J')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'K')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'L')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'M')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'N')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'O')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'P')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'Q')
			{
				printf("�"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'R')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'S')
			{
				printf("Ϣ"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'T')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'U')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'V')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'W')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'X')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'Y')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'Z')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'a')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'b')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'c')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'd')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'e')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'f')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'g')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'h')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'i')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'j')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'k')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (CardHard[i][j] == 'l')
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

// ���� ��ġ �Լ�
void CreateTitle()
{
	strcpy(Title[0], "01111111111000111111111100011111111110");
	strcpy(Title[1], "01000000001000100000000100010000000010");
	strcpy(Title[2], "0100EASY00100010NORMAL01000100HARD0010");
	strcpy(Title[3], "01000000001000100000000100010000000010");
	strcpy(Title[4], "01111111111000111111111100011111111110");
}

// ���� �̹��� & ���� ���� �� ��� �Լ�
void TitleRender()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 39; j++)
		{
			if (j > 0 && j <= 10)
			{
				if (Title[i][j] == '1')
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
				if (Title[i][j] == '1')
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
				if (Title[i][j] == '1')
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

// ���̵� ���� �� ī��Ʈ�ٿ� 3�� ��� �Լ�
void Countdown()
{
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
}

// ���� Ŭ���� �� ������ "CONGRATULATIONS!!" ���� ��� �Լ�
void Cleargame()
{
	char Clear[16] = "CONGRATULATIONS";
	Sleep(100);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	for (int i = 0; i < 16; i++)
	{
		printf("%c ", Clear[i]);
		Sleep(50);
	}
	printf("!!\n");
}

// ī���� ¦�� �´��� Ȯ���ϴ� �Լ�
void Ingame(enum Diff diff)
{ 
	switch (diff)
	{
	case Easy: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { MatchEasy--; CardEasy[Memory[1]][Memory[2]] = 'x'; CardEasy[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			 else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			 if (MatchEasy == 0) { Point++; }
			 break;

	case Normal: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { MatchNormal--; CardNormal[Memory[1]][Memory[2]] = 'x'; CardNormal[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			   else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			   if (MatchNormal == 0) { Point++; }
			   break;

	case Hard: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { MatchHard--; CardHard[Memory[1]][Memory[2]] = 'x'; CardHard[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			 else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			 if (MatchHard == 0) { Point++; }
			 break;

	default:
		break;
	}
}

// ������ ī�� �̹����� ���� �� ����ϴ� �Լ�
char ShowCard(enum Diff diff, CurrentCard * Current)
{
	switch (diff)
	{
	case Easy: {if (Current->show == '0')
	{
		printf("��");
	}
			 else if (Current->show == '1')
	{
		printf("��");
	}
			 else if (Current->show == '2')
	{
		printf("��");
	}
			 else if (Current->show == '3')
	{
		printf("��");
	}
			 else if (Current->show == '4')
	{
		printf("��");
	}
			 else if (Current->show == '5')
	{
		printf("��");
	}
			 else if (Current->show == '6')
	{
		printf("��");
	}
			 else if (Current->show == '7')
	{
		printf("��");
	}
			 else if (Current->show == '8')
	{
		printf("��");
	}
			 else if (Current->show == '9')
	{
		printf("��");
	}
			 break;
	}

	case Normal: {if (Current->show == '0')
	{
		printf("��");
	}
			   else if (Current->show == '1')
	{
		printf("��");
	}
			   else if (Current->show == '2')
	{
		printf("��");
	}
			   else if (Current->show == '3')
	{
		printf("��");
	}
			   else if (Current->show == '4')
	{
		printf("��");
	}
			   else if (Current->show == '5')
	{
		printf("��");
	}
			   else if (Current->show == '6')
	{
		printf("��");
	}
			   else if (Current->show == '7')
	{
		printf("��");
	}
			   else if (Current->show == '8')
	{
		printf("��");
	}
			   else if (Current->show == '9')
	{
		printf("��");
	}
			   else if (Current->show == 'A')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'B')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'C')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'D')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'E')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'F')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'G')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'H')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'I')
	{
		printf("�"); // Ư������ ����
	}
			   else if (Current->show == 'J')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'K')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'L')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'M')
	{
		printf("��"); // Ư������ ����
	}
			   else if (Current->show == 'N')
	{
		printf("��"); // Ư������ ����
	}
			   break;
	}

	case Hard: {if (Current->show == '0')
	{
		printf("��");
	}
			 else if (Current->show == '1')
	{
		printf("��");
	}
			 else if (Current->show == '2')
	{
		printf("��");
	}
			 else if (Current->show == '3')
	{
		printf("��");
	}
			 else if (Current->show == '4')
	{
		printf("��");
	}
			 else if (Current->show == '5')
	{
		printf("��");
	}
			 else if (Current->show == '6')
	{
		printf("��");
	}
			 else if (Current->show == '7')
	{
		printf("��");
	}
			 else if (Current->show == '8')
	{
		printf("��");
	}
			 else if (Current->show == '9')
	{
		printf("��");
	}
			 else if (Current->show == 'A')
	{
		printf("��"); // Ư������ ���� A
	}
			 else if (Current->show == 'B')
	{
		printf("��"); // Ư������ ���� B
	}
			 else if (Memory[0] == 'C')
	{
		printf("��"); // Ư������ ���� C
	}
			 else if (Current->show == 'D')
	{
		printf("��"); // Ư������ ���� D
	}
			 else if (Current->show == 'E')
	{
		printf("��"); // Ư������ ���� E
	}
			 else if (Current->show == 'F')
	{
		printf("��"); // Ư������ ���� F
	}
			 else if (Current->show == 'G')
	{
		printf("��"); // Ư������ ���� G
	}
			 else if (Current->show == 'H')
	{
		printf("��"); // Ư������ ���� H
	}
			 else if (Current->show == 'I')
	{
		printf("��"); // Ư������ ���� I
	}
			 else if (Current->show == 'J')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'K')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'L')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'M')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'N')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'O')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'P')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'Q')
	{
		printf("�"); // Ư������ ���� J
	}
			 else if (Current->show == 'R')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'S')
	{
		printf("Ϣ"); // Ư������ ���� J
	}
			 else if (Current->show == 'T')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'U')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'V')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'W')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'X')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'Y')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'Z')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'a')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'b')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'c')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'd')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'e')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'f')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'g')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'h')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'i')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'j')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'k')
	{
		printf("��"); // Ư������ ���� J
	}
			 else if (Current->show == 'l')
	{
		printf("��"); // Ư������ ���� J
	}
			 break;
	}
	default:
		break;
	}
	return 0;
}


int main()
{
	CursorView();
	SelectCard Select = { 0, 2, "��"};
	CurrentCard Current = { 0, 0, "������ ī�� : ", ' '};
	CreateTitle();
	char CardPrint = Current.show;
	while (1)
	{
		while (Point == 0)
		{
			TitleRender();
			Keyboard(&Select, &Current);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);
			Sleep(100);
			system("cls");
		}

		Countdown();

		while (Point == 1)
		{
			CardRender();
			Keyboard(&Select, &Current);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);

			if (diff == Easy)
			{
				Current.y = 8;
				if (Check == 0)
				{
					Current.show = ' ';
				}
				else if(Check == 1)
				{
					Current.show = CardEasy[Select.y][Select.x / 2 + 1];
				}
				GotoXY(Current.x, Current.y);
				printf("%s", Current.text);
				ShowCard(Easy, &Current);
			}
			else if (diff == Normal)
			{
				Current.y = 12;
				if (Check == 0)
				{
					Current.show = ' ';
				}
				else if (Check == 1)
				{
					Current.show = CardNormal[Select.y][Select.x / 2 + 1];
				}
				GotoXY(Current.x, Current.y);
				printf("%s", Current.text);
				ShowCard(Normal, &Current);
			}
			else if (diff == Hard)
			{
				Current.y = 16;
				if (Check == 0)
				{
					Current.show = ' ';
				}
				else if (Check == 1)
				{
					Current.show = CardHard[Select.y][Select.x / 2 + 1];
				}
				GotoXY(Current.x, Current.y);
				printf("%s", Current.text);
				ShowCard(Hard, &Current);
			}
			Sleep(100);
			system("cls");
		}

		Cleargame();
		break;
	}
	return 0;
}