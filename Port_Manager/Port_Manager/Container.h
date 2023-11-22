#pragma once
#include <iostream>
#include <mysql.h>
#include <map>
using namespace std;

class Container
{
	Container();
	~Container();
public:

	static Container* GetInstance()
	{
		static Container Container;
		return &Container;
	}
	void query_amount();
	void query_tendency();
	void query_portion();
	void query_direction();
	void add_record();

private:

	string port_name;
	string lading_id;
	string action;

	MYSQL* con;

	const char* host = "127.0.0.1";
	const char* user = "root";
	const char* pw = "Cyy200577";
	const char* database_name = "port_manager";
	const int port = 3306;

	void query_portion_1();
	void query_portion_2();
};

