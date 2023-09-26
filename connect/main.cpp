#include <iostream>
#include <WS2tcpip.h>
#include <string>
using namespace std;

#pragma comment (lib,"ws2_32.lib")

void main()
{
	// init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "can't Initialize winsock! Quitting" << endl;
		return;
	}

	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	// bind the ip address and port to the socket
	sockaddr_in hint; //sockaddr_in is a data type in the C++ socket library used to represent the IP address and port number of an endpoint
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;  //could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//tell me winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// waiting for connect
	sockaddr_in client; 
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXHOST]; //service (i.e.port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connect on port " << service << endl;
	}
	else
	{	//get the IP address of the client (from the sin_addr field in sockaddr_in), and save it in the server variable. Then the code enters the client's IP address (stored in the server variable) and the port number the client connects to (from the sin_port field in sockaddr_in)
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << ntohs(client.sin_port) << endl;
	}

	//close listening socket
	closesocket(listening);

	//while loop: accept and echo message back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		// wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "client disconnected" << endl;
			break;
		}

		// Echo massage back to client
		send(clientSocket, buf, bytesReceived + 1, 0);

		// display on sever
		cout << "client: " << buf << endl;
	}

	//close socket
	closesocket(clientSocket);

	//cleanup winsock
	WSACleanup();
}