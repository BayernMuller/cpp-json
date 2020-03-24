#include <iostream>
#include "Json.h"

using namespace std;

int main()
{
	json::Value arr(1,2,"string");
	json::Value integer(45);
	cout << integer << endl;
	return 0;
}