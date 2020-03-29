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
	class Json;

	/*  --------------------------------  */

	class Value
	{
		friend Json;
	public: // definitions
		enum class value_type { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
		using Array = std::vector<Value>;
		using Object = std::map<std::string, Value>;
		using Null = std::nullptr_t;
		using json_value = std::variant<bool, int, double, std::string, Null, Array, Object>;

	public: // constructors
		Value(bool value)		: m_Value(value), m_nDepth(0) {}
		Value(int value)		: m_Value(value), m_nDepth(0) {}
		Value(double value)		: m_Value(value), m_nDepth(0) {}
		Value(const char* str)	: m_Value(std::move(std::string(str))), m_nDepth(0) {}
		Value(Null value)		: m_Value(value), m_nDepth(0) {}
		Value(Array&& value)	: m_Value(std::forward<Array>(value)), m_nDepth(0) {}
		Value(Object&& value)	: m_Value(std::forward<Object>(value)), m_nDepth(0) {}
		Value(const Value& value) : m_Value(value.m_Value), m_nDepth(value.m_nDepth) {}
		Value(Value&& value) : m_Value(std::move(value.m_Value)), m_nDepth(value.m_nDepth) {}
		Value() : m_Value(nullptr), m_nDepth(0) {}


	public: // public funtions
		template<class T>
		T& GetValue();

		value_type GetType() const;
		bool HasValue();

	public: // operators
		Value& operator=(Value&& value);
		Value& operator=(const Value& value);
		Value& operator[](std::size_t index);
		Value& operator[](const char* key);

	public: // friend functions
		friend std::ostream& operator<<(std::ostream& os, Value& val);

	private:
		void SpaceDepth(std::ostream& os, int depth);
		void SetDepth(Object& obj, int depth);
		void SetDepth(Array& arr, int depth);

	private:

		json_value m_Value;
		int m_nDepth;
	};

	///////////////////////////////////////////////////////

	template<class T>
	inline T& Value::GetValue()
	{
		return std::get<T>(m_Value);
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

	inline bool Value::HasValue()
	{
		return !m_Value.valueless_by_exception();
	}

	inline Value& Value::operator[](std::size_t index)
	{
		return GetValue<Array>()[index];
	}
	
	inline Value& Value::operator[](const char* key)
	{
		return GetValue<Object>()[key];
	}

	inline void Value::SpaceDepth(std::ostream& os, int depth)
	{
		for (int i = 0; i < depth; i++)
			os << '\t';
	}

	inline void Value::SetDepth(Object& obj, int depth)
	{
		for (auto& [key, val] : obj)
		{
			switch (val.GetType())
			{
			case value_type::ARRAY:
				SetDepth(val.GetValue<Array>(), depth + 1);
				break;
			case value_type::OBJECT:
				SetDepth(val.GetValue<Object>(), depth + 1);
				break;
			default:
				val.m_nDepth = depth + 1;
				break;
			}
		}
	}

	inline void Value::SetDepth(Array& arr, int depth)
	{
		for (auto& val : arr)
		{
			switch (val.GetType())
			{
			case value_type::ARRAY:
				SetDepth(val.GetValue<Array>(), depth + 1);
				break;
			case value_type::OBJECT:
				SetDepth(val.GetValue<Object>(), depth + 1);
				break;
			default:
				val.m_nDepth = m_nDepth;
				break;
			}
		}
	}

	std::ostream& operator<<(std::ostream& os, Value::Object& obj)
	{
		operator<<(os, *(&Value(std::move(obj))));
		return os;
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
			os << "[\r\n";
			for (auto itr = arr.begin(); itr != arr.end(); itr++)
			{
				val.SpaceDepth(os, val.m_nDepth + 1);
				os << *itr;
				if (itr != arr.end() - 1)
					os << ", ";
				os << "\r\n";
			}
			val.SpaceDepth(os, val.m_nDepth);
			os << ']';
			break;
		}
		case Value::value_type::OBJECT:
		{
			int cnt = 0;
			auto& obj = val.GetValue<Value::Object>();
			val.SetDepth(obj, 0);
			os << "{\r\n";
			for (auto& [key, val] : obj)
			{
				val.SpaceDepth(os, val.m_nDepth + 1);
				os << '\"' << key << "\": " << val;
				if (cnt++ != obj.size() - 1)
					os << ',';
				os << "\r\n";
			}
			val.SpaceDepth(os, val.m_nDepth);
			os << '}';
			break;
		}
		}
		return os;
	}

}