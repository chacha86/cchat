#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <wchar.h>
#pragma comment(lib, "ws2_32")

#define BUFFER_SIZE 1024

typedef struct _ChatMessage {
	int userid;
	wchar_t name[20];
	wchar_t msg[100];
} ChatMessage;

SOCKET* client_list;
int client_size;

void init() {
	int conn_size = socket__get_conn_size();
	client_list = (SOCKET*)malloc(sizeof(SOCKET) * conn_size);
	client_size = 0;
}

void destroy() {
	for (int i = 0; i < client_size; i++) {
		free(client_list[i]);
	}
}

ChatMessage get_msg_data(wchar_t* buff) {
	ChatMessage chat_message;
	memcpy(&chat_message, buff, sizeof(chat_message));
	return chat_message;
}

void recv_msg(SOCKET client) {
	while (1) {

		wchar_t buffer[BUFFER_SIZE] = { 0 };

		int recv_byte_size = recv(client, buffer, BUFFER_SIZE, 0);
		if (recv_byte_size < 0) {
			printf("데이터 수신 중 이상 발생!\n");
			return;
		}

		for (int i = 0; i < client_size; i++) {
			send(client_list[i], buffer, BUFFER_SIZE, 0);
		}

	}
}

void create_acc_threads(SOCKET server) {

	SOCKET client = socket__my_accept(server);
	client_list[client_size++] = client;

	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, recv_msg, client);
	pthread_join(recv_thread, NULL);

	closesocket(server);
	printf("소켓 닫기 완료!!\n");

	WSACleanup();
	printf("winsock 자원 반납\n");

	/*for (int i = 0; i < conn_size; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, socket__my_accept, server);
		pthread_detach(thread);
	}*/
}

int main() {
	init();

	SOCKET server = socket__new_socket();
	int conn_size = socket__get_conn_size();

	for (int i = 0; i < conn_size; i++) {
		pthread_t acc_thread;
		pthread_create(&acc_thread, NULL, create_acc_threads, server);
		pthread_detach(acc_thread);
	}

	while (1);

	destroy();

	/*while (1) {
		wchar_t buff[1024];
		recv(client, buff, 1024, 0);
		ChatMessage chat_message = get_msg_data(buff);
		wprintf(L"%ls : %ls", chat_message.name, chat_message.msg);
		send(client, buff, 1024, 0);
	}*/
}
