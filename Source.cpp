#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72	 // ����Ű(��)
#define LEFT 75  // ����Ű(��)
#define RIGHT 77 // ����Ű(��)
#define DOWN 80  // ����Ű(��)
#define SPACE 32 // �����̽� ��

#define EASY_WIDTH 11	 // ���̵�(Easy)�� ���� ����
#define EASY_HEIGHT 8	 // ���̵�(Easy)�� ���� ����
#define NORMAL_WIDTH 17  // ���̵�(Normal)�� ���� ����
#define NORMAL_HEIGHT 12 // ���̵�(Normal)�� ���� ����
#define HARD_WIDTH 25	 // ���̵�(Hard)�� ���� ����
#define HARD_HEIGHT 16	 // ���̵�(Hard)�� ���� ����

char Title[5][39];								// 2���� ���ڿ�
char CardEasy[EASY_HEIGHT][EASY_WIDTH];			// Easy 5 X 4
char CardNormal[NORMAL_HEIGHT][NORMAL_WIDTH];	// Normal 8 X 6
char CardHard[HARD_HEIGHT][HARD_WIDTH];			// Hard 12 X 8

int Scene = 0; // Scene ������ Ȯ���ϴ� ����
int Check = 0; // �ΰ��� �� ī�� ���� ���θ� Ȯ���ϴ� ����
char Memory[7] = "zzzzzz"; // [0] = ù��°�� ������ ī���� ��� / [1] = ù��° ī���� y��ǥ / [2] = ù��° ī���� x��ǥ
						   // [3] = �ι�°�� ������ ī���� ��� / [4] = �ι�° ī���� y��ǥ / [5] = �ι�° ī���� x��ǥ

int MatchEasy = 10;	  // Easy ���̵����� ������ϴ� ¦�� ����
int MatchNormal = 24; // Normal ���̵����� ������ϴ� ¦�� ����
int MatchHard = 48;	  // Hard ���̵����� ������ϴ� ¦�� ����

#pragma region ���̵��� ���� ũ�� ����

/* [EASY] �ΰ��� ���� ũ�� : 5 x 4 / ���� ���� ũ�� : 10 x 8

 X = ��ĭ
 O = ī�� ��ġ

XOXOXOXOXO
XXXXXXXXXX
XOXOXOXOXO
XXXXXXXXXX
XOXOXOXOXO
XXXXXXXXXX
XOXOXOXOXO
XXXXXXXXXX

 [NORMAL] �ΰ��� ���� ũ�� : 8 x 6 / ���� ���� ũ�� : 16 x 12
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXX

 [HARD] �ΰ��� ���� ũ�� : 12 x 8 / ���� ���� ũ�� : 24 x 16
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX
XOXOXOXOXOXOXOXOXOXOXOXO
XXXXXXXXXXXXXXXXXXXXXXXX */

#pragma endregion

// ���� ��ǥ�� ȭ��ǥ�� ���
typedef struct SelectCard
{
	int x;
	int y;
	const char* shape;
}SelectCard;

// �ΰ��ӿ��� ī�� ���� �� ī�� �̹����� ���
typedef struct CurrentCard
{
	int x;
	int y;
	const char* text;
	char show;
}CurrentCard;

// ���� ���̵�
enum Diff
{
	Easy,
	Normal,
	Hard
};
Diff diff;

void CreateCard(enum Diff diff);
void Ingame(enum Diff diff);
char ShowCard(enum Diff diff, CurrentCard* Current);

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

// ī�� ��ġ �Լ�
void CreateCard(enum Diff diff)
{
	srand(time(NULL));
	switch (diff)
	{
	case Easy: 
		int ArrEasy[10];
		for (int i = 0; i < 10; i++)
		{
			ArrEasy[i] = 0;
		}

		for (int i = 0; i < EASY_HEIGHT; i++)
		{
			int j = 0;
			while (j < EASY_WIDTH)
			{
				if (i % 2 != 0)
				{
					int k = 0;
					while (k < EASY_WIDTH)
					{
						CardEasy[i][k] = 'x';
						k++;
					}
					break;
				}
				
				if (j % 2 == 0)
				{
					CardEasy[i][j] = 'x';
					j++;
				}
				else
				{
					int Card = rand() % 10;
					if (ArrEasy[Card] >= 2)
					{
						continue;
					}
					else
					{
						switch (Card)
						{
						case 0: ArrEasy[0]++; CardEasy[i][j] = '0';
							break;

						case 1: ArrEasy[1]++; CardEasy[i][j] = '1';
							break;

						case 2: ArrEasy[2]++; CardEasy[i][j] = '2';
							break;

						case 3: ArrEasy[3]++; CardEasy[i][j] = '3';
							break;

						case 4: ArrEasy[4]++; CardEasy[i][j] = '4';
							break;

						case 5: ArrEasy[5]++; CardEasy[i][j] = '5';
							break;

						case 6: ArrEasy[6]++; CardEasy[i][j] = '6';
							break;

						case 7: ArrEasy[7]++; CardEasy[i][j] = '7';
							break;

						case 8: ArrEasy[8]++; CardEasy[i][j] = '8';
							break;

						case 9: ArrEasy[9]++; CardEasy[i][j] = '9';
							break;

						default:
							break;
						}
						j++;
					}
				}
			}
		}
	
	break;
	
	case Normal: 
		int ArrNormal[24];
		for (int i = 0; i < 24; i++)
		{
			ArrNormal[i] = 0;
		}

		for (int i = 0; i < NORMAL_HEIGHT; i++)
		{
			int j = 0;
			while(j < NORMAL_WIDTH)
			{
				if (i % 2 != 0)
				{
					int k = 0;
					while (k < NORMAL_WIDTH)
					{
						CardNormal[i][k] = 'x';
						k++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					CardNormal[i][j] = 'x';
					j++;
				}
				else
				{
					int Card = rand() % 24;
					if (ArrNormal[Card] >= 2)
					{
						continue;
					}
					else
					{
						switch (Card)
						{
						case 0: ArrNormal[0]++; CardNormal[i][j] = '0';
							break;

						case 1: ArrNormal[1]++; CardNormal[i][j] = '1';
							break;

						case 2: ArrNormal[2]++; CardNormal[i][j] = '2';
							break;

						case 3: ArrNormal[3]++; CardNormal[i][j] = '3';
							break;

						case 4: ArrNormal[4]++; CardNormal[i][j] = '4';
							break;

						case 5: ArrNormal[5]++; CardNormal[i][j] = '5';
							break;

						case 6: ArrNormal[6]++; CardNormal[i][j] = '6';
							break;

						case 7: ArrNormal[7]++; CardNormal[i][j] = '7';
							break;

						case 8: ArrNormal[8]++; CardNormal[i][j] = '8';
							break;

						case 9: ArrNormal[9]++; CardNormal[i][j] = '9';
							break;

						case 10: ArrNormal[10]++; CardNormal[i][j] = 'A';
							break;

						case 11: ArrNormal[11]++; CardNormal[i][j] = 'B';
							break;

						case 12: ArrNormal[12]++; CardNormal[i][j] = 'C';
							break;

						case 13: ArrNormal[13]++; CardNormal[i][j] = 'D';
							break;

						case 14: ArrNormal[14]++; CardNormal[i][j] = 'E';
							break;

						case 15: ArrNormal[15]++; CardNormal[i][j] = 'F';
							break;

						case 16: ArrNormal[16]++; CardNormal[i][j] = 'G';
							break;

						case 17: ArrNormal[17]++; CardNormal[i][j] = 'H';
							break;

						case 18: ArrNormal[18]++; CardNormal[i][j] = 'I';
							break;

						case 19: ArrNormal[19]++; CardNormal[i][j] = 'J';
							break;

						case 20: ArrNormal[20]++; CardNormal[i][j] = 'K';
							break;

						case 21: ArrNormal[21]++; CardNormal[i][j] = 'L';
							break;

						case 22: ArrNormal[22]++; CardNormal[i][j] = 'M';
							break;

						case 23: ArrNormal[23]++; CardNormal[i][j] = 'N';
							break;

						default:
							break;
						}
						j++;
					}
				}
			}
		}
	
	break;

	case Hard:	
		int ArrHard[48];
		for (int i = 0; i < 48; i++)
	{
		ArrHard[i] = 0;
	}

		for (int i = 0; i < HARD_HEIGHT; i++)
		{
			int j = 0;
			while (j < HARD_WIDTH)
			{
				if (i % 2 != 0)
				{
					int k = 0;
					while (k < HARD_WIDTH)
					{
						CardHard[i][k] = 'x';
						k++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					CardHard[i][j] = 'x';
					j++;
				}
				else
				{
					int Card = rand() % 48;
					if (ArrHard[Card] >= 2)
					{
						continue;
					}
					else
					{
						switch (Card)
						{
						case 0: ArrHard[0]++; CardHard[i][j] = '0';
							break;

						case 1: ArrHard[1]++; CardHard[i][j] = '1';
							break;

						case 2: ArrHard[2]++; CardHard[i][j] = '2';
							break;

						case 3: ArrHard[3]++; CardHard[i][j] = '3';
							break;

						case 4: ArrHard[4]++; CardHard[i][j] = '4';
							break;

						case 5: ArrHard[5]++; CardHard[i][j] = '5';
							break;

						case 6: ArrHard[6]++; CardHard[i][j] = '6';
							break;

						case 7: ArrHard[7]++; CardHard[i][j] = '7';
							break;

						case 8: ArrHard[8]++; CardHard[i][j] = '8';
							break;

						case 9: ArrHard[9]++; CardHard[i][j] = '9';
							break;

						case 10: ArrHard[10]++; CardHard[i][j] = 'A';
							break;

						case 11: ArrHard[11]++; CardHard[i][j] = 'B';
							break;

						case 12: ArrHard[12]++; CardHard[i][j] = 'C';
							break;

						case 13: ArrHard[13]++; CardHard[i][j] = 'D';
							break;

						case 14: ArrHard[14]++; CardHard[i][j] = 'E';
							break;

						case 15: ArrHard[15]++; CardHard[i][j] = 'F';
							break;

						case 16: ArrHard[16]++; CardHard[i][j] = 'G';
							break;

						case 17: ArrHard[17]++; CardHard[i][j] = 'H';
							break;

						case 18: ArrHard[18]++; CardHard[i][j] = 'I';
							break;

						case 19: ArrHard[19]++; CardHard[i][j] = 'J';
							break;

						case 20: ArrHard[20]++; CardHard[i][j] = 'K';
							break;

						case 21: ArrHard[21]++; CardHard[i][j] = 'L';
							break;

						case 22: ArrHard[22]++; CardHard[i][j] = 'M';
							break;

						case 23: ArrHard[23]++; CardHard[i][j] = 'N';
							break;

						case 24: ArrHard[24]++; CardHard[i][j] = 'O';
							break;

						case 25: ArrHard[25]++; CardHard[i][j] = 'P';
							break;

						case 26: ArrHard[26]++; CardHard[i][j] = 'Q';
							break;

						case 27: ArrHard[27]++; CardHard[i][j] = 'R';
							break;

						case 28: ArrHard[28]++; CardHard[i][j] = 'S';
							break;

						case 29: ArrHard[29]++; CardHard[i][j] = 'T';
							break;

						case 30: ArrHard[30]++; CardHard[i][j] = 'U';
							break;

						case 31: ArrHard[31]++; CardHard[i][j] = 'V';
							break;

						case 32: ArrHard[32]++; CardHard[i][j] = 'W';
							break;

						case 33: ArrHard[33]++; CardHard[i][j] = 'X';
							break;

						case 34: ArrHard[34]++; CardHard[i][j] = 'Y';
							break;

						case 35: ArrHard[35]++; CardHard[i][j] = 'Z';
							break;

						case 36: ArrHard[36]++; CardHard[i][j] = 'a';
							break;

						case 37: ArrHard[37]++; CardHard[i][j] = 'b';
							break;

						case 38: ArrHard[38]++; CardHard[i][j] = 'c';
							break;

						case 39: ArrHard[39]++; CardHard[i][j] = 'd';
							break;

						case 40: ArrHard[40]++; CardHard[i][j] = 'e';
							break;

						case 41: ArrHard[41]++; CardHard[i][j] = 'f';
							break;

						case 42: ArrHard[42]++; CardHard[i][j] = 'g';
							break;

						case 43: ArrHard[43]++; CardHard[i][j] = 'h';
							break;

						case 44: ArrHard[44]++; CardHard[i][j] = 'i';
							break;

						case 45: ArrHard[45]++; CardHard[i][j] = 'j';
							break;

						case 46: ArrHard[46]++; CardHard[i][j] = 'k';
							break;

						case 47: ArrHard[47]++; CardHard[i][j] = 'l';
							break;

						}
						j++;
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
		case SPACE: if (Scene == 0 && Select->x == 0) { diff = Easy; CreateCard(Easy); Scene++; Select->y = 0; }
				  else if (Scene == 0 && Select->x == 26) { diff = Normal; CreateCard(Normal); Scene++; Select->x = 0; Select->y = 0; }
				  else if (Scene == 0 && Select->x == 52) { diff = Hard; CreateCard(Hard); Scene++; Select->x = 0; Select->y = 0; }
				  else if (Scene == 1 && Check == 0 && diff == Easy && CardEasy[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardEasy[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 0 && diff == Normal && CardNormal[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardNormal[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 0 && diff == Hard && CardHard[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = CardHard[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 1 && diff == Easy && CardEasy[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardEasy[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Easy); }
				  else if (Scene == 1 && Check == 1 && diff == Normal && CardNormal[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardNormal[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Normal); }
				  else if (Scene == 1 && Check == 1 && diff == Hard && CardHard[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = CardHard[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Ingame(Hard); }

			break;

		case UP: if (Scene == 1 && Select->y / 2 - 1 >= 0) { Select->y -= 2; }
			break;

		case DOWN: if (Scene == 1 && diff == Easy && Select->y / 2 + 1 < EASY_HEIGHT / 2) { Select->y += 2; }
				 else if (Scene == 1 && diff == Normal && Select->y / 2 + 1 < NORMAL_HEIGHT / 2) { Select->y += 2; }
				 else if (Scene == 1 && diff == Hard && Select->y / 2 + 1 < HARD_HEIGHT / 2) { Select->y += 2; }
			break;

		case LEFT: if (Scene == 0 && Select->x / 2 - 13 >= 0) { Select->x -= 26; }
				 else if (Scene == 1 && Select->x / 4 - 1 >= 0) { Select->x -= 4; }
			break;

		case RIGHT: if (Scene == 0 && Select->x / 2 + 13 <= 38) { Select->x += 26; }
				  else if (Scene == 1 && diff == Easy && Select->x / 4 + 1 < EASY_WIDTH / 2) { Select->x += 4; }
				  else if (Scene == 1 && diff == Normal && Select->x / 4 + 1 < NORMAL_WIDTH / 2) { Select->x += 4; }
				  else if (Scene == 1 && diff == Hard && Select->x / 4 + 1 < HARD_WIDTH / 2) { Select->x += 4; }
			break;

		default:
			break;
		}
	}
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

			 if (MatchEasy == 0) { Scene++; }
			 break;

	case Normal: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { MatchNormal--; CardNormal[Memory[1]][Memory[2]] = 'x'; CardNormal[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			   else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			   if (MatchNormal == 0) { Scene++; }
			   break;

	case Hard: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { MatchHard--; CardHard[Memory[1]][Memory[2]] = 'x'; CardHard[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			 else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			 if (MatchHard == 0) { Scene++; }
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

// ���� Ŭ���� �� ������ "CONGRATULATIONS!!" ���� ��� �Լ�
void Cleargame()
{
	char Clear[16] = "CONGRATULATIONS";
	Sleep(100);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	for (int i = 0; i < 16; i++)
	{
		printf("%c ", Clear[i]);
		Sleep(30);
	}
	printf("!!\n");
}

int main()
{
	CursorView();
	SelectCard Select = { 0, 2, "��"};
	CurrentCard Current = { 0, 0, "������ ī�� : ", ' '};
	CreateTitle();
	while (1)
	{
		while (Scene == 0)
		{
			TitleRender();
			Keyboard(&Select, &Current);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);
			Sleep(100);
			system("cls");
		}

		Countdown();

		while (Scene == 1)
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
					Current.show = Memory[0];
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
					Current.show = Memory[0];
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
					Current.show = Memory[0];
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