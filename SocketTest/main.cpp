#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#include <direct.h>

int  main(int argc, char** argv) {
    char* path = getcwd(NULL, 0);


    if (argc < 2) {
        fprintf(stderr, "args is not enough\n");
        return 1;
    }

	if(strcmp(argv[1]+strlen(argv[1])-3,"rep")){
		fprintf(stderr, "file is not rep\n");
		return 2;
	}

    char fileDir[300];
    strcpy(fileDir, path);
    strcat(fileDir, "\\");
    strcat(fileDir, argv[1]);

    //����ļ��Ƿ����
    FILE* f = fopen(fileDir, "r");
    if (!f) {
        fprintf(stderr, "file %s is not exist", argv[1]);
        return 2;
    }
    fclose(f);

    int err;
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired = MAKEWORD(1, 1);
    err = WSAStartup(versionRequired, &wsaData); //Э���İ汾��Ϣ
    if (!err) {
        printf("�ͻ���Ƕ�����Ѿ���!\n");
    } else {
        printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
        return 3;//����
    }
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN clientsock_in;
    clientsock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    clientsock_in.sin_family = AF_INET;
    clientsock_in.sin_port = htons(6000);
    //bind(clientSocket,(SOCKADDR*)&clientsock_in,strlen(SOCKADDR));//ע�����������
    //listen(clientSocket,5);

    if (connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == 0) { //��ʼ����
        //flag = 1;
        char receiveBuf[1000];
        //
        recv(clientSocket, receiveBuf, 1001, 0);
        send(clientSocket, fileDir, strlen(fileDir) + 1, 0);
		
		int timeout=3000;
		setsockopt(clientSocket,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(int));
		setsockopt(clientSocket,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(int));
		
		while(1){
			recv(clientSocket, receiveBuf, 2, 0);
			if(receiveBuf[0]=='0')
				break;
			send(clientSocket, "1", 2, 0);
		}

        printf("%s\n", receiveBuf);

    } else {
        fprintf(stderr, "�˿�δ��\n");
        return 4;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}