#pragma once
#include <iostream>
#include <vector>
#include <mysql.h>
using namespace std;

typedef struct Users
{
	string name;
	string id;
	string phone;
	string address;
}Users;

class User
{
	User();
	User(const string &_name, const string & _id, const string & _phone, const string & _address);
	~User();

private:
	string name;
	string id;
	string phone;
	string address;

	MYSQL* con;
	const char* host = "127.0.0.1";
	const char* user = "root";
	const char* pw = "Cyy200577";
	const char* database_name = "port_manager";
	const int port = 3306;

public:
	static User* GetInstance()
	{
		static User User;
		return &User;
	}
	void query_user();
	void add_user();

};

