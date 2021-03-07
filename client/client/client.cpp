/*
	Author:			Werner van der Heever
	Application:	This is a client-server application for Stock management system
	File:			client.cpp
*/
#include "client.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

# pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define DEFAULT_PORT "7777"

typedef basic_string<TCHAR> tstring;

winsock w;

int winsock::winCon(string msg){
	int iResult;
	// Step 1: Initialize Winsock
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		cout <<"WSAStartup failed: ", iResult;
		return 1;
	}
	// Step 2: Create a socket. To do this a couple of other steps have tobe done first
	// Step i: Declare addrinfo structures to hold address information
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET; // Use IP version 4 address familly
	hints.ai_socktype = SOCK_STREAM; // Use a stream sock
	hints.ai_protocol = IPPROTO_TCP; // Use the TCP protocol
	
	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	GetComputerName(computerName, &size);

	wstring arr_w(computerName);
	string the_server(arr_w.begin(), arr_w.end());

	//cout <<"Server name (or IP address) to which you wish to connect: ";
	//cin >> the_server;

	// Step ii: Call getaddrinfo() to fill addrinfo structure
	iResult = getaddrinfo(the_server.c_str(),DEFAULT_PORT,&hints,&result);

	if (iResult != 0){
		cout <<"Error getting address info of the server: "<< gai_strerror(iResult) << endl;
		WSACleanup();
		return 1;
	}
	int soc = INVALID_SOCKET;

	//for loop used to iterate through the linked list of addrinfo
	//structures (result)returned by getaddrinfo() function
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next){
		// Step iii: call socket() to create socket to server address
		soc = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (soc == INVALID_SOCKET){
			cout <<"Error in creating the socket handle: "<< WSAGetLastError() << endl;
			WSACleanup();
			return 1;
		}

		//Step 3: Connect to server
		iResult = connect(soc, ptr->ai_addr, ptr->ai_addrlen);
		if (iResult == -1){
		//if it could not connect with current soc, move onto the
		//next one in the linked list
			closesocket(soc);
			soc = INVALID_SOCKET;
			continue;
		}
	break;
	}
	freeaddrinfo(result); //free memory used by addrinfo structure.
	if (soc == INVALID_SOCKET){
		cout <<"Unable to connect to server "<< endl;
		WSACleanup();
	return 1;
	}
	
	cout <<"Connected to the server"<< endl;
	//system("PAUSE");
	//system("CLS");

	//Step 4: Send and Recieve Data
	//string message;
	while (msg.compare("EXIT")) { //send words until EXIT is sent.
		
		iResult = send(soc, msg.c_str(), msg.length(), 0);
		if (iResult == -1){
			cout <<"Message was not sent! Error: "<< WSAGetLastError() << endl;
			closesocket(soc);
			WSACleanup();
			return 1;
		}
		//system("CLS");
		cout <<"Message sent! Waiting for reply"<< endl;
		char buff[512]; //Buffer to hold reply from server
		iResult = recv(soc, buff, 512, 0);

		if (iResult > 0 && iResult <= 512){
			//display word sent back from server
			buff[iResult] = '\0';

			//cout <<"Server Reply: "<< buff <<endl;
			
			vector<string> words;
			istringstream f(buff);
			string word;
			while( getline(f, word, '-') )
			{
				//cout<<word<<" ";
				words.push_back(word);
				
			}
			cout<<"\n"<<endl;
			words.erase(words.begin() + 0);
			int count = 0;
			manager m;
			client c;
			m.setOrders(words);
			c.setOrders(words);
			for(int i = 0; i < words.size(); i++)
			{
				count++;
				cout<< left << setw(12)<< words[i];
				if(count == 5)
				{
						cout<<endl;
						count = 0;
				}
			}
		} 
		else if (iResult == -1){
		cout <<"Recieve failed: Error "<< WSAGetLastError();
		}
		break;
	}
	cout <<"Done!"<< endl;
	system("PAUSE");
	//Step 5: Disconnect
	closesocket(soc);
	WSACleanup();
	return 0;
}

int menu::mainM(){
	
	cout<<"*************************************************"<<endl;
	cout<<"*************STOCK MANAGEMENT SYSTEM*************"<<endl;
	cout<<"*************************************************"<<endl;
	//cout<<""<<endl;
	cout<<"\n"<<endl;

	int logOption;
	
	
		cout<<"Do you wish to log in as a: (Choose a option number)"<<endl;
		cout<<"1.) Manager"<<endl;
		cout<<"2.) Client"<<endl;
		cout<<"3.) Exit"<<endl;
		cin>>logOption;
		cout<<"\n"<<endl;
		system("CLS");

		if(logOption == 3)
		{
			cout<<"Thanks for using this application!"<<endl;
			w.winCon("EXIT");
			exit(1);
		}
			

	return logOption;
}

int menu::mMenu(){
	
	int option;
	
		cout<<"You logged in as a manager!"<<endl;
		cout<<"Choose a option from the manager Menu:"<<endl;
		cout<<"1.) View all products & current stock level"<<endl;
		cout<<"2.) View all orders that has been shipped"<<endl;
		cout<<"3.) Increase stock level"<<endl;
		cout<<"4.) Print results of the last month"<<endl;
		cout<<"5.) Back "<<endl;
		cout<<"Enter a option: ";
		cin>>option;
		cout<<endl;
		cout<<"\n"<<endl;
		system("CLS");
	
	return option;
}

int menu::cMenu(){

	int option;
	
	cout<<"You logged in as a Client!"<<endl;
	cout<<"Choose a option from the manager Menu:"<<endl;
	cout<<"1.) View all products & their current stock level"<<endl;
	cout<<"2.) Place an order"<<endl;
	cout<<"3.) Print invoice"<<endl;
	cout<<"4.) Print all orders placed by client"<<endl;
	cout<<"5.) Back "<<endl;
	cout<<"Enter a option";
	cin>>option;
	cout<<endl;
	cout<<"\n"<<endl;
	system("CLS");
	
	return option;
}

string manager::viewProdStockLvl(){
	string msg ="SELECT * FROM product;";
	return msg;
}

string manager::ordersNotShipped(){
	string msg = "SELECT * FROM orders WHERE DELIVERED = 'FALSE';";
	return msg;
}

string manager::increaseStock(){
	string units = "";
	string prodNo = "";

	cout<<endl;
	cout<<"Enter the number of units: ";
	cin>>units;
	cout<<endl;
	system("CLS");

	cout<<endl;
	cout<<"Enter the product number: ";
	cin>>prodNo;
	cout<<endl;
	system("CLS");

	string msg = "UPDATE product SET stock_level = stock_level + "+units+" WHERE product_id = "+prodNo+";";
	msg.insert(46,units);
	msg.insert(66,prodNo);
	system("CLS");
	return msg;
}

//PRINT LIST OF ORDERS - FILE HANDLING
void manager::printFile(vector<string> orderList){
	//number of orders placed on each product in the last month
	//print what percentage each order make from total orders
	cout<<"busy printing file..."<<endl;
	cout<<endl;
	
	ofstream myfile ("manText.txt");
	if(myfile.is_open())
	{
		int count = 0;
		for(int i = 0; i < orderList.size(); i++)
		{
			count++;
			myfile<<orderList[i]<<" ";
			if(count == 5)
				cout<<"\n";
		}
	}
	system("PAUSE");
	system("CLS");
}

string client::viewProdStockLvl(){
	string msg = "SELECT * FROM product;";
	return msg;
}

string client::placeOrder(){
	
	char yn = 'n';

	do{
	string units;
	string prodNo;

	cout<<endl;
	cout<<"Enter the number of units: ";
	cin>>units;
	cout<<endl;
	system("CLS");

	cout<<endl;
	cout<<"Enter the product number: ";
	cin>>prodNo;
	cout<<endl;
	system("CLS");

	string msg = "UPDATE product SET stock_level = stock_level - "+units+" WHERE product_id = "+prodNo+";";
	
	//msg.insert(46,units);
	//msg.insert(66,prodNo);
	return msg;

	cout<<"Do you want to make another order? (Enter 'Y' or 'N'): ";
	cin>>yn;
	cout<<endl;

	if(yn == 'n' || yn == 'N')
		break;

	}while(yn == 'y' || yn == 'Y');
	system("CLS");
}

//PRINT INVOICE - FILE HANDLING
void client::printInvoice(vector<string> orderList){
	cout<<"printing invoice..."<<endl;
	cout<<endl;
	
	ofstream myfile ("clientText.txt");
	if(myfile.is_open())
	{
		int count = 0;
		for(int i = 0; i < orderList.size(); i++)
		{
			count++;
			myfile<<orderList[i]<<" ";
			if(count == 5)
				cout<<"\n";
		}
	}
	system("PAUSE");
	system("CLS");
}

//CREATE a template function to calculate amount due

//PRINT ALL ORDERS PLACED BY CLIENT - FILE HANDLING
void client::printOrder(vector <string> orderList){
		cout<<"printing order..."<<endl;

		cout<<"printing invoice..."<<endl;
	cout<<endl;
	
	ofstream myfile; 
	myfile.open("order.txt");
	if(myfile.is_open())
	{
		int count = 0;
		for(int i = 0; i < orderList.size(); i++)
		{
			count++;
			myfile<<orderList[i]<<" ";
			if(count == 5)
				cout<<"\n";
		}
	}
	system("PAUSE");
	system("CLS");
}

