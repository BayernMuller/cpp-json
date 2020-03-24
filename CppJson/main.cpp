#include <iostream>
#include "Json.h"

using namespace std;

int main()
{
	json::Value arr({ 1,2,"string" });
	//json::Value integer(45);
	json::Value obj
	(
		{ "key", {1,2,3} }
	);
	cout << arr << endl;
	
	return 0;
}