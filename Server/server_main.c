#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")  // ws2_32.lib 라이브러리를 링크 (오류 제어)

#define PORT 8080  // 포트 번호 8080을 정의 (매크로 설정)
#define BUFFER_SIZE 1024  // 1024 바이트 크기의 버퍼를 정의

int main() {
    WSADATA wsaData;  // Winsock 데이터 구조체를 선언하여 네트워크 초기화에 사용
    SOCKET serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;  // 서버와 클라이언트의 주소 정보를 담는 sockaddr_in 구조체를 선언
    int addrSize = sizeof(clientAddr);  // 클라이언트 주소 정보의 크기를 저장하는 변수를 초기화
    char buffer[BUFFER_SIZE];  // 채팅을 임시로 저장할 버퍼를 선언

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Winsock 초기화 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 생성
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("바인딩 실패");
        exit(EXIT_FAILURE);
    }

    // 클라이언트로부터의 연결 요청 대기
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        perror("대기 실패");
        exit(EXIT_FAILURE);
    }

    printf("채팅 서버가 실행 중입니다...\n");

    // 클라이언트로부터 연결 수락
    if ((newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize)) == INVALID_SOCKET) {
        perror("연결 수락 실패");
        exit(EXIT_FAILURE);
    }

    printf("클라이언트가 연결되었습니다.\n");

    // 클라이언트와의 채팅
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(newSocket, buffer, BUFFER_SIZE, 0); // 클라이언트로부터 데이터를 수신하여 버퍼에 저장
        printf("클라이언트: %s\n", buffer); // 수신한 데이터를 출력

        printf("메시지를 입력하세요: ");
        fgets(buffer, BUFFER_SIZE, stdin); // 사용자로부터 메시지를 입력받아 버퍼에 저장
        send(newSocket, buffer, strlen(buffer), 0); // 입력한 메시지를 클라이언트에게 전송

        if (strncmp(buffer, "종료", 2) == 0)
            break;
    }

    // 연결 종료
    closesocket(newSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
