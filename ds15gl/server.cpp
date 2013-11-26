//��������socket
#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include <iostream>
using std::cout;
using std::endl;

extern void dsGameInit(const std::string& rep_file_name);
extern bool gameOver;


int server() {
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	//����winsock��
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		return 0;
	}
	// �����׽���
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET) {
		printf("socket error\n");
		return 0;
	}
	// ��sockaddr_in�ṹ��װ���ַ��Ϣ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(6000); // htons���� ���������޷��Ŷ�������ת��������
	//�ֽ�˳��
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	// ʹ�׽��ֺͱ��ص�ַ��
	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf(" bind error \n");
		closesocket(sListen);
		return 0;
	}
	// �����׽��ֽ������ģʽ
	if (listen(sListen, 100) == SOCKET_ERROR) {
		printf("listen error\n");
		closesocket(sListen);
		return 0;
	}
	// ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr;
	SOCKET sClient;
	int nAddrLen = sizeof(remoteAddr);
	char revData[255];
	while (TRUE) {
		// ����һ��������
		sClient = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		//accept��������ʧ��������ȴ����ӡ�
		
		int timeout=3000;
		setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(int));
		setsockopt(sClient,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(int));
		if (sClient == INVALID_SOCKET) {
			printf("accept() error");
			continue;
		}
		//��ӡ�������ߵ�ip
		printf(" ���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		//ֱ���յ���Ч����ʱ�Ŵ�ӡ����
		//      while(1)
		//      {

		//          while(1)
		//          {
		char buff[256]="0";

		//��������
		send(sClient, buff, strlen(buff)+1, 0);
		// �ر��׽��־���������Ự
		// closesocket(sClient);
		//}
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0) {
			//Ϊ�˷�ֹ��ӡ�������ַ�����β���0x00
			revData[ret] = 0x00;
			cout << revData << endl;
		}
		
		dsGameInit(revData);
		while(!gameOver){
			send(sClient, "1", 2, 0);
			int ret = recv(sClient, revData, 2, 0);
		}

	}
	closesocket(sListen);
	WSACleanup();
	return 0;
}