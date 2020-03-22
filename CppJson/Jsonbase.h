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
public:
	enum json_value_type { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
	using json_value = std::variant<bool, int, double, std::string, std::nullptr_t, Array, Object>;

public:
	friend std::ostream& operator<<(std::ostream& os, json_value&& val);
};

std::ostream& operator<<(std::ostream& os, Jsonbase::json_value&& val)
{
	switch (val.index())
	{
	case Jsonbase::json_value_type::BOOLEAN:
		os << std::boolalpha << std::get<bool>(val);
		break;

	case Jsonbase::json_value_type::INT:
		os << std::get<int>(val);
		break;

	case Jsonbase::json_value_type::DOUBLE:
		os << std::get<double>(val);
		break;

	case Jsonbase::json_value_type::STRING:
		os << '\"' << std::get<std::string>(val) << '\"';
		break;

	case Jsonbase::json_value_type::EMPTY:
		os << std::get<std::nullptr_t>(val);
		break;

	case Jsonbase::json_value_type::ARRAY:
		os << std::get<Array>(val);
		break;

	case Jsonbase::json_value_type::OBJECT:
		os << std::get<Object>(val);
		break;
	}
	return os;
}
