#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;


int main()
{
	JsonCreator bayern
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

		{"best_players", Array{
								   Object{
											{"name", "Lewandowski"},
											{"back_number", 9},
											{"goal", 22},
											{"assist", 5}
								   },
							
								   Object{
											{"name", "Muller"},
											{"back_number", 25},
											{"goal", 5},
											{"assist", 18}
								   }
							  }
		},
		{"is_best", true},
		{"winning_rate", 99.9}
	};

	
	auto str = Json::Dumps(bayern);
	cout << str << endl;
	return 0;
}