#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <conio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32")

// 클라이언트 소켓을 생성하기 위한 함수. 접속할 서버의 IP와 PORT 번호를 입력받는다.
SOCKET new_socket(char* ip, int port) {
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
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

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

	return sock;
}

