#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <wchar.h>
#pragma comment(lib, "ws2_32")

typedef struct _ChatMessage {
	int userid;
	wchar_t name[20];
	wchar_t msg[100];
} ChatMessage;

ChatMessage get_msg_data(wchar_t* buff) {
	ChatMessage chat_message;
	memcpy(&chat_message, buff, sizeof(chat_message));
	return chat_message;
}

int main() {

	SOCKET server = new_socket(9999);
	SOCKET client = my_accept(server);

	while (1) {
		wchar_t buff[1024];
		recv(client, buff, 1024, 0);
		ChatMessage chat_message = get_msg_data(buff);
		wprintf(L"%ls : %ls", chat_message.name, chat_message.msg);
		send(client, buff, 1024, 0);
	}
}
