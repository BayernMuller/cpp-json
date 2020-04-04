#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;

int main()
{
	JsonCreator china
	{
		{"idiots", Array{"XiJinping", "just_people"}},
		{"is_sucks", true},
		{"tibet", "FREE"}
	};

	Json china_json = Utility::ToJson(std::move(china));
	cout << china_json << endl;
	return 0;
}