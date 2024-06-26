#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <wchar.h>
#pragma comment(lib, "ws2_32")

// 데이터를 주고 받을 저장소의 크기를 지정하기 위함
#define BUFFER_SIZE 1024

// 채팅 메시지 데이터는 여러 정보로 이루어지므로 구조체를 활용
typedef struct _ChatMessage {
	int userid; // 유저 번호
	wchar_t name[20]; // 유저 이름
	wchar_t msg[200]; // 채팅 메시지
} ChatMessage;

static SOCKET* client_list; // 멀티 채팅 서버는 여러 유저를 관리해야 하므로 유저를 저장하기 위한 배열 주소
static int client_size; // 유저의 수

// 필요한 변수를 초기화 하기 위한 초기화 함수
void init() {
	int conn_size = socket__get_conn_size();
	client_list = (SOCKET*)malloc(sizeof(SOCKET) * conn_size);
	client_size = 0;
}

// 다 사용한 메모리를 반환하기 위한 함수. 프로그램 종료 전에 실행할 것임.
void destroy() {
	for (int i = 0; i < client_size; i++) {
		free(client_list[i]);
	}
}

// buff 저장소로 부터 chat_message로 메모리 내용을 복사.
ChatMessage get_msg_data(char* buff) {
	ChatMessage chat_message;
	memcpy(&chat_message, buff, sizeof(chat_message));
	return chat_message;
}

// 클라이언트를 인수로 받아 해당 클라이언트로 부터 데이터를 수신하는 함수.
// 수신한 데이터는 서버가 관리하는 모든 유저에게 다시 보낸다. 채팅 메시지를 모두에게 전달하기 위함
// 해당 함수는 여러 클라이언트에게 데이터를 수신할 수 있어야 하므로 멀티스레드로 실행되어야 함.
void recv_msg(SOCKET client) {
	while (1) {

		char buffer[BUFFER_SIZE] = { 0 };

		printf("수신 대기중..\n");
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

// 서버 소켓을 받아 해당 서버를 클라이언트와 연결하고 클라이언트와의 데이터 수신 부분을 멀티스레드로 분리하는 함수
void create_acc_threads(SOCKET server) {

	SOCKET client = socket__my_accept(server);
	client_list[client_size++] = client;

	// 위에서 언급한 것처럼 recv_msg는 멀티스레드로 동시 실행되어야 하는 함수이므로 멀티스레드 처리.
	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, recv_msg, client);
	pthread_join(recv_thread, NULL); // recv_thread가 완료되면 기다렸다가 종료함.

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

	SOCKET server = socket__new_socket(); // 새로운 채팅 서버 생성
	int conn_size = socket__get_conn_size(); // 해당 서버가 가용하는 최대 유저수 구하기

	// 유저 수만큼 서버가 연결을 수립할 수 있도록 하는 코드.
	// 동시에 여러 유저와 연결되어야 하므로 유저와 연결하는 함수인 create_acc_threads 함수를 멀티스레드로 처리.
	for (int i = 0; i < conn_size; i++) {
		pthread_t acc_thread;
		pthread_create(&acc_thread, NULL, create_acc_threads, server);
		pthread_detach(acc_thread); // 백그라운드에서 돌도록 처리
	}

	while (1); // 서버가 종료되지 않도록 처리. 추후 break 처리 해줘야 함.

	destroy(); // 서버 종료시 자원 반납
}
