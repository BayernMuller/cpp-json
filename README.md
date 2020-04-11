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
int main()
{
	JsonCreator my_json
	{
		{"Is 2018 worldcup the worst", true}
	};
	// You can't print JsonCreator directly.
	// operator<< only accepts Json's lvalue-reference
	Json json = Utility::ToJson(my_json);
	cout << json << endl;
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
	Json json = Utility::ToJson(std::move(my_json));
	// Move Construct. If you will not use JsonCreator, you'd better use move.
	cout << json << endl;
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



**Accessing each values**

Code:

```c++
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
```

Result:

```json
10
9.9
null
"text"
false
[
    1,
    true,
    "bayern"
]
{
    "changmo": "god"
}

"bayern"
"god"
```


### Remember this.

I made for just fun. I didn't make this json library for you. You can use this but don't blame me even exist bugs.

