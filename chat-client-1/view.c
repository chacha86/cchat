#include <stdio.h>
#define NAME_SIZE 20

// ä�� ������� �̸��� �Է��ϱ� ���� ȭ��
void view__input_name(int user_id, wchar_t* name) {
	
	printf("�̸��� �Է����ּ��� : ");
	wscanf_s(L"%ls", name, NAME_SIZE);
	wprintf(L"�ȳ��ϼ��� %ls(%d)��!", name, user_id);
	printf("����Ϸ��� ���͸� ��������\n");

	while (1) {
		char c = _getch();
		if (c == 13) {
			system("cls");
			break;
		}
	}
}

// ä�� �޽����� �Է��ϱ� ���� �⺻ �Է� ȭ��
void view__input_message(wchar_t* tmp_msg, int tmp_msg_size) {
	printf("========================================================\n");
	printf("���� �޽��� �Է� :  ");
	for (int i = 0; i < tmp_msg_size; i++) {
		wprintf(L"%lc", tmp_msg[i]);
	}
}