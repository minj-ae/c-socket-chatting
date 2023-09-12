#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char serverIP[16];  // 서버 IP 주소를 저장할 문자열 배열 선언

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Winsock 초기화 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 IP 주소 입력
    printf("서버 IP 주소를 입력하세요: ");
    fgets(serverIP, sizeof(serverIP), stdin);
    serverIP[strcspn(serverIP, "\n")] = '\0';  // fgets로 입력받은 개행 문자 제거

    // 소켓 생성
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 정보 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, serverIP, &(serverAddr.sin_addr)) <= 0) { // 입력받은 서버 IP 주소 변환
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
    while (1) {
        printf("메시지를 입력하세요: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("서버: %s\n", buffer);

        if (strncmp(buffer, "종료", 2) == 0)
            break;
    }

    // 연결 종료
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
