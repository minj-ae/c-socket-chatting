#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;  // Winsock 데이터 구조체를 선언하여 네트워크 초기화에 사용
    SOCKET clientSocket;  // 클라이언트 소켓을 선언
    struct sockaddr_in serverAddr;  // 서버 주소 정보를 담는 sockaddr_in 구조체를 선언
    char buffer[BUFFER_SIZE];  // 채팅을 임시로 저장할 버퍼를 선언

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // Winsock 초기화
        perror("Winsock 초기화 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 생성
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {  // 클라이언트 소켓 생성
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 정보 설정
    serverAddr.sin_family = AF_INET;  // IPv4
    serverAddr.sin_port = htons(PORT);  // 포트 번호 설정
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) <= 0) { 
        perror("유효한 IP 주소 변환 실패");
        exit(EXIT_FAILURE);
    }

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("연결 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버에 연결되었습니다.\n");

    // 서버와의 채팅
    while (1) {  // 무한 루프를 돌면서 클라이언트-서버-클라이언트-서버 순으로 채팅
        printf("메시지를 입력하세요: ");
        fgets(buffer, BUFFER_SIZE, stdin);  // 사용자로부터 메시지를 입력받아 버퍼에 저장
        send(clientSocket, buffer, strlen(buffer), 0);  // 입력한 메시지 전송

        memset(buffer, 0, sizeof(buffer));  // 버퍼 초기화
        recv(clientSocket, buffer, BUFFER_SIZE, 0);  // 서버로부터 데이터를 수신하여 버퍼에 저장
        printf("서버: %s\n", buffer);  // 수신한 데이터를 출력

        if (strncmp(buffer, "종료", 2) == 0)  // 입력한 메시지가 "종료"일 때 루프를 종료하여 채팅을 종료
            break;
    }

    // 연결 종료
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}