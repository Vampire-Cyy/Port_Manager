#include <iostream>
#include <mysql.h>
#include "User.h"
#include "Container.h"
#include "Identity.h"
using namespace std;

int main()
{
	cout << "Welcome to the Port_Manager System(beta)!" << endl;
	cout << endl << endl << endl;

	Identity::GetInstance()->insert_user();
	
	while (Identity::GetInstance()->login_user());
	
	Container::GetInstance()->query_amount();
	Container::GetInstance()->query_tendency();
	Container::GetInstance()->query_portion();
	Container::GetInstance()->query_direction();
	Container::GetInstance()->add_record();
	User::GetInstance()->add_user();
	User::GetInstance()->query_user();

	return 0;
}