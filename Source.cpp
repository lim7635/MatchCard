#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 32

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

// 카드 2차원 배열(40장)
char Card[4][10];

typedef struct SelectCard
{
	int x;
	int y;
	const char* shape;
}SelectCard;

// 카드 배치
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
	SelectCard Select = { 0, 0, "□" };

	return 0;
}