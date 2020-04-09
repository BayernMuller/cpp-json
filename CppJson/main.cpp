#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;
int main()
{
	JsonCreator my_json
	{
		{"int", 10},
		{"double", 99e-1},
		{"empty", nullptr},
		{"string", "text"},
		{"boolean", false},
		{"array", Array{1, true, "bayern"}},
		{"object", Object{ {"changmo", "god"} }}
	};
	Json json = Utility::ToJson(std::move(my_json));
	cout << json["int"] << endl;
	cout << json["double"] << endl;
	cout << json["empty"] << endl;
	cout << json["string"] << endl;
	cout << json["boolean"] << endl;
	cout << json["array"] << endl;
	cout << json["object"] << endl;
	cout << endl;

	cout << json["array"][2] << endl;
	cout << json["object"]["changmo"] << endl;
	return 0;
}