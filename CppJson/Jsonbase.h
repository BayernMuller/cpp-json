#pragma once
#include <variant>
#include <string>
#include <vector>
#include <any>
#include <ios>
class Array;
class Object;
class Json;

class JsonBase
{
	using value = std::variant<bool, long long, double, std::string, Array, Object>;

	
};
