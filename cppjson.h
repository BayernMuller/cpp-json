#pragma once
#include <sstream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include <variant>

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
		Value(bool value)				: m_Value(value), m_nDepth(0) {}
		Value(int value)				: m_Value(value), m_nDepth(0) {}
		Value(double value)				: m_Value(value), m_nDepth(0) {}
		Value(const char* str)			: m_Value(std::move(std::string(str))), m_nDepth(0) {}
		Value(Null value)				: m_Value(value), m_nDepth(0) {}
		Value(std::string&& str)		: m_Value(std::move(str)), m_nDepth(0) {}
		Value(Array&& arr)				: m_Value(std::move(arr)), m_nDepth(0) {}
		Value(Object&& obj)				: m_Value(std::move(obj)), m_nDepth(0) {}
		Value(const std::string& str)	: m_Value(str), m_nDepth(0) {}
		Value(const Array& arr)			: m_Value(arr), m_nDepth(0) {}
		Value(const Object& obj)		: m_Value(obj), m_nDepth(0) {}
		Value(const Value& value)		: m_Value(value.m_Value), m_nDepth(value.m_nDepth) {}
		Value(Value&& value)			: m_Value(std::move(value.m_Value)), m_nDepth(value.m_nDepth) {}
		Value()							: m_Value(nullptr), m_nDepth(0) {}


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
					os << '\n';
					Value::spaceDepth(os, val.m_nDepth);
					os << ']';
					break;
				}
				os << '\n';
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
					os << '\n';
					Value::spaceDepth(os, val.m_nDepth);
					os << '}';
					break;
				}
				os << '\n';
			}
			break;
		}
		}
		return os;
	}

	using value_type = Value::types;
	using Array = Value::Array;
	using Object = Value::Object;
	using JsonCreator = Value::Object;
	using Json = Value;
	constexpr Value::Null Null = nullptr;

	/* ---------------------------- */

	class Utility
	{ 
		using Iter = std::istream_iterator<std::string>;
	public:
		template<class T>
		static std::string Dumps(T& obj)
		{
			std::ostringstream oss;
			oss << obj;
			return oss.str();
		}

		template<class T>
		static Json ToJson(T&& value)
		{
			return Json(std::forward<T>(value)); // RVO
		}

		static bool WriteJson(std::ofstream& file, Json& json)
		{
			return static_cast<bool>(file << json);
		}

		static Json LoadJson(std::ifstream& file)
		{
			std::istreambuf_iterator<char> begin(file), end;
			std::string str(begin, end);
			return Parse(std::move(str)); // RVO
		}

		static Json Parse(std::string src)
		{
			std::istringstream iss;
			refineString(src);
			iss.str(src);
			Iter begin(iss);
			return parseValue(begin); // RVO
		}


	private:
		static Value parseValue(Iter& outIter)
		{
			char first = outIter->front();
			static std::map<char, Value(*)(Iter&)> parse
			{
				{'{', parseObject},
				{'[', parseArray},
				{'\"',parseString},
				{'n', parseNull},
				{'t', parseBoolean},
				{'f', parseBoolean},
			};
			if (std::isdigit(first) || first == '-')
				return parseNumber(outIter);
			return parse[first](outIter);
		}

		static Value parseObject(Iter& outIter)
		{
			Object obj;
			outIter++; // exclude '{'
			for (; (*outIter)[0] != '}'; outIter++)
			{
				std::string key = takeString(outIter);
				outIter++;
				obj[key] = parseValue(outIter);
			}
			return Value(std::move(obj)); // RVO
		}

		static Value parseArray(Iter& outIter)
		{
			Array arr;
			outIter++; // exclude '['
			for (; (*outIter)[0] != ']'; outIter++)
			{
				arr.push_back(std::move(parseValue(outIter)));
			}
			return Value(std::move(arr)); // RVO
		}

		static Value parseString(Iter& outIter)
		{
			return Value(std::move(takeString(outIter))); // RVO
		}

		static Value parseNumber(Iter& iter)
		{
			std::istringstream iss(*iter);
			for (const auto& ch : *iter)
			{
				if (ch == '.' || ch == 'e' || ch == 'E')
				{
					double number;
					iss >> number;
					return Value(number); // RVO
				}
			}
			int integer;
			iss >> integer;
			return Value(integer); // RVO
		}

		static Value parseBoolean(Iter& iter)
		{
			return Value(iter->front() == 't'); // RVO
		}

		static Value parseNull(Iter& iter)
		{
			return Value(Null); // RVO
		}
		
		static std::string takeString(Iter& outIter)
		{
			static const Iter end;
			std::string str;
			while (outIter != end)
			{
				str.append(std::move(*outIter));
				if (outIter->back() != '\"')
				{
					str += ' ';
					outIter++;
					continue;
				}
				break;
			}
			str.assign(str.begin() + 1, str.end() - 1);
			return str; // NRVO
		}

		static void refineString(std::string& str)
		{
			for (auto itr = str.begin(); itr != str.end(); itr++)
			{
				char& ch = *itr;
				if (ch == ',' || ch == ':' || ch == '\t')
				{
					ch = ' ';
				}
				else if (ch == '[' || ch == '{')
				{
					auto next = itr + 1;
					if (*next != ' ' && *next != '\n')
					{
						itr = str.insert(next, ' ');
					}
				}
				else if (ch == ']' || ch == '}')
				{
					auto prev = itr - 1;
					if (*prev != ' ' && *prev != '\n')
					{
						str.insert(itr, ' ');
						itr++;
					}
				}
			}
		}

	};

	/* ------------------ */

	Json operator "" _Json(const char* str, std::size_t)
	{
		return Utility::Parse(str);
	}
}