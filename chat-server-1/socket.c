#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#include <conio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32")

SOCKET new_socket(char* ip, int port) {
	WSADATA wsadata;
	//char** msgList = (char**)malloc(sizeof(char*) * MSG_CAPACITY); // 총 1000개의 메시지 저장

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

	// 4. 주소와 포트번호 설정
	// sin_family : 주소체계
	// sin_port : 포트번호
	// sin_addr : IP 주소 (ADDR_ANY 로컬의 주소를 자동 세팅)
	// htons : 리틀인디안 > 빅인디안 

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999); // 9000번대 이후로는 다 비어있음. 9000 ~ 63000
	addr.sin_addr.S_un.S_addr = ADDR_ANY;

	// 5. 바인딩
	// bind(sock, addr, addrlen)
	// sock : 바인딩할 대상 소켓
	// addr : 대상 소켓에 설정한 주소 구조체(sockaddr) 주소
	// addrlen : 주소 구조체의 크기
	// sockaddr* : sockaddr_in ip v4 특화된 구조체인데, 이걸 좀더 범용성있게 바꾼것이 sockaddr
	if (bind(sock, (SOCKADDR_IN*)&addr, sizeof(addr)) != 0) {
		printf("바인딩 중 에러!!\n");
		return -1;
	}

	//6. 연결대기(listen) - listen()
	// 연결 요청한 접속자의 정보를 저장소에 차례대로 저장.
	// listen(sock, backlog)
	// sock - 연결 대기 서버 소켓
	// backlog - 저장소에 저장할 연결 정보 최대 갯수. 제일 먼저 저장된 것을 제일 먼저 뺀다.(큐)

	printf("바인딩 성공!!\n");
	listen(sock, 5);
	printf("연결 대기\n");

	return sock;
}

SOCKET my_accept(SOCKET sock) {

	SOCKADDR_IN caddr;
	SOCKET csock;
	int csize = sizeof(caddr);

	csock = accept(sock, (SOCKADDR_IN*)&caddr, &csize);

	if (csock == SOCKET_ERROR) {
		printf("연결 수립중 에러 발생!!\n");
		return -1;
	}

	printf("연결 성공!!\n");
	printf("연결된 소켓 번호 : %d \n", csock);

	return csock;
	

}

