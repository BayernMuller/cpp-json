#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;

int main()
{
	ifstream file("Bayern.json");
	auto json = Utility::LoadJson(file);
	cout << json << endl;
	return 0;
}