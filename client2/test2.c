#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <conio.h>
#include <windows.h>
#include "myconsole.h"
#pragma comment(lib, "ws2_32")

//int sharedData = 0;
//pthread_mutex_t mutex;
//void push() {
//	for (int i = 0; i < 100000; i++) {
//		pthread_mutex_lock(&mutex);
//		sharedData++;
//		pthread_mutex_unlock(&mutex);
//	}
//}

typedef struct _ChatMessage {
	int userid;
	char name[20];
	char msg[100];
} ChatMessage;

ChatMessage* msg_list[100];
int msg_size = 0;
int is_first = 1;

int user_id;
char name[20];

char chat_tmp[100];
int tmp_idx = 0;

pthread_mutex_t mutex;

char* printChatConsole() {

	char* msg = run_my_console();
	return msg;
}
void send_msg(SOCKET csock) {
	pthread_mutex_lock(&mutex);
	printf("========================================================\n");
	printf("보낼 메시지 입력 : ");
	while (1) {
		char sbuff[1024] = { 0 };
		char* msg = printChatConsole();
		//fgets(sbuff, 100, stdin);
		ChatMessage chat_message;
		chat_message.userid = user_id;
		memcpy(chat_message.name, name, sizeof(chat_message.name));
		memcpy(chat_message.msg, msg, sizeof(chat_message.msg));
		memcpy(sbuff, &chat_message, sizeof(chat_message));
		send(csock, sbuff, 1024, 0);
		is_first = 0;
	}

}

void recv_msg(SOCKET csock) {

	pthread_mutex_lock(&mutex);
	while (1) {
		if (is_first) {
			continue;
		}

		char rbuff[1024] = { 0 };
		recv(csock, rbuff, 1024, 0);

		ChatMessage* chat_message = (ChatMessage*)malloc(sizeof(ChatMessage));
		memcpy(chat_message, rbuff, sizeof(ChatMessage));
		msg_list[msg_size++] = chat_message;

		system("cls");
		for (int i = 0; i < msg_size; i++) {
			printf("%s(%d) : %s", msg_list[i]->name, msg_list[i]->userid, msg_list[i]->msg);
		}
		sety(msg_size);
		int tox = get_tmp_msg_size();
		char* tmp_msg = get_msg();
		printf("========================================================\n");
		printf("보낼 메시지 입력 : ");
		printf("%s", tmp_msg);
		setx(tox);

	}
	pthread_mutex_unlock(&mutex);
}

void init(SOCKET csock) {
	char rbuff[100] = { 0 };
	recv(csock, rbuff, 100, 0);
	memcpy(&user_id, rbuff, sizeof(user_id));
	/*unsigned char chat_size = sizeof(ChatMessage);

	ChatMessage chat_message;
	memcpy(&chat_message, rbuff, chat_size);*/


	printf("이름을 입력해주세요 : ");
	scanf_s("%s", name, 20);
	printf("안녕하세요 %s(%d)님!", name, user_id);
	printf("계속하려면 엔터를 누르세요\n");

	while (1) {
		char c = _getch();
		if (c == 13) {
			system("cls");
			break;
		}
	}


}

int main() {

	/*pthread_t threads[2];
	pthread_mutex_init(&mutex, NULL);

	if (pthread_create(&threads[0], NULL, push, NULL) != 0) {
		fprintf(stderr, "Error");
	}

	if (pthread_create(&threads[1], NULL, push, NULL) != 0) {
		fprintf(stderr, "Error");
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	printf("%d\n", sharedData);*/


	WSADATA wsadata;

	// 초기화 성공 0, 실패 -1
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		printf("winsock 초기화 실패\n");
		return -1;
	}

	printf("winsock 초기화 성공!!\n");

	// 3. 소켓 생성과 닫기.
	// socket(af, type, protocol) - 소켓을 생성해주는 함수.
	// af : 주소체계 (ipv4 - AF_INET, ipv6 - AF_INET6)
	// type : 소켓의 데이터 전송 타입 (TCP / UDP) - SOCK_STREAM, SOCK_DGRAM 
	// protocol : 프로토콜 선택(보통 0으로 해줌)  
	// closesocket(sock): 해당 소켓의 자원 반납

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR) {
		printf("소켓 생성시 문제 발생!!\n");
		return -1;
	}

	printf("소켓 생성 완료!!\n");


	// 4. 접속하고자 하는 서버에 대한 주소 세팅
	SOCKADDR_IN addr; // 접속 하고자하는 대상 서버 주소
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 127.0.0.1

	// 5. 서버에 연결 요청 - connect()
	// connect(sock, addr, addrlen)
	// sock - 클라이언트 소켓
	// addr - 접속할 서버의 주소값 구조체의 포인터
	// addrlen - addr의 크기

	if (connect(sock, (SOCKADDR_IN*)&addr, sizeof(addr)) != 0) {
		printf("서버에 연결 시도중 문제 발생!!\n");
		exit(1);
	}

	printf("%d\n", sock);
	printf("서버에 연결 되었습니다!\n");
	init(sock);

	// 6. 데이터 송수신
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


	return 0;
}