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
    char serverIP[16];  // ���� IP �ּҸ� ������ ���ڿ� �迭 ����

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Winsock �ʱ�ȭ ����");
        exit(EXIT_FAILURE);
    }

    // ���� IP �ּ� �Է�
    printf("���� IP �ּҸ� �Է��ϼ���: ");
    fgets(serverIP, sizeof(serverIP), stdin);
    serverIP[strcspn(serverIP, "\n")] = '\0';  // fgets�� �Է¹��� ���� ���� ����

    // ���� ����
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("���� ���� ����");
        exit(EXIT_FAILURE);
    }

    // ���� ���� ����
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, serverIP, &(serverAddr.sin_addr)) <= 0) { // �Է¹��� ���� IP �ּ� ��ȯ
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
    while (1) {
        printf("�޽����� �Է��ϼ���: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("����: %s\n", buffer);

        if (strncmp(buffer, "����", 2) == 0)
            break;
    }

    // ���� ����
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
