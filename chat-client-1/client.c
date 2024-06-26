#define HAVE_STRUCT_TIMESPEC
#include <locale.h>
#include <WinSock2.h>
#include <wchar.h>
#include <pthread.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

typedef struct _ChatMessage {
	int userid;
	wchar_t name[20];
	wchar_t msg[200];
} ChatMessage;

// 클라이언트 유저 정보
SOCKET sock;
wchar_t name[20];

// 화면 다시 그릴 때 필요한 메시지 데이터 저장소
ChatMessage* msg_list[100];
int msg_size = 0;

void set_msg_data(int userid, wchar_t* name, wchar_t* msg, char* sbuff) {
	ChatMessage chat_message;
	chat_message.userid = userid;
	memcpy(chat_message.name, name, sizeof(chat_message.name));
	memcpy(chat_message.msg, msg, sizeof(chat_message.msg));
	memcpy(sbuff, &chat_message, sizeof(ChatMessage));
}

ChatMessage* add_msg(char* buff) {
	ChatMessage* chat_message = (ChatMessage*)malloc(sizeof(ChatMessage));
	memcpy(chat_message, buff, sizeof(ChatMessage));
	msg_list[msg_size++] = chat_message;
	return chat_message;
}

void send_msg() {
	//view__input_message(NULL);
	while (1) {
		char sbuff[BUFFER_SIZE] = { 0 };
		wchar_t msg[200];
		//fgetws(msg, 200, stdin);
		myconsole__input(msg, 200);
		set_msg_data(sock, name, msg, sbuff);
		send(sock, sbuff, BUFFER_SIZE, 0);
	}
}

void recv_msg(SOCKET csock) {

	while (1) {
		char rbuff[1024] = { 0 };
		recv(csock, rbuff, 1024, 0);
		add_msg(rbuff);
		system("cls");
		for (int i = 0; i < msg_size; i++) {
			wprintf(L"%ls(%d) : %ls\n", msg_list[i]->name, msg_list[i]->userid, msg_list[i]->msg);
		}
		myconsole__nextline();
		myconsole__restore_input();
	}
}


int main() {
	setlocale(LC_ALL, "");

	sock = new_socket("127.0.0.1", 9999);

	view__input_name(sock, name);

	//wprintf(L"%ls", name);

	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, send_msg, sock);
	pthread_create(&t2, NULL, recv_msg, sock);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	closesocket(sock);
	printf("소켓 닫기 완료!!\n");

	WSACleanup();
	printf("winsock 자원 반납\n");



	while (1);
}