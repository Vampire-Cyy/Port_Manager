#include "User.h"
#include <sql.h>

User::User()
{
	name = id = phone = address = "";
	con = mysql_init(NULL);
	//设置字符编码
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		exit(1);
	}
}

User::User(const string & _name, const string & _id, const string & _phone, const string & _address)
{
	name = _name;
	id = _id;
	phone = _phone;
	address = _address;
	con = mysql_init(NULL);
	//设置字符编码
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		exit(1);
	}
}

User::~User()
{	
	mysql_close(con);
}

void User::query_user()
{
	string condition, keyword;
	cin >> condition >> keyword;

	char sql[1024];

	sprintf(sql, "SELECT * FROM userdata WHERE %s REGEXP '%s' ", condition.c_str(), keyword.c_str());

	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;

	vector<Users> UserList;
	while ((row = mysql_fetch_row(res)))
	{
		Users user;
		user.name = row[1];
		user.id = row[2];
		user.phone = row[3];
		user.address = row[4];
		UserList.push_back(user);
	}
	for (auto& t : UserList)
	{
		cout << t.name << " " << t.id << " " << t.phone << " " << t.address << endl;
	}
}

void User::add_user()
{
	char sql[5196];
	User user;
	cout << "Name:";
	cin >> user.name; 

	cout << "Id:"; 
	cin >> user.id;
	//检验身份证号是否符合
	if (user.id.length() != 18)
	{
		cout << "Please enter legal id number" << endl;
		return;
	}

	cout << "Phone:";
	cin >> user.phone;
	//检验手机号是否符合
	if (user.phone.length() != 11) 
	{
		cout << "Please enter legal phone number" << endl;
		return;
	}

	cout << "Address:";
	cin >> user.address;

	//创建该项user数据
	sprintf(sql, "insert into userdata (name, id, phone, address) values('%s', '%s', '%s', '%s')",
		user.name.c_str(), user.id.c_str(), user.phone.c_str(), user.address.c_str());

	//查询是否插入成功
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}
}
