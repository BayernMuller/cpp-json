#include <iostream>
#include <fstream>
#include "Json.h"
using namespace std;
using namespace json;

int main()
{
	ifstream file("Bayern.json");
	istreambuf_iterator<char> begin(file), end;
	string str(begin, end);
	cout << str << endl;
	Json myJson = Utility::Parse(str);
	cout << myJson << endl;
	return 0;
}