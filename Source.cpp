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

#define EASY_WIDTH 10	 // ���̵�(Easy)�� ���� ����
#define EASY_HEIGHT 8	 // ���̵�(Easy)�� ���� ����
#define NORMAL_WIDTH 16  // ���̵�(Normal)�� ���� ����
#define NORMAL_HEIGHT 12 // ���̵�(Normal)�� ���� ����
#define HARD_WIDTH 24	 // ���̵�(Hard)�� ���� ����
#define HARD_HEIGHT 16	 // ���̵�(Hard)�� ���� ����

char Main[17][13];								// ���̵� ���� ȭ�� �迭 ����
char Replay[11][13];							// �ٽ��ϱ� ȭ�� �迭 ����
char BoardEasy[EASY_HEIGHT][EASY_WIDTH];		// ���� ũ��(Easy) : 5 X 4
char BoardNormal[NORMAL_HEIGHT][NORMAL_WIDTH];  // ���� ũ��(Normal) : 8 X 6
char BoardHard[HARD_HEIGHT][HARD_WIDTH];		// ���� ũ��(Hard) : 12 X 8

int Scene = 0;  // ���� ȭ���� Ȯ���ϴ� ���� (0 = ���̵� ����, 1 = �ΰ���, 2 = �ٽ��ϱ�)
int Check = 0;  // �ΰ��� �� ī�带 �����ߴ��� Ȯ���ϴ� ���� (0 = ���� �� ��, 1 = ������)
int Count;		// ������ϴ� ¦�� ���� (Easy = 10, Normal = 24, Hard = 48)
char Memory[7]; // [0] = ù��°�� ������ ī�� �̹��� / [1] = ù��° ī���� y��ǥ / [2] = ù��° ī���� x��ǥ
				// [3] = �ι�°�� ������ ī�� �̹��� / [4] = �ι�° ī���� y��ǥ / [5] = �ι�° ī���� x��ǥ

#pragma region ���̵��� ���� ũ�� ����

/*
	[EASY]

	OOOOO
	OOOOO
	OOOOO
	OOOOO
	OOOOO

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

// ���� ȭ�� ��ġ �Լ�
void CreateMain()
{
	strcpy(Main[0], "011111111110");
	strcpy(Main[1], "010000000010");
	strcpy(Main[2], "0100EASY0010");
	strcpy(Main[3], "010000000010");
	strcpy(Main[4], "011111111110");
	strcpy(Main[5], "000000000000");
	strcpy(Main[6], "011111111110");
	strcpy(Main[7], "010000000010");
	strcpy(Main[8], "010NORMAL010");
	strcpy(Main[9], "010000000010");
	strcpy(Main[10], "011111111110");
	strcpy(Main[11], "000000000000");
	strcpy(Main[12], "011111111110");
	strcpy(Main[13], "010000000010");
	strcpy(Main[14], "0100HARD0010");
	strcpy(Main[15], "010000000010");
	strcpy(Main[16], "011111111110");
}

// ���� ȭ�� ���� & �̹��� ���� �� ��� �Լ�
void MainRender(SelectCard * Select)
{
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (Select->y == 2)
			{
				if (i >= 0 && i <= 4 && Main[i][j] == '1')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else if (i >= 5)
				{
					if (Main[i][j] == '1')
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (Select->y == 8)
			{
				if (i >= 6 && i <= 10 && Main[i][j] == '1')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else if (i <= 5 || i >= 11)
				{
					if (Main[i][j] == '1')
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (Select->y == 14)
			{
				if (i >= 12 && i <= 16 && Main[i][j] == '1')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else if (i <= 11)
				{
					if (Main[i][j] == '1')
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			
			if (Main[i][j] == '0')
			{
				printf("  ");
			}
			else if (Main[i][j] == '1')
			{
				printf("��");
			}
			else if (Main[i][j] == 'E')
			{
				printf("��");
			}
			else if (Main[i][j] == 'A')
			{
				printf("��");
			}
			else if (Main[i][j] == 'S')
			{
				printf("��");
			}
			else if (Main[i][j] == 'Y')
			{
				printf("��");
			}
			else if (Main[i][j] == 'N')
			{
				printf("��");
			}
			else if (Main[i][j] == 'O')
			{
				printf("��");
			}
			else if (Main[i][j] == 'R')
			{
				printf("��");
			}
			else if (Main[i][j] == 'M')
			{
				printf("��");
			}
			else if (Main[i][j] == 'L')
			{
				printf("��");
			}
			else if (Main[i][j] == 'H')
			{
				printf("��");
			}
			else if (Main[i][j] == 'D')
			{
				printf("��");
			}
		}
		printf("\n");
	}
}

// ī�� ��ġ �Լ�
void CreateCard()
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
						BoardEasy[i][k] = 'x';
						k++;
					}
					break;
				}
				
				if (j % 2 == 0)
				{
					BoardEasy[i][j] = 'x';
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
						case 0: ArrEasy[0]++; BoardEasy[i][j] = '0';
							break;

						case 1: ArrEasy[1]++; BoardEasy[i][j] = '1';
							break;

						case 2: ArrEasy[2]++; BoardEasy[i][j] = '2';
							break;

						case 3: ArrEasy[3]++; BoardEasy[i][j] = '3';
							break;

						case 4: ArrEasy[4]++; BoardEasy[i][j] = '4';
							break;

						case 5: ArrEasy[5]++; BoardEasy[i][j] = '5';
							break;

						case 6: ArrEasy[6]++; BoardEasy[i][j] = '6';
							break;

						case 7: ArrEasy[7]++; BoardEasy[i][j] = '7';
							break;

						case 8: ArrEasy[8]++; BoardEasy[i][j] = '8';
							break;

						case 9: ArrEasy[9]++; BoardEasy[i][j] = '9';
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
						BoardNormal[i][k] = 'x';
						k++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					BoardNormal[i][j] = 'x';
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
						case 0: ArrNormal[0]++; BoardNormal[i][j] = '0';
							break;

						case 1: ArrNormal[1]++; BoardNormal[i][j] = '1';
							break;

						case 2: ArrNormal[2]++; BoardNormal[i][j] = '2';
							break;

						case 3: ArrNormal[3]++; BoardNormal[i][j] = '3';
							break;

						case 4: ArrNormal[4]++; BoardNormal[i][j] = '4';
							break;

						case 5: ArrNormal[5]++; BoardNormal[i][j] = '5';
							break;

						case 6: ArrNormal[6]++; BoardNormal[i][j] = '6';
							break;

						case 7: ArrNormal[7]++; BoardNormal[i][j] = '7';
							break;

						case 8: ArrNormal[8]++; BoardNormal[i][j] = '8';
							break;

						case 9: ArrNormal[9]++; BoardNormal[i][j] = '9';
							break;

						case 10: ArrNormal[10]++; BoardNormal[i][j] = 'A';
							break;

						case 11: ArrNormal[11]++; BoardNormal[i][j] = 'B';
							break;

						case 12: ArrNormal[12]++; BoardNormal[i][j] = 'C';
							break;

						case 13: ArrNormal[13]++; BoardNormal[i][j] = 'D';
							break;

						case 14: ArrNormal[14]++; BoardNormal[i][j] = 'E';
							break;

						case 15: ArrNormal[15]++; BoardNormal[i][j] = 'F';
							break;

						case 16: ArrNormal[16]++; BoardNormal[i][j] = 'G';
							break;

						case 17: ArrNormal[17]++; BoardNormal[i][j] = 'H';
							break;

						case 18: ArrNormal[18]++; BoardNormal[i][j] = 'I';
							break;

						case 19: ArrNormal[19]++; BoardNormal[i][j] = 'J';
							break;

						case 20: ArrNormal[20]++; BoardNormal[i][j] = 'K';
							break;

						case 21: ArrNormal[21]++; BoardNormal[i][j] = 'L';
							break;

						case 22: ArrNormal[22]++; BoardNormal[i][j] = 'M';
							break;

						case 23: ArrNormal[23]++; BoardNormal[i][j] = 'N';
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
						BoardHard[i][k] = 'x';
						k++;
					}
					break;
				}

				if (j % 2 == 0)
				{
					BoardHard[i][j] = 'x';
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
						case 0: ArrHard[0]++; BoardHard[i][j] = '0';
							break;

						case 1: ArrHard[1]++; BoardHard[i][j] = '1';
							break;

						case 2: ArrHard[2]++; BoardHard[i][j] = '2';
							break;

						case 3: ArrHard[3]++; BoardHard[i][j] = '3';
							break;

						case 4: ArrHard[4]++; BoardHard[i][j] = '4';
							break;

						case 5: ArrHard[5]++; BoardHard[i][j] = '5';
							break;

						case 6: ArrHard[6]++; BoardHard[i][j] = '6';
							break;

						case 7: ArrHard[7]++; BoardHard[i][j] = '7';
							break;

						case 8: ArrHard[8]++; BoardHard[i][j] = '8';
							break;

						case 9: ArrHard[9]++; BoardHard[i][j] = '9';
							break;

						case 10: ArrHard[10]++; BoardHard[i][j] = 'A';
							break;

						case 11: ArrHard[11]++; BoardHard[i][j] = 'B';
							break;

						case 12: ArrHard[12]++; BoardHard[i][j] = 'C';
							break;

						case 13: ArrHard[13]++; BoardHard[i][j] = 'D';
							break;

						case 14: ArrHard[14]++; BoardHard[i][j] = 'E';
							break;

						case 15: ArrHard[15]++; BoardHard[i][j] = 'F';
							break;

						case 16: ArrHard[16]++; BoardHard[i][j] = 'G';
							break;

						case 17: ArrHard[17]++; BoardHard[i][j] = 'H';
							break;

						case 18: ArrHard[18]++; BoardHard[i][j] = 'I';
							break;

						case 19: ArrHard[19]++; BoardHard[i][j] = 'J';
							break;

						case 20: ArrHard[20]++; BoardHard[i][j] = 'K';
							break;

						case 21: ArrHard[21]++; BoardHard[i][j] = 'L';
							break;

						case 22: ArrHard[22]++; BoardHard[i][j] = 'M';
							break;

						case 23: ArrHard[23]++; BoardHard[i][j] = 'N';
							break;

						case 24: ArrHard[24]++; BoardHard[i][j] = 'O';
							break;

						case 25: ArrHard[25]++; BoardHard[i][j] = 'P';
							break;

						case 26: ArrHard[26]++; BoardHard[i][j] = 'Q';
							break;

						case 27: ArrHard[27]++; BoardHard[i][j] = 'R';
							break;

						case 28: ArrHard[28]++; BoardHard[i][j] = 'S';
							break;

						case 29: ArrHard[29]++; BoardHard[i][j] = 'T';
							break;

						case 30: ArrHard[30]++; BoardHard[i][j] = 'U';
							break;

						case 31: ArrHard[31]++; BoardHard[i][j] = 'V';
							break;

						case 32: ArrHard[32]++; BoardHard[i][j] = 'W';
							break;

						case 33: ArrHard[33]++; BoardHard[i][j] = 'X';
							break;

						case 34: ArrHard[34]++; BoardHard[i][j] = 'Y';
							break;

						case 35: ArrHard[35]++; BoardHard[i][j] = 'Z';
							break;

						case 36: ArrHard[36]++; BoardHard[i][j] = 'a';
							break;

						case 37: ArrHard[37]++; BoardHard[i][j] = 'b';
							break;

						case 38: ArrHard[38]++; BoardHard[i][j] = 'c';
							break;

						case 39: ArrHard[39]++; BoardHard[i][j] = 'd';
							break;

						case 40: ArrHard[40]++; BoardHard[i][j] = 'e';
							break;

						case 41: ArrHard[41]++; BoardHard[i][j] = 'f';
							break;

						case 42: ArrHard[42]++; BoardHard[i][j] = 'g';
							break;

						case 43: ArrHard[43]++; BoardHard[i][j] = 'h';
							break;

						case 44: ArrHard[44]++; BoardHard[i][j] = 'i';
							break;

						case 45: ArrHard[45]++; BoardHard[i][j] = 'j';
							break;

						case 46: ArrHard[46]++; BoardHard[i][j] = 'k';
							break;

						case 47: ArrHard[47]++; BoardHard[i][j] = 'l';
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

// ���ڿ� ���� ī���� �̹����� ����ϴ� �Լ� (�� ĭ�� �����, ī�尡 �ִ� ĭ�� ������� ���)
void CardRender()
{
	switch (diff)
	{
	case Easy: for (int i = 0; i < EASY_HEIGHT; i++)
				{
					for (int j = 0; j < EASY_WIDTH; j++)
					{
						if (BoardEasy[i][j] == 'x')
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
							printf("  ");
						}
						else
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						}
						
						if (BoardEasy[i][j] == '0')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '1')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '2')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '3')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '4')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '5')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '6')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '7')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '8')
						{
							printf("��");
						}
						else if (BoardEasy[i][j] == '9')
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
			if (BoardNormal[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (BoardNormal[i][j] == '0')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '1')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '2')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '3')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '4')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '5')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '6')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '7')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '8')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == '9')
			{
				printf("��");
			}
			else if (BoardNormal[i][j] == 'A')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'B')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'C')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'D')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'E')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'F')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'G')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'H')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'I')
			{
				printf("�"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'J')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'K')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'L')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'M')
			{
				printf("��"); // Ư������ ����
			}
			else if (BoardNormal[i][j] == 'N')
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
			if (BoardHard[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			
			if (BoardHard[i][j] == '0')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '1')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '2')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '3')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '4')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '5')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '6')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '7')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '8')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == '9')
			{
				printf("��");
			}
			else if (BoardHard[i][j] == 'A')
			{
				printf("��"); // Ư������ ���� A
			}
			else if (BoardHard[i][j] == 'B')
			{
				printf("��"); // Ư������ ���� B
			}
			else if (BoardHard[i][j] == 'C')
			{
				printf("��"); // Ư������ ���� C
			}
			else if (BoardHard[i][j] == 'D')
			{
				printf("��"); // Ư������ ���� D
			}
			else if (BoardHard[i][j] == 'E')
			{
				printf("��"); // Ư������ ���� E
			}
			else if (BoardHard[i][j] == 'F')
			{
				printf("��"); // Ư������ ���� F
			}
			else if (BoardHard[i][j] == 'G')
			{
				printf("��"); // Ư������ ���� G
			}
			else if (BoardHard[i][j] == 'H')
			{
				printf("��"); // Ư������ ���� H
			}
			else if (BoardHard[i][j] == 'I')
			{
				printf("��"); // Ư������ ���� I
			}
			else if (BoardHard[i][j] == 'J')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'K')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'L')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'M')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'N')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'O')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'P')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'Q')
			{
				printf("�"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'R')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'S')
			{
				printf("Ϣ"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'T')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'U')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'V')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'W')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'X')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'Y')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'Z')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'a')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'b')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'c')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'd')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'e')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'f')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'g')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'h')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'i')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'j')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'k')
			{
				printf("��"); // Ư������ ���� J
			}
			else if (BoardHard[i][j] == 'l')
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
			switch (i)
			{
			case 3: printf("��");
				break;

			case 2: printf("��");
				break;

			case 1: printf("��");
				break;
				
			default:
				break;
			}
			Sleep(1000);
			system("cls");
		}
		else
		{
			printf("�ǣ�!!");
			Sleep(500);
			system("cls");
		}
	}
}

// ī���� ¦�� �´��� Ȯ���ϴ� �Լ�
void Result()
{ 
	switch (diff)
	{
	case Easy: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { Count--; BoardEasy[Memory[1]][Memory[2]] = 'x'; BoardEasy[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			 else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			 if (Count == 0) { Scene++; }
			 break;

	case Normal: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { Count--; BoardNormal[Memory[1]][Memory[2]] = 'x'; BoardNormal[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			   else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			   if (Count == 0) { Scene++; }
			   break;

	case Hard: if (Memory[0] == Memory[3]) {
		if (Memory[1] != Memory[4] || Memory[2] != Memory[5]) { Count--; BoardHard[Memory[1]][Memory[2]] = 'x'; BoardHard[Memory[4]][Memory[5]] = 'x'; Check--; }
		else if (Memory[1] == Memory[4] && Memory[2] == Memory[5]) { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }
	}
			 else { int i = 0; while (i < 6) { Memory[i] = 'z'; i++; } Check--; }

			 if (Count == 0) { Scene++; }
			 break;

	default:
		break;
	}
}

// ������ ī�� �̹����� ����ϴ� �Լ�
char ShowCard(CurrentCard * Current)
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

// �ٽ� �ϱ� ȭ�� ��ġ �Լ�
void CreateReplay()
{
	strcpy(Replay[0], "011111111110");
	strcpy(Replay[1], "010000000010");
	strcpy(Replay[2], "010REPLAY010");
	strcpy(Replay[3], "010000000010");
	strcpy(Replay[4], "011111111110");
	strcpy(Replay[5], "000000000000");
	strcpy(Replay[6], "011111111110");
	strcpy(Replay[7], "010000000010");
	strcpy(Replay[8], "0100EXIT0010");
	strcpy(Replay[9], "010000000010");
	strcpy(Replay[10], "011111111110");
}

// �ٽ� �ϱ� ȭ�� ���� & �̹��� ���� �� ��� �Լ�
void ReplayRender(SelectCard * Select)
{
	printf("�ǣ��ͣ� �ạ̃ţ���!!\n\n");
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (Select->y == 4)
			{
				if (i >= 0 && i <= 4 && Replay[i][j] == '1')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else if (i >= 5)
				{
					if (Replay[i][j] == '1')
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (Select->y == 10)
			{
				if (i >= 6 && i <= 10 && Replay[i][j] == '1')
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
				}
				else if (i <= 5)
				{
					if (Replay[i][j] == '1')
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}

			if (Replay[i][j] == '0')
			{
				printf("  ");
			}
			else if (Replay[i][j] == '1')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'R')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'E')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'P')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'L')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'A')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'Y')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'X')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'I')
			{
				printf("��");
			}
			else if (Replay[i][j] == 'T')
			{
				printf("��");
			}
		}
		printf("\n");
	}
}

// Ű���� �Լ�
void Keyboard(SelectCard * Select)
{
	int key = 0;
	if (_kbhit()) // Ű���� �Է� Ȯ�� �Լ�(true / false)
	{
		key = _getch(); // key �Է��� �޾��ִ� �Լ�
		if (key == 224)
		{
			key = _getch();
		}
		switch (key)
		{
		case SPACE:	   if (Scene == 0 && Select->y == 2)  { diff = Easy; Scene++; Count = 10; }
				  else if (Scene == 0 && Select->y == 8)  { diff = Normal; Scene++; Count = 24; }
				  else if (Scene == 0 && Select->y == 14) { diff = Hard; Scene++; Count = 48; }
				  else if (Scene == 1 && Check == 0 && diff == Easy && BoardEasy[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = BoardEasy[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 0 && diff == Normal && BoardNormal[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = BoardNormal[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 0 && diff == Hard && BoardHard[Select->y][Select->x / 2 + 1] != 'x') { Check++; Memory[0] = BoardHard[Select->y][Select->x / 2 + 1]; Memory[1] = Select->y; Memory[2] = Select->x / 2 + 1; }
				  else if (Scene == 1 && Check == 1 && diff == Easy && BoardEasy[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = BoardEasy[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Result(); }
				  else if (Scene == 1 && Check == 1 && diff == Normal && BoardNormal[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = BoardNormal[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Result(); }
				  else if (Scene == 1 && Check == 1 && diff == Hard && BoardHard[Select->y][Select->x / 2 + 1] != 'x') { Memory[3] = BoardHard[Select->y][Select->x / 2 + 1]; Memory[4] = Select->y; Memory[5] = Select->x / 2 + 1; Result(); }
				  else if (Scene == 2 && Select->y == 4)  { Scene = 0; }
				  else if (Scene == 2 && Select->y == 10) { exit(0); }
			break;


		case UP:	   if (Scene == 0 && Select->y - 6 >= 0) { Select->y -= 6; }
				  else if (Scene == 1 && Select->y / 2 - 1 >= 0) { Select->y -= 2; }
				  else if (Scene == 2 && Select->y - 6 >= 0) { Select->y -= 6; }
			break;

		case DOWN:	   if (Scene == 0 && Select->y + 6 <= 14) { Select->y += 6; }
				  else if (Scene == 1 && diff == Easy && Select->y / 2 + 1 < EASY_HEIGHT / 2) { Select->y += 2; }
				  else if (Scene == 1 && diff == Normal && Select->y / 2 + 1 < NORMAL_HEIGHT / 2) { Select->y += 2; }
				  else if (Scene == 1 && diff == Hard && Select->y / 2 + 1 < HARD_HEIGHT / 2) { Select->y += 2; }
				  else if (Scene == 2 && Select->y + 6 <= 10) { Select->y += 6; }
			break;

		case LEFT:	   if (Scene == 1 && Select->x / 4 - 1 >= 0) { Select->x -= 4; }
			break;

		case RIGHT:	   if (Scene == 1 && diff == Easy && Select->x / 4 + 1 < EASY_WIDTH / 2) { Select->x += 4; }
				  else if (Scene == 1 && diff == Normal && Select->x / 4 + 1 < NORMAL_WIDTH / 2) { Select->x += 4; }
				  else if (Scene == 1 && diff == Hard && Select->x / 4 + 1 < HARD_WIDTH / 2) { Select->x += 4; }
			break;

		default:
			break;
		}
	}
}

int main()
{
	CursorView();
	SelectCard Select = { 0, 2, "��"};
	CurrentCard Current = { 0, 0, "������ ī�� : ", ' '};
	CreateMain();
	CreateReplay();
	while (1)
	{
		Select.y = 2;
		while (Scene == 0)
		{
			MainRender(&Select);
			Keyboard(&Select);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);
			Sleep(100);
			system("cls");
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		CreateCard();
		Countdown();
		Select.y = 0;

		while (Scene == 1)
		{
			CardRender();
			Keyboard(&Select);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);

			switch (diff)
			{
			case Easy: Current.y = 8;
				break;

			case Normal: Current.y = 12;
				break;

			case Hard: Current.y = 16;
				break;

			default:
				break;
			}

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
			ShowCard(&Current);
			Sleep(100);
			system("cls");
		}

		Select.x = 0;
		Select.y = 4;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		Sleep(200);
		
		while (Scene == 2)
		{
			ReplayRender(&Select);
			Keyboard(&Select);
			GotoXY(Select.x, Select.y);
			printf("%s", Select.shape);
			Sleep(100);
			system("cls");
		}
	}
	return 0;
}