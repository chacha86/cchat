#include <stdio.h>
#include <windows.h>
#include <conio.h>  // kbhit와 getch 함수를 사용하기 위해 필요
#define X_START_POS 20 // 커서가 처음 시작하는 x축 위치
#define Y_START_POS 1 // 커서가 처음 시작하는 y축 위치

// 윈도우용 콘솔 함수로 x, y를 좌표값으로 받아 커서를 콘솔창의 특정 위치로 옮길 수 있음.
static void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static int x_pos = X_START_POS; // x축 좌표를 처리하기 위한 변수
static int y_pos = Y_START_POS; // y축 좌표를 처리하기 위한 변수
static int res_x_pos = 0; // 입력중이던 메시지의 길이를 기억하기 위한 변수

static wchar_t tmp_msg[1000]; // 입력중이던 메시지를 저장하기 위한 배열. 입력중이던 메시지가 날아갈 경우 다시 입력하는 것을 막기 위함
static int tmp_msg_size = 0; // 입력중이던 메시지의 길이를 저장하기 위한 변수

// 초기화 함수
void myconsole__init(int x, int y) {
	x_pos = x;
	y_pos = y;
}

// x축 커서 위치 가져오기
int myconsole__getx() {
	return x_pos;
}

// y축 커서 위치 가져오기
int myconsole__gety() {
	return y_pos;
}

// x축 커서 위치 세팅하기
int myconsole__setx(int x) {
	x_pos = x;
}

// y축 커서 위치 세팅하기
int myconsole__sety(int y) {
	y_pos = y;
}

// 내 채팅 입력창(view__input_message)이 다음 줄에 나오게 하기 위한 커서 설정
void myconsole__nextline() {
	x_pos = X_START_POS;
	y_pos++;
}

// 입력중이던 메시지가 콘솔에서 사라졌을 때 복구하기 위함(내가 메시지 입력중 다른 사람에게 메시지가 오면 내 메시지가 콘솔에서 지워지는데 이를 복구하기 위함)
void myconsole__restore_input() {
	view__input_message(tmp_msg, tmp_msg_size);
	if (res_x_pos > 0) {
		myconsole__setx(res_x_pos);
	}
}

// 사용자 이름을 입력하기 위한 함수
void myconsole__input_name(int user_id, wchar_t* name) {
	view__input_name(user_id, name);
}

// 실시간을 입력된 키값을 분석해서 콘솔 메시지 입력 창에 보여주는 함수. 
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