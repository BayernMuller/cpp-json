#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;


int main()
{
	JsonMaker bayern
	{
		{"trophy", Object{
							{"domestic", Object{
								{"bundesliga", 29},
								{"pokal", 19}}
							},

							{"europe", Object{
								{"supercup", 7}, 
								{"championsleague", 5}}
							}
						 }
		},

		{"best_players", Array{"lewandowski","muller","alaba"}},
		{"is_best", true},
		{"winning_rate", 99.9}
	};
	cout << bayern << endl;
	cout << endl;
	cout << bayern["best_players"] << endl;
	return 0;
}