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
	
	while (!Identity::GetInstance()->login_user());
	
	Container::GetInstance()->query_amount();
	cout << endl << endl << endl;
	Container::GetInstance()->query_tendency();
	cout << endl << endl << endl;
	Container::GetInstance()->query_portion();
	cout << endl << endl << endl;
	Container::GetInstance()->query_portion();
	cout << endl << endl << endl;
	Container::GetInstance()->query_direction();
	cout << endl << endl << endl;
	Container::GetInstance()->add_record();
	cout << endl << endl << endl;
	User::GetInstance()->add_user();
	cout << endl << endl << endl;
	User::GetInstance()->query_user();
	cout << "Thanks for your using!" << endl;
	cout << "See you next time!" << endl;
	return 0;
}