#include <iostream>
#include <mysql.h>
#include "User.h"
#include "Container.h"
using namespace std;

int main()
{
	//User::GetInstance()->add_user();
	//User::GetInstance()->query_user();
	//Container::GetInstance()->query_amount();
	Container::GetInstance()->query_tendency();
	return 0;
}