//��������socket
#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>

void server() {
    //�����׽���
    WORD myVersionRequest;
    WSADATA wsaData;
    myVersionRequest = MAKEWORD(1, 1);
    int err;
    err = WSAStartup(myVersionRequest, &wsaData);
    if (!err) {
        printf("�Ѵ��׽���\n");
    } else {
        //��һ�����׽���
        printf("Ƕ����δ��!");
        return ;
    }
    SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0); //�����˿�ʶ���׽���
    //��Ҫ�󶨵Ĳ���
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ip��ַ
    addr.sin_port = htons(6000); //�󶨶˿�

    bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR)); //�����
    listen(serSocket, 1); //���еڶ������������ܹ����յ�����������

    //////////////////////////////////////////////////////////////////////////
    //��ʼ���м���
    //////////////////////////////////////////////////////////////////////////
    SOCKADDR_IN clientsocket;


    int len = sizeof(SOCKADDR);
    SOCKET serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len); //������ﲻ��accept����conection�Ļ������ͻ᲻�ϵļ���
	char sendBuf[100]="Welcome to ds15gl";
    //sprintf(sendBuf, "welcome %s to bejing", inet_ntoa(clientsocket.sin_addr)); //�ҶԶ�Ӧ��IP���ҽ������ִ�ӡ������
    send(serConn, sendBuf, strlen(sendBuf) + 1, 0);
    char receiveBuf[100];//����
    recv(serConn, receiveBuf, strlen(receiveBuf) + 1, 0);
    printf("%s\n", receiveBuf);
	extern void dsGameInit(const std::string& rep_file_name);
	dsGameInit(receiveBuf);
    closesocket(serConn);//�ر�
    WSACleanup();//�ͷ���Դ�Ĳ���
}