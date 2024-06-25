#define HAVE_STRUCT_TIMESPEC
#include <locale.h>
#include <WinSock2.h>
#include <wchar.h>
#include <pthread.h>
#define BUFFER_SIZE 1024

typedef struct _ChatMessage {
	int userid;
	wchar_t name[20];
	wchar_t msg[200];
} ChatMessage;

// Ŭ���̾�Ʈ ���� ����
SOCKET sock;
wchar_t name[20];

// ȭ�� �ٽ� �׸� �� �ʿ��� �޽��� ������ �����
ChatMessage* msg_list[100];
int msg_size = 0;

void set_msg_data(int userid, wchar_t* name, wchar_t* msg, wchar_t* sbuff) {
	ChatMessage chat_message;
	chat_message.userid = userid;
	memcpy(chat_message.name, name, sizeof(chat_message.name));
	memcpy(chat_message.msg, msg, sizeof(chat_message.msg));
	memcpy(sbuff, &chat_message, sizeof(chat_message));
}

ChatMessage* add_msg(wchar_t* buff) {
	ChatMessage* chat_message = malloc(sizeof(ChatMessage));
	memcpy(chat_message, buff, sizeof(ChatMessage));
	msg_list[msg_size++] = chat_message;
	return chat_message;
}

void send_msg() {
	message_input_view();
	while (1) {
		wchar_t sbuff[BUFFER_SIZE] = { 0 };
		wchar_t msg[200];
		fgetws(msg, 200, stdin);
		set_msg_data(sock, name, msg, sbuff);
		send(sock, sbuff, BUFFER_SIZE, 0);
	}
}


void recv_msg(SOCKET csock) {

	while (1) {
		wchar_t rbuff[1024] = { 0 };
		recv(csock, rbuff, 1024, 0);
		add_msg(rbuff);
		system("cls");
		for (int i = 1; i < msg_size; i++) {
			wprintf(L"%ls(%d) : %ls", msg_list[i]->name, msg_list[i]->userid, msg_list[i]->msg);
		}
		message_input_view();
	}
}


int main() {
	setlocale(LC_ALL, "");

	sock = new_socket("127.0.0.1", 9999);

	input_name_view(sock, name);

	//wprintf(L"%ls", name);

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



	while (1);
}