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

SOCKET sock; // 클라이언트(유저) 소켓
wchar_t name[20]; // 클라이언트(유저) 이름

ChatMessage* msg_list[100]; // 화면 다시 그릴 때 필요한 메시지 데이터 저장소
int msg_size = 0; // 메시지의 개수를 저장하기 위한 변수

// ChatMessage 구조체에 채팅에 필요한 정보들을 세팅해주는 함수
void set_msg_data(int userid, wchar_t* name, wchar_t* msg, char* sbuff) {
	ChatMessage chat_message;
	chat_message.userid = userid;
	memcpy(chat_message.name, name, sizeof(chat_message.name));
	memcpy(chat_message.msg, msg, sizeof(chat_message.msg));
	memcpy(sbuff, &chat_message, sizeof(ChatMessage));
}

// 나중에 전체 메시지를 다시 그릴 때 필요한 메시지 데이터를 배열에 저장해주는 함수
ChatMessage* add_msg(char* buff) {
	ChatMessage* chat_message = (ChatMessage*)malloc(sizeof(ChatMessage));
	memcpy(chat_message, buff, sizeof(ChatMessage));
	msg_list[msg_size++] = chat_message;
	return chat_message;
}

// myconsole을 이용해 실시간 입력된 메시지를 서버에 보내기 위한 함수.
// 메시지를 받는 행위와는 별개로 이루어져야 하므로 멀티스레드 처리 필요
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

// 서버로 부터 수신받은(다른 유저의 메시지) 메시지를 배열에 저장하고 전체 메시지를 출력해주는 함수
// 이때 입력중이던 메시지가 있다면 restore로 복구
// 메시지를 보내는 행위와는 별개로 이루어져야 하므로 멀티스레드 처리 필요
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
	setlocale(LC_ALL, ""); // 제대로 된 한글 처리를 위해 설정해줘야 함.

	sock = new_socket("127.0.0.1", 9999); // 127.0.0.1 서버에 9999번 포트로 접속(서버 설정과 맞아야 함)
	myconsole__input_name(sock, name); // 유저 이름을 설정하는 함수 호출

	// 위에서 언급한 것처럼 메시지 보내기와 받기는 따로 동시에 이루어져야 하므로 두 함수를 멀티스레드 처리함.
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, send_msg, sock);
	pthread_create(&t2, NULL, recv_msg, sock);

	// t1, t2 스레드가 같이 끝나기를 기다림
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	// 종료를 위해 자원 반납
	closesocket(sock);
	printf("소켓 닫기 완료!!\n");

	WSACleanup();
	printf("winsock 자원 반납\n");

	// 종료를 막기 위한 무한 루프. 실제로 종료하려면 break를 처리해야함.
	while (1);
}