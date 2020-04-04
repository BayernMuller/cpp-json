#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;

int main()
{
	ifstream ifile("Bayern.json");
	auto bayern = Utility::LoadJson(ifile);
	cout << bayern["treble"] << endl;
	cout << "[ 1, 2, 3, 4 ]"_Json << endl;
	
	auto new_json = "{ \"is_bayern_best_team\": true }"_Json;
	ofstream ofile("new_json.json");
	Utility::WriteJson(ofile, new_json);
	ofile.close();
	return 0;
}