/*
	Author:			Werner van der Heever
	Application:	This is a client-server application for Stock management system
	File:			server.h
*/
# include <string>

using namespace std;

class server{
private:
	string columnName;
	string dataDB;

public: 
	string sqlCon(string);
};
