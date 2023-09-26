#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
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

	// Do-while loop to send and receive data
	//char buf[4096];
	//string userInput;

	//do
	//{
	//	// Prompt the user for some text
	//	cout << ">";
	//	getline(cin, userInput);

	//	if (userInput.size() > 0)		// Make sure the user has typed in something
	//	{
	//		// Send the text
	//		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1 , 0);
	//		if (sendResult != SOCKET_ERROR)
	//		{
	//			// Wait for response

	//			ZeroMemory(buf, 4096);
	//			int bytesReceived = recv(sock, buf, 4096, 0); //Waiting...
	//			if (bytesReceived > 0)
	//			{
	//				// Echo response to console
	//				cout << "Sent: > " << string(buf, 0, bytesReceived) << endl;
	//			}
	//		}
	//	}
	//} while (userInput.size() > 0);

	// gửi tên file
	/*char nameFile[] = "hinh.rar";
	send(sock, (char*)nameFile, sizeof(nameFile), 0);*/

	char nameFile[] = "hinh.rar";
	send(sock, (char*)nameFile, strlen(nameFile), 0);

	ifstream file("C:\\Users\\ACER NITRO 5\\Desktop\\Tcp_protocol\\client\\hinh.rar", ios::binary);
	file.seekg(0, ios::end);
	int size = file.tellg();
	file.seekg(0, ios::beg);
	char* buffer = new char[size];
	file.read(buffer, size);
	file.close();

	// gửi kích thước file (binary)
	int* fsize = &size;
	int err = send(sock, (char*)fsize, sizeof(fsize), 0);
	cout << "size: " << size << endl;
	if (err <= 0)
	{
		printf("send: %d\n", WSAGetLastError());
	}
	printf("send %d bytes [OK]\n", err);

	// gửi data (binary) 
	err = send(sock, buffer, size, 0);
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
