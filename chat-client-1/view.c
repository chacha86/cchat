#include <stdio.h>
#define NAME_SIZE 20

// 채팅 사용자의 이름을 입력하기 위한 화면
void view__input_name(int user_id, wchar_t* name) {
	
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

// 채팅 메시지를 입력하기 위한 기본 입력 화면
void view__input_message(wchar_t* tmp_msg, int tmp_msg_size) {
	printf("========================================================\n");
	printf("보낼 메시지 입력 :  ");
	for (int i = 0; i < tmp_msg_size; i++) {
		wprintf(L"%lc", tmp_msg[i]);
	}
}