#include <stdio.h>
#include <windows.h>
#include <conio.h>  // kbhit와 getch 함수를 사용하기 위해 필요

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int x_pos = 20;
int y_pos = 0;

void setxy(int x, int y) {
	x_pos = x;
	y_pos = y;
}

int getx() {
	return x_pos;
}

int gety() {
	return y_pos;
}

int setx(int x) {
	x_pos = 20 + x;
}

int sety(int y) {
	y_pos = y;
}

static char msg[1000];
static char tmp_msg[1000];
static int tmp_msg_size = 0;

char* get_msg() {
	return tmp_msg;
}

int get_tmp_msg_size() {
	return tmp_msg_size - 1;
}

char* run_my_console() {

	int ch;
	int msg_idx = 0;

	while (1) {
		// kbhit()는 키보드 입력이 있는지 확인합니다.

		if (_kbhit()) {

			ch = _getch();  // getch()는 키보드 입력을 읽어옵니다.

			// 'q' 키가 입력되면 루프를 종료합니다.
			if (ch == '0') {
				break;
			}

			if (ch == 13) {
				tmp_msg[msg_idx++] = '\n';
				tmp_msg[msg_idx] = '\0';
				memcpy(msg, tmp_msg, sizeof(msg));
				y_pos++;
				x_pos = 20;
				tmp_msg[0] = '\0';
				tmp_msg_size = 0;
				return msg;
			}

			else if (ch == 8 && x_pos > 20) {
				msg_idx--;
				gotoxy(--x_pos, y_pos);
				printf("%c", ' ');
				gotoxy(x_pos, y_pos);
			}
			else if (ch != 8) {
				tmp_msg[msg_idx++] = ch;
				tmp_msg_size++;
				gotoxy(x_pos, y_pos);
				x_pos++;
				printf("%c", ch);
			}

		}
	}

	printf("\nExiting program.\n");
	return;
}