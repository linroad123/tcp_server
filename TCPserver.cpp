#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>

#define SERVER_PORT 5150

int main(int argc, char** argv)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int ret,length;
    SOCKET sListen, sServer;
    struct sockaddr_in saServer, saClient; //address information
    wVersionRequested=MAKEWORD(2,2);
    ret = WSAStartup(wVersionRequested, &wsaData);
    if (ret!=0)
    {
        printf("WSAStartup() failed!\n");
        //return 0;
    }

    // create Socket, use TCP protocol
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == INVALID_SOCKET)
    {
        WSACleanup();
        printf("socket() faild!\n");
    }

    saServer.sin_family = AF_INET;
    saServer.sin_port = htons(SERVER_PORT);
    saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    ret = bind(sListen,(struct sockaddr *)&saServer, sizeof(saServer));
    if (ret == SOCKET_ERROR)
    {
        printf("bind() failed! code:%d\n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
    }

    // listening connecting request
    ret = listen(sListen,5);
    if (ret == SOCKET_ERROR)
    {
        printf("bind() failed! code:%d\n",WSAGetLastError());
        closesocket(sListen);

    }

    printf("Waiting for client connecting!\n");
    printf("Tips: Ctrl+c to quit!\n");

    length = sizeof(saClient);
    sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
    if (sServer == INVALID_SOCKET)
    {
        printf("accept() failed! code:%d\n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
        return 0;
    }

    char receiveMessage[1024];

    ret = recv(sServer, receiveMessage, 1024,0);
    if (ret == SOCKET_ERROR)
    {
        printf("recv() failed!\n");
        return 0;
    }

    char sendMessage[] = "Hello ! This is Server";
    send(sServer,sendMessage, strlen(sendMessage)+1,0);

    printf("receive message:%s\n",receiveMessage);

    closesocket(sListen);
    closesocket(sServer);
    WSACleanup();
    system("pause");
    return 0;
}