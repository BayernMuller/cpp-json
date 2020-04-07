#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;

int main()
{
	auto json = Utility::Parse("{\"numbers\":[1,2,3]}");
	cout << json << endl;
	return 0;
}