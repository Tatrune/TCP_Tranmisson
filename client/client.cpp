#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>
#include <WS2tcpip.h>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef struct setting
{
	char nameFile[50] = "";
	string path;
	string ipAddress; // dia chi may chi
	int size; // kich thuoc cua file
	int	port; // Listening port # on the server
}set;

void main()
{
	set st1;
	cout << "Nhap dia chi IP may chu: " << endl;
	getline(cin, st1.ipAddress);
	cout << "Nhap port: " << endl;
	cin >> st1.port;
	getchar();

	// Khởi tạo WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Tạo socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(st1.port);
	inet_pton(AF_INET, st1.ipAddress.c_str(), &hint.sin_addr);

	// ket noi toi server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// gửi tên file
	cout << "Nhap duong dan den file thu muc can tao ben may chu: " << endl;
	getline(cin, st1.path);

	// lấy tên của file trong đường dẫn
	size_t lastSlashPosition = st1.path.rfind('\\');
	if (lastSlashPosition != string::npos)
	{

		string directory = st1.path.substr(lastSlashPosition + 1);

		cout << "name: " << directory << std::endl;
		strcpy(st1.nameFile, directory.c_str());
	}
	else {
		cout << "name: " << st1.path << endl;
	}
	// gui ten file
	int err = send(sock, (char*)st1.nameFile, 50, 0);
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);

	ifstream file(st1.path, ios::in | ios::binary);
	file.seekg(0, ios::end);
	st1.size = file.tellg();
	file.seekg(0, ios::beg );
	char* buffer = new char[st1.size];
	file.read(buffer,st1.size);
	//buffer[size] = 0;
	file.close();
	
	// gửi kích thước file (binary)
	int* fsize = &st1.size;
	err = send(sock, (char*)fsize, sizeof(fsize), 0); //sizeof(fize) fail
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);
	cout << "size of data: " << st1.size << endl;

	// gửi data (binary) 
	err = send(sock, buffer, st1.size , 0);
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);
	cout << "data: " << buffer << endl;

	delete[] buffer;

	//close socket
	closesocket(sock);
	//cleanup winsock
	WSACleanup();
}
