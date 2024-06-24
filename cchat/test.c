#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

long long user_list[100] = {-1};
int user_size = 0;

typedef struct _ChatMessage {
	int userid;
	char name[20];
	char msg[100];
} ChatMessage;

//void send_msg(SOCKET csock) {
//	while (1) {
//		char sbuff[100] = { 0 };
//
//		// strcpy ���ڿ� ���� �Լ�
//		send(csock, sbuff, 100, 0);
//	}
//}
void recv_msg(SOCKET csock) {
	while (1) {
		char rbuff[1024] = { 0 };
		recv(csock, rbuff, 1024, 0);
		//ChatMessage chat_message;
		//memcpy(&chat_message, rbuff, sizeof(ChatMessage));

		for (int i = 0; i < user_size; i++) {
			SOCKET target = user_list[i];
			send(target, rbuff, 1024, 0);
		}
	}
}

void my_accept(SOCKET sock) {
	SOCKADDR_IN caddr;
	SOCKET csock;
	int csize = sizeof(caddr);

	csock = accept(sock, (SOCKADDR_IN*)&caddr, &csize);
	if (csock < 0) {
		exit(1);
	}

	if (csock == SOCKET_ERROR) {
		printf("���� ������ ���� �߻�!!\n");
		exit(1);
	}

	printf("���� ����!!\n");
	printf("����� ���� ��ȣ : %d \n", csock);

	char rbuff[1024] = { 0 };
	user_list[user_size++] = csock;
	int size = sizeof(csock);

	memcpy(rbuff, &csock, sizeof(csock));
	send(csock, rbuff, 1024, 0);
	recv(csock, rbuff, 1024, 0);
	// 8. ������ �ۼ��� - send() / recv()
	// send(sock, buff, len, flags)
	// recv(sock, buff, len, flags)
	// sock : send - �����͸� ���� ��� ����, recv - �����͸� ���� ��� ����
	// buff : send - ���� �����͸� ���� ����, recv - ���� �����͸� ���� ����
	// len : ������ ũ��
	// flags : �Ϲ������� 0

	// Ŭ���̾�Ʈ�� ������ ������
	pthread_t t2;
	//pthread_create(&t1, NULL, init, csock);
	pthread_create(&t2, NULL, recv_msg, csock);

	//pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	closesocket(sock);
	printf("���� �ݱ� �Ϸ�!!\n");

	WSACleanup();
	printf("winsock �ڿ� �ݳ�\n");

}
int main() {
	//const MSG_CAPACITY = 1000;

	WSADATA wsadata;
	//char** msgList = (char**)malloc(sizeof(char*) * MSG_CAPACITY); // �� 1000���� �޽��� ����

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

	// 4. �ּҿ� ��Ʈ��ȣ ����
	// sin_family : �ּ�ü��
	// sin_port : ��Ʈ��ȣ
	// sin_addr : IP �ּ� (ADDR_ANY ������ �ּҸ� �ڵ� ����)
	// htons : ��Ʋ�ε�� > ���ε�� 

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999); // 9000���� ���ķδ� �� �������. 9000 ~ 63000
	addr.sin_addr.S_un.S_addr = ADDR_ANY;

	// 5. ���ε�
	// bind(sock, addr, addrlen)
	// sock : ���ε��� ��� ����
	// addr : ��� ���Ͽ� ������ �ּ� ����ü(sockaddr) �ּ�
	// addrlen : �ּ� ����ü�� ũ��
	// sockaddr* : sockaddr_in ip v4 Ưȭ�� ����ü�ε�, �̰� ���� ���뼺�ְ� �ٲ۰��� sockaddr
	if (bind(sock, (SOCKADDR_IN*)&addr, sizeof(addr)) != 0) {
		printf("���ε� �� ����!!\n");
		return -1;
	}

	//6. ������(listen) - listen()
	// ���� ��û�� �������� ������ ����ҿ� ���ʴ�� ����.
	// listen(sock, backlog)
	// sock - ���� ��� ���� ����
	// backlog - ����ҿ� ������ ���� ���� �ִ� ����. ���� ���� ����� ���� ���� ���� ����.(ť)

	printf("���ε� ����!!\n");
	listen(sock, 5);
	printf("���� ���\n");

	// 7. ���� ���� - accept()
	// accept(sock, caddr, caddrlen) : ����ִ� caddr �ѱ�� accept() �Լ��� ���������� ������� �� caddr ����ü�� ���� ��û�� Ŭ���̾�Ʈ�� �ּ� ������ ���� ����. �����͸� ���� Ŭ���̾�Ʈ �ּҸ� �� �� ����.
	// sock - ���ε��� ���� ����
	// caddr - Ŭ���̾�Ʈ �ּ� ������ ���� ����ü
	// caddrlen - caddr�� ũ��
	// return : ���� ��û�� Ŭ���̾�Ʈ�� ���� ����

	/*SOCKADDR_IN caddr;
	SOCKET csock;
	int csize = sizeof(caddr);

	csock = accept(sock, (SOCKADDR_IN*)&caddr, &csize);
	if (csock < 0) {
		exit(1);
	}

	if (csock == SOCKET_ERROR) {
		printf("���� ������ ���� �߻�!!\n");
		exit(1);
	}

	printf("���� ����!!\n");
	printf("����� ���� ��ȣ : %d \n", csock);

	char rbuff[1024] = { 0 };
	user_list[user_size++] = csock;
	int size = sizeof(csock);

	memcpy(rbuff, &csock, sizeof(csock));
	send(csock, rbuff, 1024, 0);
	recv(csock, rbuff, 1024, 0); */
	// 8. ������ �ۼ��� - send() / recv()
	// send(sock, buff, len, flags)
	// recv(sock, buff, len, flags)
	// sock : send - �����͸� ���� ��� ����, recv - �����͸� ���� ��� ����
	// buff : send - ���� �����͸� ���� ����, recv - ���� �����͸� ���� ����
	// len : ������ ũ��
	// flags : �Ϲ������� 0

	// Ŭ���̾�Ʈ�� ������ ������
	for (int i = 0; i < 5; i++) {
		pthread_t t1;
		pthread_create(&t1, NULL, my_accept, sock);
		pthread_detach(t1);
	}

	while (1);
	//pthread_create(&t2, NULL, recv_msg, csock);

	//pthread_join(t1, NULL);
	//pthread_join(t2, NULL);


	//closesocket(sock);
	//printf("���� �ݱ� �Ϸ�!!\n");

	//WSACleanup();
	//printf("winsock �ڿ� �ݳ�\n");

	

	return 0;
}