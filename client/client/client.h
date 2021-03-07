/*
	Author:			Werner van der Heever
	Application:	This is a client-server application for Stock management system
	File:			client.h
*/
# include <string>
# include <vector>
using namespace std;

class winsock{
private:
	
public:
	int winCon(string);
};

class menu{
private:
	
public:
	int mainM();
	int mMenu();
	int cMenu();
};

class manager{
private:
		string username;
		string password;
		vector<string> orders;

public:
		string viewProdStockLvl();
		string ordersNotShipped();
		string increaseStock();
		void printFile(vector<string>);
		void setOrders(vector<string> order){orders.swap(order);}
		vector <string> getOrders(){return orders;}
};

class client {
private:
		string id;
		vector<string> orders;
public:
		string viewProdStockLvl();
		string placeOrder();
		void printInvoice(vector<string>);
		//use a template function to determine the amount due
		void printOrder(vector <string>);
		void setOrders(vector<string> order){orders.swap(order);}
		vector <string> getOrders(){return orders;}
};
