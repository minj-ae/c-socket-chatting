#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")  // ws2_32.lib ���̺귯���� ��ũ (���� ����)

#define PORT 8080  // ��Ʈ ��ȣ 8080�� ���� (��ũ�� ����)
#define BUFFER_SIZE 1024  // 1024 ����Ʈ ũ���� ���۸� ����

int main() {
    WSADATA wsaData;  // Winsock ������ ����ü�� �����Ͽ� ��Ʈ��ũ �ʱ�ȭ�� ���
    SOCKET serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;  // ������ Ŭ���̾�Ʈ�� �ּ� ������ ��� sockaddr_in ����ü�� ����
    int addrSize = sizeof(clientAddr);  // Ŭ���̾�Ʈ �ּ� ������ ũ�⸦ �����ϴ� ������ �ʱ�ȭ
    char buffer[BUFFER_SIZE];  // ä���� �ӽ÷� ������ ���۸� ����

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Winsock �ʱ�ȭ ����");
        exit(EXIT_FAILURE);
    }

    // ���� ����
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("���� ���� ����");
        exit(EXIT_FAILURE);
    }

    // ���� ����
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // ���� ���ε�
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("���ε� ����");
        exit(EXIT_FAILURE);
    }

    // Ŭ���̾�Ʈ�κ����� ���� ��û ���
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        perror("��� ����");
        exit(EXIT_FAILURE);
    }

    printf("ä�� ������ ���� ���Դϴ�...\n");

    // Ŭ���̾�Ʈ�κ��� ���� ����
    if ((newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize)) == INVALID_SOCKET) {
        perror("���� ���� ����");
        exit(EXIT_FAILURE);
    }

    printf("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.\n");

    // Ŭ���̾�Ʈ���� ä��
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(newSocket, buffer, BUFFER_SIZE, 0); // Ŭ���̾�Ʈ�κ��� �����͸� �����Ͽ� ���ۿ� ����
        printf("Ŭ���̾�Ʈ: %s\n", buffer); // ������ �����͸� ���

        printf("�޽����� �Է��ϼ���: ");
        fgets(buffer, BUFFER_SIZE, stdin); // ����ڷκ��� �޽����� �Է¹޾� ���ۿ� ����
        send(newSocket, buffer, strlen(buffer), 0); // �Է��� �޽����� Ŭ���̾�Ʈ���� ����

        if (strncmp(buffer, "����", 2) == 0)
            break;
    }

    // ���� ����
    closesocket(newSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
