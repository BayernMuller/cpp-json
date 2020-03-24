#pragma once
#include <initializer_list>
#include <variant>
#include <vector>
#include <cassert>
#include <string>
#include <ios>
#include <map>

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

	public: // public funtions
		template<class ...Args, std::size_t size = sizeof...(Args)> Value(Args&& ...args);
		template<class T> Value(T&& value) : m_Value(std::forward<T>(value)) {}
		template<class T> Value(const T& value) : m_Value(value) {}
		Value(const char* str) : m_Value(std::string(str)) {}
		Value() {}

		template<class T>
		T& GetValue();

		value_type GetType() const;
		auto HasValue() { return m_Value.valueless_by_exception(); }

	public: // operators
		/*
		template<class T>
		T& operator=(T&& value) ;

		template<class T>
		T& operator=(const T& value) ;
		*/
		Value& operator[](std::size_t index);
		Value& operator[](const char* key);

	public: // friend functions
		friend std::ostream& operator<<(std::ostream& os, const Value& val);

	private:
		

	private:
		json_value m_Value;
	};

	///////////////////////////////////////////////////////

	template<class ...Args, std::size_t size>
	inline Value::Value(Args&& ...args)
	{
		m_Value.emplace<Array>().reserve(size);
		(std::get<Array>(m_Value).push_back(std::forward<Args>(args)), ...);
	}

	template<class T>
	inline T& Value::GetValue()
	{
		return std::get<T>(m_Value);
	}


	/*
	template<class T>
	inline T& Value::operator=(T&& value) const
	{
		m_Value.emplace<T>(value);
	}

	template<class T>
	inline T& Value::operator=(const T& value) const
	{
		m_Value.emplace<T>(value);
	}
	*/
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
			os << "[ \n\r";
			for (auto itr = arr.begin(); itr != arr.end(); itr++)
			{
				os << *itr;
				if (itr != arr.end() - 1)
				{
					os << ", ";
				}
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