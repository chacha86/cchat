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

SOCKET sock; // Ŭ���̾�Ʈ(����) ����
wchar_t name[20]; // Ŭ���̾�Ʈ(����) �̸�

ChatMessage* msg_list[100]; // ȭ�� �ٽ� �׸� �� �ʿ��� �޽��� ������ �����
int msg_size = 0; // �޽����� ������ �����ϱ� ���� ����

// ChatMessage ����ü�� ä�ÿ� �ʿ��� �������� �������ִ� �Լ�
void set_msg_data(int userid, wchar_t* name, wchar_t* msg, char* sbuff) {
	ChatMessage chat_message;
	chat_message.userid = userid;
	memcpy(chat_message.name, name, sizeof(chat_message.name));
	memcpy(chat_message.msg, msg, sizeof(chat_message.msg));
	memcpy(sbuff, &chat_message, sizeof(ChatMessage));
}

// ���߿� ��ü �޽����� �ٽ� �׸� �� �ʿ��� �޽��� �����͸� �迭�� �������ִ� �Լ�
ChatMessage* add_msg(char* buff) {
	ChatMessage* chat_message = (ChatMessage*)malloc(sizeof(ChatMessage));
	memcpy(chat_message, buff, sizeof(ChatMessage));
	msg_list[msg_size++] = chat_message;
	return chat_message;
}

// myconsole�� �̿��� �ǽð� �Էµ� �޽����� ������ ������ ���� �Լ�.
// �޽����� �޴� �����ʹ� ������ �̷������ �ϹǷ� ��Ƽ������ ó�� �ʿ�
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

// ������ ���� ���Ź���(�ٸ� ������ �޽���) �޽����� �迭�� �����ϰ� ��ü �޽����� ������ִ� �Լ�
// �̶� �Է����̴� �޽����� �ִٸ� restore�� ����
// �޽����� ������ �����ʹ� ������ �̷������ �ϹǷ� ��Ƽ������ ó�� �ʿ�
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
	setlocale(LC_ALL, ""); // ����� �� �ѱ� ó���� ���� ��������� ��.

	sock = new_socket("127.0.0.1", 9999); // 127.0.0.1 ������ 9999�� ��Ʈ�� ����(���� ������ �¾ƾ� ��)
	myconsole__input_name(sock, name); // ���� �̸��� �����ϴ� �Լ� ȣ��

	// ������ ����� ��ó�� �޽��� ������� �ޱ�� ���� ���ÿ� �̷������ �ϹǷ� �� �Լ��� ��Ƽ������ ó����.
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, send_msg, sock);
	pthread_create(&t2, NULL, recv_msg, sock);

	// t1, t2 �����尡 ���� �����⸦ ��ٸ�
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	// ���Ḧ ���� �ڿ� �ݳ�
	closesocket(sock);
	printf("���� �ݱ� �Ϸ�!!\n");

	WSACleanup();
	printf("winsock �ڿ� �ݳ�\n");

	// ���Ḧ ���� ���� ���� ����. ������ �����Ϸ��� break�� ó���ؾ���.
	while (1);
}