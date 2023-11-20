#include "Container.h"

Container::Container()
{
	port_name = action = lading_id = "";
	con = mysql_init(NULL);
	//ÉèÖÃ×Ö·û±àÂë
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		exit(1);
	}
}

Container::~Container()
{
	mysql_close(con);

}

void Container::query_amount()
{
	string _port_name, _date, date;
	cin >> _port_name >> _date >> date;

	char sql[1024];
	sprintf(sql, "SELECT * FROM container, logistics WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND container.lading_id = logistics.lading_id", 
		_port_name.c_str(), _date.c_str(), date.c_str() );
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}
	
	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;

	map<string, bool> action;
	action.clear();
	string type = "Èë¿â", _action, order;

	while ((row = mysql_fetch_row(res)))
	{
		_action = row[6];
		order = row[4];
		if (_action == type) action[order] = true;
		else action[order] = false;
	}

	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	long long in, out, sum;
	in = out = sum = 0;

	res = mysql_store_result(con);

	while ((row = mysql_fetch_row(res)))
	{
		if (action[row[1]]) in += atoi(row[7]);
		else out += atoi(row[7]);
		sum += atoi(row[7]);
	}

	cout << "ÍÌ£º" << in << endl << "ÍÂ£º" << out << endl << "×ÜÁ¿: " << sum << endl;

}

void Container::query_tendency()
{
	string _port_name, _date, date, cargo_name;
	cin >> _port_name >> _date >> date >> cargo_name;

	char sql[1024];
	sprintf(sql, "SELECT * FROM container, logistics WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND logistics.cargo_name = '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str(), cargo_name.c_str() );

	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;

	map<string, int> month;
	month.clear();
	string order, time;

	while ((row = mysql_fetch_row(res)))
	{
		order = row[4];
		time = row[7];
		time = time.substr(5, 2);
		month[order] = atoi(time.c_str());
	}

	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND logistics.cargo_name = '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str(), cargo_name.c_str());

	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	res = mysql_store_result(con);

	long long sum[15];
	for (int i = 0; i <= 12; i++) sum[i] = 0;

	while ((row = mysql_fetch_row(res)))
	{
		sum[month[row[1]]] += atoi(row[7]);
	}
	string st, ed;
	int s, e;
	st = _date.substr(5, 2);
	ed = date.substr(5, 2);
	s = atoi(st.c_str());
	e = atoi(ed.c_str());
	for (int i = s; i <= e; i++)
	{
		if (i == s) cout << sum[i] << endl;
		else cout << sum[i] << "(" << sum[i] - sum[i - 1] << ")" << endl;
	}
}
