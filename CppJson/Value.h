#pragma once
#include <variant>
#include <vector>
#include <string>
#include <ios>
#include <map>

namespace json
{
	class Value
	{
	public: // definitions
		enum class types { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
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
		Value(Array&& arr)		: m_Value(std::move(arr)), m_nDepth(0) {}
		Value(Object&& obj)		: m_Value(std::move(obj)), m_nDepth(0) {}
		Value(const Array& arr) : m_Value(arr), m_nDepth(0) {}
		Value(const Object& obj): m_Value(obj), m_nDepth(0) {}
		Value(const Value& value) : m_Value(value.m_Value), m_nDepth(value.m_nDepth) {}
		Value(Value&& value) : m_Value(std::move(value.m_Value)), m_nDepth(value.m_nDepth) {}
		Value() : m_Value(nullptr), m_nDepth(0) {}


	public: // public funtions
		template<class T>
		T& GetValue();

		types GetType() const;
		bool HasValue();

	public: // operators
		Value& operator=(Value&& value);
		Value& operator=(const Value& value);
		Value& operator[](int index);
		Value& operator[](const char* key);

	public: // friend functions
		friend std::ostream& operator<<(std::ostream& os, Value& val);
		friend std::ostream& operator<<(std::ostream& os, Value&& val);

	private:
		static void spaceDepth(std::ostream& os, int depth);
		static void setDepth(Value& val, int depth);
		
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
		m_nDepth = value.m_nDepth;
		return *this;
	}

	inline Value& Value::operator=(const Value& value)
	{
		m_Value = value.m_Value;
		m_nDepth = value.m_nDepth;
		return *this;
	}

	inline Value::types Value::GetType() const
	{
		return static_cast<Value::types>(m_Value.index());
	}

	inline bool Value::HasValue()
	{
		return !m_Value.valueless_by_exception();
	}

	inline Value& Value::operator[](int index)
	{
		return GetValue<Array>()[index];
	}
	
	inline Value& Value::operator[](const char* key)
	{
		return GetValue<Object>()[key];
	}

	inline void Value::spaceDepth(std::ostream& os, int depth)
	{
		for (int i = 0; i < depth; i++)
			os << "    ";
	}
	
	inline void Value::setDepth(Value& val, int depth)
	{
		static auto isNotValue = 
			[](Value& v) {return v.GetType() == types::OBJECT || v.GetType() == types::ARRAY; };
		if (!isNotValue(val))
			return;
		val.m_nDepth = depth;
		if (val.GetType() == types::OBJECT)
		{
			for (auto& [key, value] : val.GetValue<Object>())
				if (isNotValue(value))
					Value::setDepth(value, depth + 1);
		}
		else
		{
			for (auto& value : val.GetValue<Array>())
				if (isNotValue(value))
					Value::setDepth(value, depth + 1);
		}
	}

	std::ostream& operator<<(std::ostream& os, Value& val)
	{
		switch (val.GetType())
		{
		case Value::types::BOOLEAN:
		{
			os << std::boolalpha << val.GetValue<bool>();
			break;
		}
		case Value::types::INT:
		{
			os << val.GetValue<int>();
			break;
		}
		case Value::types::DOUBLE:
		{
			os << val.GetValue<double>();
			break;
		}
		case Value::types::STRING:
		{
			os << '\"' << val.GetValue<std::string>() << '\"';
			break;
		}
		case Value::types::EMPTY:
		{
			os << "null";
			break;
		}
		case Value::types::ARRAY:
		{
			auto& arr = val.GetValue<Value::Array>();
			Value::setDepth(val, val.m_nDepth);
			os << "[\n";
			for (auto itr = arr.begin(); itr != arr.end(); itr++)
			{
				Value::spaceDepth(os, val.m_nDepth + 1);
				os << *itr;
				if (itr != arr.end() - 1)
				{
					os << ',';	
				}
				else
				{
					os << "\n";
					Value::spaceDepth(os, val.m_nDepth);
					os << ']';
					break;
				}
				os << "\n";
			}
			break;
		}
		case Value::types::OBJECT:
		{
			int cnt = 0;
			auto& obj = val.GetValue<Value::Object>();
			Value::setDepth(val, val.m_nDepth);
			os << "{\n";
			for (auto& [key, value] : obj)
			{
				Value::spaceDepth(os, val.m_nDepth + 1);
				os << '\"' << key << "\": " << value;
				if (cnt++ != obj.size() - 1)
				{
					os << ',';
				}
				else
				{
					os << "\n";
					Value::spaceDepth(os, val.m_nDepth);
					os << '}';
					break;
				}
				os << "\n";
			}
			break;
		}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Value&& val)
	{
		return operator<<(os, *(&val));
	}

}