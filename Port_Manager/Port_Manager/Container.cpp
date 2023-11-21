#include "Container.h"

Container::Container()
{
	port_name = action = lading_id = "";
	con = mysql_init(NULL);
	//设置字符编码
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
	string type = "入库", _action, order;

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

	cout << "吞：" << in << endl << "吐：" << out << endl << "总量: " << sum << endl;

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
		else
		{
			cout << sum[i] << "(";
			if(sum[i] - sum[i-1] < 0) cout << sum[i] - sum[i - 1] << ")" << endl;
			else cout << "+" << sum[i] - sum[i - 1] << ")" << endl;
		}
	}
}

void Container::query_portion()
{
	int type;
	cin >> type;
	if (type == 1) query_portion_1();
	else query_portion_2();
}

void Container::query_direction()
{
	string cargo_name;
	cin >> cargo_name;

	char sql[1024];
	sprintf(sql, "SELECT * FROM unload LEFT JOIN logistics ON logistics.cargo_name='%s' AND unload.lading_id = logistics.lading_id UNION SELECT * FROM unload RIGHT JOIN logistics ON logistics.cargo_name = '%s' AND unload.lading_id = logistics.lading_id; ", cargo_name.c_str(), cargo_name.c_str() );
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error : %s\n.", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;

	map<string, map<string, long long> > route;
	route.clear();

	while ((row = mysql_fetch_row(res)))
	{
		if (row[19] == NULL ) continue;
		if (row[0] == NULL) break;
		//cout << row[11] << " " << row[12] << " " << row[19] << endl;
		route[row[11]][row[12]] += atoi(row[20]);
		//cout << route[row[11]][row[12]] << endl;
	}

	for (auto & temp_1 : route)
	{
		//cout << temp_1.first << " ";
		for (auto & temp_2 : temp_1.second)
		{
			cout << temp_1.first << "->" << temp_2.first << ": " << temp_2.second << endl;
		}
	}
}

void Container::add_record()
{
	string st, ed, cargo_name, cargo_weight, action_date;
	cin >> st >> ed >> cargo_name >> cargo_weight >> action_date;

	string lading_id, action_1, action_2;
	string strRand(int length);
	lading_id = strRand(12);
	action_1 = "入库";
	action_2 = "出库";
	//cout << lading_id;

	char sql[1024];
	//插入container(两条)
	sprintf(sql, "INSERT INTO container (port_name, lading_id, action, action_date) VALUES('%s', '%s', '%s', '%s')",
		st.c_str(), lading_id.c_str(), action_2.c_str(), action_date.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}

	sprintf(sql, "INSERT INTO container (port_name, lading_id, action, action_date) VALUES('%s', '%s', '%s', '%s')",
		ed.c_str(), lading_id.c_str(), action_1.c_str(), action_date.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}

	//插入logistics
	sprintf(sql, "INSERT INTO logistics (lading_id, cargo_name, cargo_weight) VALUES('%s', '%s', '%s')",
		lading_id.c_str(), cargo_name.c_str(), cargo_weight.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}

	//插入load表
	sprintf(sql, "INSERT INTO `load` (port_name, lading_id, depart_part, arrive_part) VALUES('%s', '%s', '%s', '%s')",
		st.c_str(), lading_id.c_str(), st.c_str(), ed.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}

	//插入unload表
	sprintf(sql, "INSERT INTO unload (port_name, lading_id, depart_port, arrive_port) VALUES('%s', '%s', '%s', '%s')",
		ed.c_str(), lading_id.c_str(), st.c_str(), ed.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to insert data : Error:%s\n", mysql_error(con));
		return;
	}
}

void Container::query_portion_1()
{
	string _port_name, _date, date, cargo_name;
	cin >> _port_name >> _date >> date >> cargo_name;

	char sql[1024];
	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;
	
	double sum = 0;

	while ((row = mysql_fetch_row(res)))
	{
		sum += atoi(row[7]);
	}

	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND logistics.cargo_name = '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str(), cargo_name.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	res = mysql_store_result(con);

	double sum_cargo = 0;

	while ((row = mysql_fetch_row(res)))
	{
		sum_cargo += atoi(row[7]);
	}
	cout << (sum_cargo / sum) << endl;
}

void Container::query_portion_2()
{
	string _port_name, _date, date, cargo_name;
	cin >> _port_name >> _date >> date >> cargo_name;

	string st, ed;
	st = _date.substr(0, 5) + (string)"01-01";
	ed = _date.substr(0, 5) + (string)"12-31";

	char sql[1024];
	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND logistics.cargo_name = '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), st.c_str(), ed.c_str(), cargo_name.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;

	double sum = 0;

	while ((row = mysql_fetch_row(res)))
	{
		sum += atoi(row[7]);
	}

	sprintf(sql, "SELECT * FROM logistics, container WHERE port_name = '%s' AND action_date >= '%s' AND action_date <= '%s' AND logistics.cargo_name = '%s' AND container.lading_id = logistics.lading_id",
		_port_name.c_str(), _date.c_str(), date.c_str(), cargo_name.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to select data : Error: %s\n", mysql_error(con));
		return;
	}

	res = mysql_store_result(con);

	double sum_cargo = 0;

	while ((row = mysql_fetch_row(res)))
	{
		sum_cargo += atoi(row[7]);
	}

	cout << (sum_cargo / sum) << endl;
}

