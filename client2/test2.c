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
	printf("���� �޽��� �Է� : ");
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
		printf("���� �޽��� �Է� : ");
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


	printf("�̸��� �Է����ּ��� : ");
	scanf_s("%s", name, 20);
	printf("�ȳ��ϼ��� %s(%d)��!", name, user_id);
	printf("����Ϸ��� ���͸� ��������\n");

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

	// �ʱ�ȭ ���� 0, ���� -1
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		printf("winsock �ʱ�ȭ ����\n");
		return -1;
	}

	printf("winsock �ʱ�ȭ ����!!\n");

	// 3. ���� ������ �ݱ�.
	// socket(af, type, protocol) - ������ �������ִ� �Լ�.
	// af : �ּ�ü�� (ipv4 - AF_INET, ipv6 - AF_INET6)
	// type : ������ ������ ���� Ÿ�� (TCP / UDP) - SOCK_STREAM, SOCK_DGRAM 
	// protocol : �������� ����(���� 0���� ����)  
	// closesocket(sock): �ش� ������ �ڿ� �ݳ�

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR) {
		printf("���� ������ ���� �߻�!!\n");
		return -1;
	}

	printf("���� ���� �Ϸ�!!\n");


	// 4. �����ϰ��� �ϴ� ������ ���� �ּ� ����
	SOCKADDR_IN addr; // ���� �ϰ����ϴ� ��� ���� �ּ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 127.0.0.1

	// 5. ������ ���� ��û - connect()
	// connect(sock, addr, addrlen)
	// sock - Ŭ���̾�Ʈ ����
	// addr - ������ ������ �ּҰ� ����ü�� ������
	// addrlen - addr�� ũ��

	if (connect(sock, (SOCKADDR_IN*)&addr, sizeof(addr)) != 0) {
		printf("������ ���� �õ��� ���� �߻�!!\n");
		exit(1);
	}

	printf("%d\n", sock);
	printf("������ ���� �Ǿ����ϴ�!\n");
	init(sock);

	// 6. ������ �ۼ���
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, send_msg, sock);
	pthread_create(&t2, NULL, recv_msg, sock);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	closesocket(sock);
	printf("���� �ݱ� �Ϸ�!!\n");

	WSACleanup();
	printf("winsock �ڿ� �ݳ�\n");


	return 0;
}