#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;  // Winsock ������ ����ü�� �����Ͽ� ��Ʈ��ũ �ʱ�ȭ�� ���
    SOCKET clientSocket;  // Ŭ���̾�Ʈ ������ ����
    struct sockaddr_in serverAddr;  // ���� �ּ� ������ ��� sockaddr_in ����ü�� ����
    char buffer[BUFFER_SIZE];  // ä���� �ӽ÷� ������ ���۸� ����

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // Winsock �ʱ�ȭ
        perror("Winsock �ʱ�ȭ ����");
        exit(EXIT_FAILURE);
    }

    // ���� ����
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {  // Ŭ���̾�Ʈ ���� ����
        perror("���� ���� ����");
        exit(EXIT_FAILURE);
    }

    // ���� ���� ����
    serverAddr.sin_family = AF_INET;  // IPv4
    serverAddr.sin_port = htons(PORT);  // ��Ʈ ��ȣ ����
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) <= 0) { 
        perror("��ȿ�� IP �ּ� ��ȯ ����");
        exit(EXIT_FAILURE);
    }

    // ������ ����
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("���� ����");
        exit(EXIT_FAILURE);
    }

    printf("������ ����Ǿ����ϴ�.\n");

    // �������� ä��
    while (1) {  // ���� ������ ���鼭 Ŭ���̾�Ʈ-����-Ŭ���̾�Ʈ-���� ������ ä��
        printf("�޽����� �Է��ϼ���: ");
        fgets(buffer, BUFFER_SIZE, stdin);  // ����ڷκ��� �޽����� �Է¹޾� ���ۿ� ����
        send(clientSocket, buffer, strlen(buffer), 0);  // �Է��� �޽��� ����

        memset(buffer, 0, sizeof(buffer));  // ���� �ʱ�ȭ
        recv(clientSocket, buffer, BUFFER_SIZE, 0);  // �����κ��� �����͸� �����Ͽ� ���ۿ� ����
        printf("����: %s\n", buffer);  // ������ �����͸� ���

        if (strncmp(buffer, "����", 2) == 0)  // �Է��� �޽����� "����"�� �� ������ �����Ͽ� ä���� ����
            break;
    }

    // ���� ����
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}