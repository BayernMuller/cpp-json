#pragma once
#include <variant>
#include <string>
#include <vector>
#include <ios>
#include "Object.h"
#include "Array.h"

class Value
{
public: // definitions
	enum json_value_type { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
	using json_value = std::variant<bool, int, double, std::string, std::nullptr_t, Array, Object>;

public: // public funtions
	template<class T>
	Value(T&& value) : m_Value(value) {};

	template<class T>
	Value(const T& value) : m_Value(value) {};

	template<class T>
	T& GetValue();

	json_value_type GetType();

public: // operators



public: // friend functions
	friend std::ostream& operator<<(std::ostream& os, Value&& val);

private:
	json_value m_Value;
};

template<class T>
inline T& Value::GetValue()
{
	return std::get<T>(m_Value);
}

Value::json_value_type Value::GetType()
{
	return static_cast<Value::json_value_type>(m_Value.index());
}

std::ostream& operator<<(std::ostream& os, Value&& val)
{
	switch (val.GetType())
	{
	case Value::json_value_type::BOOLEAN:
		os << std::boolalpha << val.GetValue<bool>();
		break;

	case Value::json_value_type::INT:
		os << val.GetValue<int>();
		break;

	case Value::json_value_type::DOUBLE:
		os << val.GetValue<double>();
		break;

	case Value::json_value_type::STRING:
		os << '\"' << val.GetValue<std::string>() << '\"';
		break;

	case Value::json_value_type::EMPTY:
		os << val.GetValue<std::nullptr_t>();
		break;
		
	case Value::json_value_type::ARRAY:
		os << std::move(val.GetValue<Array>());
		break;

	case Value::json_value_type::OBJECT:
		//os << std::move(val.GetValue<Object>());
		break;
	}
	return os;
}

