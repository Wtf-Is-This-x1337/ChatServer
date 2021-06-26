#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include "string"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	// Change console title
	SetConsoleTitle(TEXT("Server Setup"));

	// Setup for the server
	int port;
	cout << "Chat Server:\n-> What port do you want to use?\n";
	cin >> port;
	Sleep(500);

	// Display info
	printf("-> Updating data ");
	Sleep(500);
	printf("- done\n");
	printf("-> Initializing winsock ");

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 99;
	}

	// Display info
	Sleep(500);
	printf("- Done\n");
	printf("-> Creating a socket ");

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return 99;
	}

	// Display info
	Sleep(500);
	printf("- Done\n");
	printf("-> Binding the ip address and the port to a socket ");

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Create the master file descriptor set and zero it
	fd_set master;
	FD_ZERO(&master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(listening, &master);

	// this will be changed by the \quit command (see below, bonus not in video!)
	bool running = true;

	// Display info
	Sleep(500);
	printf("- Done\n");
	Sleep(600);
	printf("\n---- Starting Up ----\n");
	Sleep(2000);
	SetConsoleTitle(TEXT("Server"));
	system("cls");
	cout << "Server logs:\n";

	while (running)
	{
		stringstream se;
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == listening)
			{
				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				// Display a message on the server
				printf("\n-> A new client connected to the server!");

				// Send a welcome message to the connected client
				string welcomeMsg = "SERVER:Welcome to the Awesome Chat Server!";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					bool isUsername = false;
					se << sock;
					string strsock = se.str();

					if (buf[0] == '*') {
						string username;
						string msg = string(buf, bytesIn);
						// Recieve username
						if (msg.find("**/%%/***") != std::string::npos) {
							username = msg.erase(0,9);
							isUsername = true;
							cout << "\nSocket#" + strsock + " sent his username (" + username + ')';
							break;
						}
					}

					// Check to see if it's a command. \quit kills the server
					if (buf[0] == '\\')
					{
						string cmd = string(buf, bytesIn);

						// Is the command quit? 
						if (cmd == "\\quit")
						{
							cout << "\nSocket#" + strsock + " killed the server.\n";
							running = false;
							break;
						}

						// Unknown command
						cout << "\nSocket#" + strsock + " sent an unknown command.";
						continue;
					}
					if (isUsername == false) {
						cout << "\nSocket#" + strsock + " sent a message on the server.";

						// Send message to other clients, and definiately NOT the listening socket
						for (u_int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock == listening)
							{
								continue;
							}

							ostringstream ss;

							if (outSock != sock)
							{
								ss << "Client #" << sock << ":" << buf << "\r\n";
							}
							else
							{
								ss << "Me:" << buf << "\r\n";
							}

							string strOut = ss.str();
							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(listening, &master);
	closesocket(listening);

	// Message to let users know what's happening.
	string msg = "SERVER:Server is shutting down. Goodbye\r\n";

	while (master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();

	system("pause");
	return 0;
}