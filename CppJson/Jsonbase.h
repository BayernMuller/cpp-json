#pragma once
#include <variant>
#include <string>
#include <vector>
#include <any>
#include <ios>
class Array;
class Object;
class Json;

class Jsonbase
{
protected:
	using value = std::variant<bool, int, double, std::string, std::nullptr_t, Array, Object>;

	template<int> struct Value;
	template<> struct Value<0> { using type = bool; };
	template<> struct Value<1> { using type = long long; };
	template<> struct Value<2> { using type = double; };
	template<> struct Value<3> { using type = std::string; };
	template<> struct Value<4> { using type = std::nullptr_t; };
	template<> struct Value<5> { using type = Array; };
	template<> struct Value<6> { using type = Object; };

protected:

};
