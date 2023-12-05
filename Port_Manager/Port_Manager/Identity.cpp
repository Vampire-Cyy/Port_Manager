#include "Identity.h"

Identity::Identity()
{
	name = email = password = "";
	
	con = mysql_init(NULL);
	//ÉèÖÃ×Ö·û±àÂë
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		exit(1);
	}
}

Identity::~Identity()
{
	mysql_close(con);
}

void Identity::insert_user()
{
	cout << "Please sign up an account!" << endl;

	cout << "Name:";
	cin >> name;

	cout << "Email:";
	cin >> email;

	cout << "Password:";
	cin >> password;
	
	char sql[1024];
	sprintf(sql, "INSERT INTO users (name, email, password) VALUES('%s', '%s', CONCAT('*', UPPER(SHA1(UNHEX(SHA1('%s') ) ) ) ) )",
		name.c_str(), email.c_str(), password.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to log in an account : Error: %s\n.", mysql_error(con));
		return;
	}

	cout << "Success to sign up an account!" << endl << endl << endl;

}

bool Identity::login_user()
{
	cout << "Please login your account!" << endl;
	cin >> email >> password;
	cout << endl;

	char sql[1024];
	sprintf(sql, "SELECT * FROM users WHERE email = '%s' AND password = CONCAT('*', UPPER(SHA1(UNHEX(SHA1('%s')))))",
		email.c_str(), password.c_str());
	if (mysql_query(con, sql)) 
	{
		fprintf(stderr, "Failed to log in : Error : %s\n", mysql_error(con));
		return false;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(res)))
	{
		if (row[0] == NULL)
		{
			cout << "Please sign up an account!" << endl;
			return false;
		}
		else
		{
			cout << "Welcome to sign in " << row[0] << "!" << endl << endl;
			return true;
		}
	}
}

void Identity::delete_user()
{
	cin >> email >> password;

	char sql[1024];
	sprintf(sql, "DELETE FROM users WHERE email = '%s' AND password = CONCAT('*', UPPER(SHA1(UNHEX(SHA1('%s')))))",
		email.c_str(), password.c_str());

	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to delete data : Error : %s\n", mysql_error(con));
		return;
	}

	sprintf(sql, "SELECT * FROM users WHERE email = '%s'", email.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error : %s\n", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		cout << "Please enter right email or password!" << endl;
		return;
	}

	cout << "See you next time!" << endl;
}
