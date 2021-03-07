/*
	Author:			Werner van der Heever
	Application:	This is a client-server application for Stock management system
	File:			server.cpp
*/
# include <iostream>
# include <iomanip>
# include "server.h"

//SQL libraries
#include"mysql_connection.h"	//Include to use the Connection object
#include<cppconn/driver.h>		//Include to use the Driver
#include<cppconn/exception.h>	//Include to use the SQLException object
#include<cppconn/resultset.h>
#include<cppconn/statement.h>
#include<cppconn/resultset_metadata.h>

using namespace sql;
using namespace std;

string server::sqlCon(string message){

	Driver* theDriver;										//Driver object
	Connection* theConnection;								//Connection object
	Statement* theStatement;
	ResultSet* theResultSet;
	ResultSetMetaData* metaData;

	try{
		cout <<"Getting driver..."<< endl;
		theDriver = get_driver_instance();					//get the driver
		cout <<"Getting driver..."<< endl;
		//connect to server
		theConnection = theDriver->connect("tcp://127.0.0.1:3306", "root", "");
		theConnection->setSchema("sms_db");					//set database to use
		theStatement = theConnection->createStatement();	//get statement
	}
	catch(sql::SQLException &e){
		cout <<"ERROR: "<< e.what();
	}

	try {
		bool result = theStatement->execute(message);
		//if statement to check if the SQL statement returned a resultSet or not
		if(result){
			//get the result set
			theResultSet = theStatement->getResultSet();
			metaData = theResultSet->getMetaData(); //get meta data
			cout <<"Table Name: "<< metaData->getTableName(1)<< endl; //get the table name
			
			int numColumns = metaData->getColumnCount();

			string CN;
			//for loop to print column lables
			for (int j = 1; j <= numColumns; j++){
				CN = CN.append("-");
				CN = CN.append(metaData->getColumnLabel(j));
				cout << left << setw(12)<< metaData->getColumnLabel(j);
			}
			cout<<endl;
			

			string data;
			int row;
			while (theResultSet->next()){
				row++;
				//for loop  to print the data
				for (int j = 0; j < numColumns; j++){
					data = data.append("-");
					data = data.append(theResultSet->getString(j+1));
					cout << left << setw(12)<< theResultSet->getString(j+1);
				}
				cout << endl;
			}
			string x = "";
			x = message.append(CN);
			x = message.append(data);
			
			int dSize = row * numColumns;
			return x;
			
		}
		else 
		{
			cout <<"Number of rows affected: "<< theStatement->getUpdateCount() << endl;
		}
	}
	catch (sql::SQLException &e) 
	{
			cout <<"ERROR: "<< e.what();				//display what went wrong
	}
	system("CLS");
	cout<<"Closing sql connections..."<<endl;
	//free up the memory
	delete theResultSet;
	delete theStatement;
	delete theConnection;	//always delete connection object when done
	
	cout<<"Connections are closed..."<<endl;
	cout<<"\n"<<endl;
}