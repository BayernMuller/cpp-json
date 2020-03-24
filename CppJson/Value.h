#pragma once
#include <initializer_list>
#include <variant>
#include <vector>
#include <cassert>
#include <string>
#include <ios>
#include <map>
#include <iostream>

namespace json
{
	class Value
	{
	public: // definitions
		enum class value_type { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
		using Array = std::vector<Value>;
		using Object = std::map<std::string, Value>;
		using Null = std::nullptr_t;
		using json_value = std::variant<bool, int, double, std::string, Null, Array, Object>;

	public: // constructors
		
		template<class T> Value(T value);
		//template<class T> Value(const T& value);
		Value(const Value& value) : m_Value(value.m_Value) {}
		Value(Value&& value) : m_Value(std::move(value.m_Value)) {}
		Value(const char* str) : m_Value(std::string(str)) {}
		Value(std::initializer_list<Value> list);
		Value(std::initializer_list<std::pair<std::string, Value>> list);
		Value() {}


	public: // public funtions
		template<class T>
		T& GetValue();

		value_type GetType() const;
		bool HasValue() { return !m_Value.valueless_by_exception(); }

	public: // operators
		Value& operator=(Value&& value);
		Value& operator=(const Value& value);
		Value& operator[](std::size_t index);
		Value& operator[](const char* key);

	public: // friend functions
		friend std::ostream& operator<<(std::ostream& os, Value& val);

	private:
		

	private:
		json_value m_Value;
	};

	///////////////////////////////////////////////////////

	template<class T>
	inline Value::Value(T value) : m_Value(value)
	{
		
	}

	/*template<class T>
	inline Value::Value(const T& value) : m_Value(value)
	{

	}*/

	template<class T>
	inline T& Value::GetValue()
	{
		return std::get<T>(m_Value);
	}


	inline Value::Value(std::initializer_list<Value> list)
	{
		m_Value.emplace<Array>().assign(list.begin(), list.end());
	}

	inline Value::Value(std::initializer_list<std::pair<std::string, Value>> list)
	{

	}

	inline Value& Value::operator=(Value&& value)
	{
		m_Value = std::move(value.m_Value);
		return *this;
	}

	inline Value& Value::operator=(const Value& value)
	{
		m_Value = value.m_Value;
		return *this;
	}

	inline Value::value_type Value::GetType() const
	{
		return static_cast<Value::value_type>(m_Value.index());
	}

	inline Value& Value::operator[](std::size_t index)
	{
		return GetValue<Array>()[index];
	}

	inline Value& Value::operator[](const char* key)
	{
		return GetValue<Object>()[key];
	}

	std::ostream& operator<<(std::ostream& os, Value& val)
	{
		switch (val.GetType())
		{
		case Value::value_type::BOOLEAN:
		{
			os << std::boolalpha << val.GetValue<bool>();
			break;
		}
		case Value::value_type::INT:
		{
			os << val.GetValue<int>();
			break;
		}
		case Value::value_type::DOUBLE:
		{
			os << val.GetValue<double>();
			break;
		}
		case Value::value_type::STRING:
		{
			os << '\"' << val.GetValue<std::string>() << '\"';
			break;
		}
		case Value::value_type::EMPTY:
		{
			os << "null";
			break;
		}
		case Value::value_type::ARRAY:
		{
			auto& arr = val.GetValue<Value::Array>();
			os << "[ ";
			for (auto itr = arr.begin(); itr != arr.end(); itr++)
			{
				os << *itr;
				if (itr != arr.end() - 1)
					os << ", ";
			}
			os << " ]\n\r";
			break;
		}
		case Value::value_type::OBJECT:
		{
			break;
		}
		}
		return os;
	}

}