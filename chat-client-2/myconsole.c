#include <stdio.h>
#include <windows.h>
#include <conio.h>  // kbhit�� getch �Լ��� ����ϱ� ���� �ʿ�
#define X_START_POS 20 // Ŀ���� ó�� �����ϴ� x�� ��ġ
#define Y_START_POS 1 // Ŀ���� ó�� �����ϴ� y�� ��ġ

// ������� �ܼ� �Լ��� x, y�� ��ǥ������ �޾� Ŀ���� �ܼ�â�� Ư�� ��ġ�� �ű� �� ����.
static void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static int x_pos = X_START_POS; // x�� ��ǥ�� ó���ϱ� ���� ����
static int y_pos = Y_START_POS; // y�� ��ǥ�� ó���ϱ� ���� ����
static int res_x_pos = 0; // �Է����̴� �޽����� ���̸� ����ϱ� ���� ����

static wchar_t tmp_msg[1000]; // �Է����̴� �޽����� �����ϱ� ���� �迭. �Է����̴� �޽����� ���ư� ��� �ٽ� �Է��ϴ� ���� ���� ����
static int tmp_msg_size = 0; // �Է����̴� �޽����� ���̸� �����ϱ� ���� ����

// �ʱ�ȭ �Լ�
void myconsole__init(int x, int y) {
	x_pos = x;
	y_pos = y;
}

// x�� Ŀ�� ��ġ ��������
int myconsole__getx() {
	return x_pos;
}

// y�� Ŀ�� ��ġ ��������
int myconsole__gety() {
	return y_pos;
}

// x�� Ŀ�� ��ġ �����ϱ�
int myconsole__setx(int x) {
	x_pos = x;
}

// y�� Ŀ�� ��ġ �����ϱ�
int myconsole__sety(int y) {
	y_pos = y;
}

// �� ä�� �Է�â(view__input_message)�� ���� �ٿ� ������ �ϱ� ���� Ŀ�� ����
void myconsole__nextline() {
	x_pos = X_START_POS;
	y_pos++;
}

// �Է����̴� �޽����� �ֿܼ��� ������� �� �����ϱ� ����(���� �޽��� �Է��� �ٸ� ������� �޽����� ���� �� �޽����� �ֿܼ��� �������µ� �̸� �����ϱ� ����)
void myconsole__restore_input() {
	view__input_message(tmp_msg, tmp_msg_size);
	if (res_x_pos > 0) {
		myconsole__setx(res_x_pos);
	}
}

// ����� �̸��� �Է��ϱ� ���� �Լ�
void myconsole__input_name(int user_id, wchar_t* name) {
	view__input_name(user_id, name);
}

// �ǽð��� �Էµ� Ű���� �м��ؼ� �ܼ� �޽��� �Է� â�� �����ִ� �Լ�. 
char* myconsole__input(wchar_t* msg, int msg_size) {

	wchar_t ch;
	int msg_idx = 0;

	view__input_message(tmp_msg, tmp_msg_size);
	while (1) {
		// kbhit()�� Ű���� �Է��� �ִ��� Ȯ���մϴ�.

		if (_kbhit()) {

			ch = _getwch();  // getch()�� Ű���� �Է��� �о�ɴϴ�.

			// 'q' Ű�� �ԷµǸ� �����մϴ�.
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