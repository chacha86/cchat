#include <stdio.h>
#define NAME_SIZE 20

void input_name_view(int user_id, wchar_t* name) {
	
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

void message_input_view() {
	printf("========================================================\n");
	printf("���� �޽��� �Է� : ");
}