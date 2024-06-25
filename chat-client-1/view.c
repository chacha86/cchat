#include <stdio.h>
#define NAME_SIZE 20

void input_name_view(int user_id, wchar_t* name) {
	
	printf("이름을 입력해주세요 : ");
	wscanf_s(L"%ls", name, NAME_SIZE);
	wprintf(L"안녕하세요 %ls(%d)님!", name, user_id);
	printf("계속하려면 엔터를 누르세요\n");

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
	printf("보낼 메시지 입력 : ");
}