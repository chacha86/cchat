#include <stdio.h>
#include <windows.h>
#include <conio.h>  // kbhit와 getch 함수를 사용하기 위해 필요
#define X_START_POS 20
#define Y_START_POS 1


static void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static int x_pos = X_START_POS;
static int y_pos = Y_START_POS;
static int res_x_pos = 0;

void myconsole__init(int x, int y) {
	x_pos = x;
	y_pos = y;
}

int myconsole__getx() {
	return x_pos;
}

int myconsole__gety() {
	return y_pos;
}

int myconsole__setx(int x) {
	x_pos = x;
}

int myconsole__sety(int y) {
	y_pos = y;
}

static wchar_t tmp_msg[1000];
static int tmp_msg_size = 0;

char* myconsole__get_tmp_msg() {
	return tmp_msg;
}

int myconsole__get_tmp_msg_size() {
	return tmp_msg_size - 1;
}

void myconsole__nextline() {
	x_pos = X_START_POS;
	y_pos++;
}

void myconsole__restore_input() {
	view__input_message(tmp_msg, tmp_msg_size);
	if (res_x_pos > 0) {
		myconsole__setx(res_x_pos);
	}
}
char* myconsole__input(wchar_t* msg, int msg_size) {

	wchar_t ch;
	int msg_idx = 0;

	view__input_message(tmp_msg, tmp_msg_size);
	while (1) {
		// kbhit()는 키보드 입력이 있는지 확인합니다.

		if (_kbhit()) {

			ch = _getwch();  // getch()는 키보드 입력을 읽어옵니다.

			// 'q' 키가 입력되면 종료합니다.
			if (ch == 27) {
				exit(1);
			}

			if (ch == 13 || tmp_msg_size >= msg_size) {
				tmp_msg[tmp_msg_size++] = L'\0';

				memcpy(msg, tmp_msg, msg_size);
				memset(tmp_msg, '\0', 1000);
				tmp_msg_size = 0;
				res_x_pos = 0;

				return;
			}

			else if (ch == L'\b' && x_pos > 20) {
				tmp_msg_size--;

				gotoxy(--x_pos, y_pos);
				printf("%c", ' ');
				gotoxy(x_pos, y_pos);
			}
			else if (ch != L'\b') {
				tmp_msg[tmp_msg_size++] = ch;
				gotoxy(x_pos, y_pos);

				if (ch >= 32 && ch < 127) {
					x_pos++;
				}
				else {
					x_pos += 2;
				}

				res_x_pos = x_pos;
				wprintf(L"%lc", ch);
			}
		}
	}
	return;
}