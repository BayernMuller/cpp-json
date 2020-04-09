#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;
int main()
{
	JsonCreator my_json
	{
		{"KEY", 123}
	};
	cout << my_json << endl;
	return 0;
}