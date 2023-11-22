#pragma once
#include <iostream>
#include <mysql.h>
using namespace std;

class Identity
{
	Identity();
	~Identity();

public:

	static Identity* GetInstance()
	{
		static Identity Identity;
		return &Identity;
	}

	void insert_user();
	bool login_user();
	void delete_user();

private:
	string name;
	string email;
	string password;

	MYSQL* con;

	const char* host = "127.0.0.1";
	const char* user = "root";
	const char* pw = "Cyy200577";
	const char* database_name = "port_manager";
	const int port = 3306;

};

