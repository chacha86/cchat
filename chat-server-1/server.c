#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <wchar.h>
#pragma comment(lib, "ws2_32")

// �����͸� �ְ� ���� ������� ũ�⸦ �����ϱ� ����
#define BUFFER_SIZE 1024

// ä�� �޽��� �����ʹ� ���� ������ �̷�����Ƿ� ����ü�� Ȱ��
typedef struct _ChatMessage {
	int userid; // ���� ��ȣ
	wchar_t name[20]; // ���� �̸�
	wchar_t msg[200]; // ä�� �޽���
} ChatMessage;

static SOCKET* client_list; // ��Ƽ ä�� ������ ���� ������ �����ؾ� �ϹǷ� ������ �����ϱ� ���� �迭 �ּ�
static int client_size; // ������ ��

// �ʿ��� ������ �ʱ�ȭ �ϱ� ���� �ʱ�ȭ �Լ�
void init() {
	int conn_size = socket__get_conn_size();
	client_list = (SOCKET*)malloc(sizeof(SOCKET) * conn_size);
	client_size = 0;
}

// �� ����� �޸𸮸� ��ȯ�ϱ� ���� �Լ�. ���α׷� ���� ���� ������ ����.
void destroy() {
	for (int i = 0; i < client_size; i++) {
		free(client_list[i]);
	}
}

// buff ����ҷ� ���� chat_message�� �޸� ������ ����.
ChatMessage get_msg_data(char* buff) {
	ChatMessage chat_message;
	memcpy(&chat_message, buff, sizeof(chat_message));
	return chat_message;
}

// Ŭ���̾�Ʈ�� �μ��� �޾� �ش� Ŭ���̾�Ʈ�� ���� �����͸� �����ϴ� �Լ�.
// ������ �����ʹ� ������ �����ϴ� ��� �������� �ٽ� ������. ä�� �޽����� ��ο��� �����ϱ� ����
// �ش� �Լ��� ���� Ŭ���̾�Ʈ���� �����͸� ������ �� �־�� �ϹǷ� ��Ƽ������� ����Ǿ�� ��.
void recv_msg(SOCKET client) {
	while (1) {

		char buffer[BUFFER_SIZE] = { 0 };

		printf("���� �����..\n");
		int recv_byte_size = recv(client, buffer, BUFFER_SIZE, 0);
		if (recv_byte_size < 0) {
			printf("������ ���� �� �̻� �߻�!\n");
			return;
		}

		for (int i = 0; i < client_size; i++) {
			send(client_list[i], buffer, BUFFER_SIZE, 0);
		}

	}
}

// ���� ������ �޾� �ش� ������ Ŭ���̾�Ʈ�� �����ϰ� Ŭ���̾�Ʈ���� ������ ���� �κ��� ��Ƽ������� �и��ϴ� �Լ�
void create_acc_threads(SOCKET server) {

	SOCKET client = socket__my_accept(server);
	client_list[client_size++] = client;

	// ������ ����� ��ó�� recv_msg�� ��Ƽ������� ���� ����Ǿ�� �ϴ� �Լ��̹Ƿ� ��Ƽ������ ó��.
	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, recv_msg, client);
	pthread_join(recv_thread, NULL); // recv_thread�� �Ϸ�Ǹ� ��ٷȴٰ� ������.

	closesocket(server);
	printf("���� �ݱ� �Ϸ�!!\n");

	WSACleanup();
	printf("winsock �ڿ� �ݳ�\n");

	/*for (int i = 0; i < conn_size; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, socket__my_accept, server);
		pthread_detach(thread);
	}*/
}

int main() {
	init();

	SOCKET server = socket__new_socket(); // ���ο� ä�� ���� ����
	int conn_size = socket__get_conn_size(); // �ش� ������ �����ϴ� �ִ� ������ ���ϱ�

	// ���� ����ŭ ������ ������ ������ �� �ֵ��� �ϴ� �ڵ�.
	// ���ÿ� ���� ������ ����Ǿ�� �ϹǷ� ������ �����ϴ� �Լ��� create_acc_threads �Լ��� ��Ƽ������� ó��.
	for (int i = 0; i < conn_size; i++) {
		pthread_t acc_thread;
		pthread_create(&acc_thread, NULL, create_acc_threads, server);
		pthread_detach(acc_thread); // ��׶��忡�� ������ ó��
	}

	while (1); // ������ ������� �ʵ��� ó��. ���� break ó�� ����� ��.

	destroy(); // ���� ����� �ڿ� �ݳ�
}
