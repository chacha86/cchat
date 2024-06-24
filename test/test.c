#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#pragma comment(lib, "ws2_32")

int main() {

	WSADATA wsadata;

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

	while (1) {
		char rbuff[1024];
		recv(sock, rbuff, 1024, 0);
		printf("%s\n", rbuff);
	}
	printf("===>");
	return 0;
}