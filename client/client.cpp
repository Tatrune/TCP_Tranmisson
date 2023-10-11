#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "192.168.1.118";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
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
	hint.sin_port = htons(54000);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// gửi tên file
	char nameFile[30] = "";
	string path;
	cout << "Nhap ten thu muc can tao ben may chu: " << endl; 
	cin >> nameFile;
	getchar();
	cout << "Nhap duong dan den file thu muc can tao ben may chu: " << endl;
	getline(cin, path);
	int err = send(sock, (char*)nameFile, 30, 0);
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);
	//ifstream file("C:\\Users\\ACER NITRO 5\\Desktop\\Tcp_protocol\\client\\hinh.rar", ios::in | ios::binary );
	ifstream file(path, ios::in | ios::binary);
	file.seekg(0, ios::end);
	int size = file.tellg();
	file.seekg(0, ios::beg );
	char* buffer = new char[size];
	file.read(buffer,size);
	buffer[size] = 0;
	file.close();
	
	// gửi kích thước file (binary)
	int* fsize = &size;
	err = send(sock, (char*)fsize, sizeof(fsize), 0); //sizeof(fize) fail
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);
	cout << "size of data: " << size << endl;

	// gửi data (binary) 
	err = send(sock, buffer, size , 0);
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);
	cout << "data: " << buffer << endl;

	delete[] buffer;

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
