# Cpp-Json


### What is this?

Code that allows json to be used in C++.

----



### How to install?

> Your C++ version **must be C++ 17** at least.

Add `CppJson/Value.h` and `CppJson/Json.h` in your project.  And include Json.h header to use it.

----



### How to use?

**Making Json**

``` C++
#include "Json.h"

int main()
{
	json::JsonCreator bayern
	{
		{"trophy", json::Object{
							{"domestic", json::Object{
								{"bundesliga", 29},
								{"pokal", 19}}
							},

							{"europe", json::Object{
								{"supercup", 7},
								{"championsleague", 5}}
							}
						 }
		},

		{"best_players", json::Array{
								   json::Object{
											{"name", "Lewandowski"},
											{"back_number", 9},
											{"goal", 22},
											{"assist", 5}
								   },

								   json::Object{
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
	return 0;
}
```



**Printing Json**

Code:

```C++
#include <iostream>
#include "Json.h"
using namespace std;
using namespace json;
int main()
{
	JsonCreator my_json
	{
		{"Is 2018 worldcup the worst", true}
	};

	cout << my_json << endl;
    // Printing at std::ostream.
	return 0;
}
```

Result:

```json
{
    "Is 2018 worldcup the worst": true
}
```



Code:

```c++
int main()
{
	JsonCreator my_json
	{
		{"best_clup", "FC Bayern Munich"},
		{"best_leagues", Object{
							{"europe", "champions league"},
							{"germany", "bundesliga"},
							{"other", "trash"}
						 }
		},
		{"best_players", Array{"lewandowski", "muller", "alaba"}}
	};

	cout << my_json << endl;
	return 0;
}
```

Result:

```Json
{
    "best_clup": "FC Bayern Munich",
    "best_leagues": {
        "europe": "champions league",
        "germany": "bundesliga",
        "other": "trash"
    },
    "best_players": [
        "lewandowski",
        "muller",
        "alaba"
    ]
}
```



**Saving Json**

Code:

```C++
int main()
{
	JsonCreator my_json
	{
		{"KEY", 99e-1}
	};

	Json json = Utility::ToJson(my_json);
	ofstream file("new_json.json");
	Utility::WriteJson(file, json);
	file.close();
	return 0;
}
```



**Loading Json**

Code:

```C++
int main()
{
	ifstream file("new_json.json");
	Json json = Utility::LoadJson(file);
	file.close();
	return 0;
}
```



**String to Json**

Code:

```C++
int main()
{
	const char* str = "{ \"Numbers\": [ 1,2,3,4 ] }";
	Json json = Utility::Parse(str);
	return 0;
}
```



**Json to String**

Code:

```C++
int main()
{
	JsonCreator my_json
	{
		{"KEY", 123}
	};
	string str = Utility::Dumps(my_json);
	return 0;
}
```



### Remember this.

I made for just fun. I didn't make this json library for you. You can use this but don't blame me even exist bugs.

