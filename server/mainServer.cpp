/*
	Author:			Werner van der Heever
	Application:	This is a client-server application for Stock management system
	File:			mainServer.cpp
*/
#include "server.h"
#include<winsock2.h>
#include<ws2tcpip.h>
#include<string>
#include<iostream>

# pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define DEFAULT_PORT "7777"

int main (){
	
	int listenSoc = INVALID_SOCKET;
	int clientSoc = INVALID_SOCKET;
	int iResult;

	//Step 1: Initialize Winsock
	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (iResult != 0) {
		cout <<"WSAStartup failed: ", iResult;
		return 1;
	}

	//Step 2: Create a socket
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET; // Use IP version 4 adress familly
	hints.ai_socktype = SOCK_STREAM; // Use a stream sock
	hints.ai_protocol = IPPROTO_TCP; // Use the TCP protocol
	hints.ai_flags = AI_PASSIVE; // Flag used to tell it to use host's IP
	int res = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (res != 0){
			cout <<"Error: "<< gai_strerror(res) << endl;
			WSACleanup();
			return 1;
		}

	listenSoc = socket(result->ai_family,
	result->ai_socktype,
	result->ai_protocol);

	if (listenSoc == INVALID_SOCKET){
		cout <<"Error in creating the socket handle: "<< WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//Step 3: Bind the socket
	iResult = bind(listenSoc, result->ai_addr, result->ai_addrlen);
	if (iResult == -1){
		cout <<"Error in binding the socket handle: "<< WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);
	iResult = listen(listenSoc, 10); //Step 4: Listen on socket for client
	if (iResult == -1){
		cout <<"Error listening on socket"<< WSAGetLastError() << endl;
		closesocket(listenSoc);
		WSACleanup();
		return 1;
	}
	cout <<"Listening..."<< endl;

	// accept an incoming connection:
	struct sockaddr_storage client_addr; //struct to hold client's details
	socklen_t client_addr_size = sizeof client_addr;
	
	while (1){ //continue accepting connections until the server is closed
		
		//Step 5: Accept an incomming connection
		//clientSoc = accept(listenSoc,&((struct sockaddr_in *)&client_addr)->sin_addr,&client_addr_size);
		clientSoc = accept(listenSoc,(struct sockaddr *)&client_addr,&client_addr_size);
		if (clientSoc == INVALID_SOCKET){
			cout <<"Error creating new socket descriptor "
			<< WSAGetLastError() << endl;
			closesocket(listenSoc);
			WSACleanup();
			return 1;
		}
		char address[INET_ADDRSTRLEN];
		inet_ntop(client_addr.ss_family, // get client IP address
		&((struct sockaddr_in *)&client_addr)->sin_addr,
		address, INET_ADDRSTRLEN);
		
		//Step 6: Send and recieve data
		do { //continue recieving words from client, until client closes
			char buff[512];
			iResult = recv(clientSoc, buff, 512, 0);
			if (iResult > 0){
				buff[iResult] = '\0';
				cout <<"Message recieved from client"<< endl;
				cout <<"============================"<< endl;
				cout <<"Client address: "<< address << endl;
				cout <<"Client Message: "<< buff << endl;
				cout <<"============================"<< endl;
				
				//****************ENTER OWN CODE HERE******************
				server s;						//create server object
				string msg="";					//create a string to store data
				msg = s.sqlCon(buff);			//get data from database
				
				cout<<"DATA TO BE SENT: "<<msg<<endl;
				cout<<"----------------------------"<<endl;
				
				int Length = strlen(msg.c_str());
				if(Length > 512)
					Length = 512;				//make sure the lenth of the string is 512
				//*****************************************************
			
				//echo word back to client
				iResult = send(clientSoc, msg.c_str(), Length , 0);
				cout<<"message is sent!"<<endl;
				
				if (iResult == -1){
					cout <<"Could not reply to client. Error: "<< WSAGetLastError() << endl;
					closesocket(clientSoc);
					break;
				}
			} 
			else if (iResult == 0){
				cout <<"Client Closed"<< endl;
			} 
			else {
				cout <<"Recieve Error: "<< WSAGetLastError()<< endl;
				closesocket(clientSoc);
				break;
			}
		} while (iResult > 0);
	}
	system("CLS");

	closesocket(listenSoc);//Step 7: Disconnect
	WSACleanup();
	return 0;
}