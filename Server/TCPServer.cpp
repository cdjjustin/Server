#include <stdio.h>
#include <Winsock2.h>   
#pragma comment(lib, "ws2_32.lib")
#include <iostream>

using namespace std;

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "error" << WSAGetLastError() << endl;
		return 0;
	}


	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		cout << "socket error!" << endl;
		return 0;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind error!" << endl;
	}

	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		cout << "listen error!" << endl;
		return 0;
	}

	SOCKET sClient;
	sockaddr_in clientAddr;
	int al = sizeof(clientAddr);
	char revData[BUFSIZ];
	int i = 0;
	while (true)
	{
		cout << "waiting for connecting..." << endl;
		sClient = accept(slisten, (sockaddr*)&clientAddr, &al);
		if (sClient == INVALID_SOCKET)
		{
			cout << "accept error!" << endl;
			continue;
		}
		cout << "accept a connecting: " << inet_ntoa(clientAddr.sin_addr) << endl;

		int ret = 0;
		//long long count = 0;
		char sendData[BUFSIZ] = "hello,TCP cilent!";
		//ret = recv(sClient, revData, BUFSIZ, 0);
		char over[BUFSIZ] = "Finnal";

		char filename[20] = "d://%d.doc";
		sprintf(filename, "d://%d.doc", i++);
		FILE* fp;
		fp = fopen(filename, "wb");
		if (NULL == fp)
		{
			cout << "cannot open file and write it!" << endl;
		}
		else
		{
			cout << "create file success!" << endl;
		}
		send(sClient, sendData, BUFSIZ, 0);
		cout << "data receving... ";
		while ((ret = recv(sClient, revData, BUFSIZ, 0)) > 0)
		{
			//cout << "#";
			if (strcmp(revData, over) == 0)
			{
				cout << " file transfer successs!" << endl;
				break;
				//send(sClient, over, BUFSIZ, 0);
			}
			fwrite(revData, 1, ret, fp);
			//send(sClient, sendData, BUFSIZ, 0);
			send(sClient, over, BUFSIZ, 0);
		}
		fclose(fp);
		if (strcmp(revData, over) != 0)
		{
			cout << "disconnect with " << inet_ntoa(clientAddr.sin_addr) << endl;
			remove(filename);
		}

		closesocket(sClient);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;

}
