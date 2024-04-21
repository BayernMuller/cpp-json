<p align="center">
    <img src='docs/logo.svg' width='100%' height='100%'/>
</p>

---

<span align="center">

Header-only JSON library for C++17

</span>

<p align="center">
    <img src="https://github.com/bayernmuller/cpp-json/actions/workflows/build.yaml/badge.svg"/>
    <img src="https://img.shields.io/github/license/bayernmuller/cpp-json"/>
</p>

### Introduction
* cpp-json is a code library for handling JSON in C++ applications, requiring C++17 or higher.
* To use, simply include the `cppjson.h` header file in your project.
* The library offers features for creating, managing, saving, and loading JSON data, as well as converting between JSON and strings.

### Installation

##### Requirements
* CMake
* Compiler with C++17 support
	* GCC 7.3 or later
	* Clang 5.0 or later
	* MSVC 19.14 or later

##### Build
* Run the following commands to build cpp-json.
    * `mkdir build && cd build`
    * `cmake ..`
    * `make`
	* `sudo make install`

##### Run example code
* After building the library, build directory will contain an example executable.
	* `./example_main`

### Usage

##### Creating JSON
```cpp
#include <cppjson.h>
using namespace json;

Json json
{
	{"best_club", "FC Bayern"},
	{"best_players", Array{"Manuel Neuer", "Thomas Müller"}},
	{"best_coach", Object{{"name", "Hansi Flick"}, {"age", 56}}}
};
```

##### Accessing JSON
```cpp
auto club = json["best_club"];
auto player = json["best_players"][0];
auto coach = json["best_coach"]["name"];
```

##### Modifying JSON
```cpp
json["best_coach"]["name"] = "Jupp Heynckes";
json["best_coach"]["age"] = 75;
json["best_players"].GetValue<Array>().push_back("Joshua Kimmich");
```

##### Converting string to JSON
```cpp
std::string json_str = R"(
{
    "best_club": "FC Bayern"
}
)";
Json json = Utility::Parse(json_str);
std::cout << json["best_club"] << std::endl;
```


### License
* cpp-json is licensed under the MIT License.

### Contributing
* Contributions are welcome! Please feel free to submit a Pull Request.