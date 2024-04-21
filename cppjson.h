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
		enum class Types { BOOLEAN, INT, DOUBLE, STRING, EMPTY, ARRAY, OBJECT };
		using Array = std::vector<Value>;
		using Object = std::map<std::string, Value>;
		using Null = std::nullptr_t;
		using JsonValue = std::variant<bool, int, double, std::string, Null, Array, Object>;

	public: // constructors
		Value(bool value)				: value_(value), depth_(0) {}
		Value(int value)				: value_(value), depth_(0) {}
		Value(double value)				: value_(value), depth_(0) {}
		Value(const char* str)			: value_(std::move(std::string(str))), depth_(0) {}
		Value(Null value)				: value_(value), depth_(0) {}
		Value(std::string&& str)		: value_(std::move(str)), depth_(0) {}
		Value(Array&& arr)				: value_(std::move(arr)), depth_(0) {}
		Value(Object&& obj)				: value_(std::move(obj)), depth_(0) {}
		Value(const std::string& str)	: value_(str), depth_(0) {}
		Value(const Array& arr)			: value_(arr), depth_(0) {}
		Value(const Object& obj)		: value_(obj), depth_(0) {}
		Value(const Value& value)		: value_(value.value_), depth_(value.depth_) {}
		Value(Value&& value)			: value_(std::move(value.value_)), depth_(value.depth_) {}
		Value()							: value_(nullptr), depth_(0) {}
		
		Value(std::initializer_list<std::pair<const char*, Value>> list)
			: value_(Object()), depth_(0)
		{
			Object obj;
			for (auto& [key, value] : list)
				obj[key] = value;
			value_ = std::move(obj);
		}



	public: // public funtions
		template<class T>
		T& GetValue();

		Types GetType() const;
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
		JsonValue value_;
		int depth_;
	};

	///////////////////////////////////////////////////////

	template<class T>
	inline T& Value::GetValue()
	{
		return std::get<T>(value_);
	}

	inline Value& Value::operator=(Value&& value)
	{
		value_ = std::move(value.value_);
		depth_ = value.depth_;
		return *this;
	}

	inline Value& Value::operator=(const Value& value)
	{
		value_ = value.value_;
		depth_ = value.depth_;
		return *this;
	}

	inline Value::Types Value::GetType() const
	{
		return static_cast<Value::Types>(value_.index());
	}

	inline bool Value::HasValue()
	{
		return !value_.valueless_by_exception();
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
			[](Value& v) {return v.GetType() == Types::OBJECT || v.GetType() == Types::ARRAY; };
		if (!isNotValue(val))
			return;
		val.depth_ = depth;
		if (val.GetType() == Types::OBJECT)
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
		case Value::Types::BOOLEAN:
		{
			os << std::boolalpha << val.GetValue<bool>();
			break;
		}
		case Value::Types::INT:
		{
			os << val.GetValue<int>();
			break;
		}
		case Value::Types::DOUBLE:
		{
			os << val.GetValue<double>();
			break;
		}
		case Value::Types::STRING:
		{
			os << '\"' << val.GetValue<std::string>() << '\"';
			break;
		}
		case Value::Types::EMPTY:
		{
			os << "null";
			break;
		}
		case Value::Types::ARRAY:
		{
			auto& arr = val.GetValue<Value::Array>();
			Value::setDepth(val, val.depth_);
			os << "[\n";
			for (auto itr = arr.begin(); itr != arr.end(); itr++)
			{
				Value::spaceDepth(os, val.depth_ + 1);
				os << *itr;
				if (itr != arr.end() - 1)
				{
					os << ',';	
				}
				else
				{
					os << '\n';
					Value::spaceDepth(os, val.depth_);
					os << ']';
					break;
				}
				os << '\n';
			}
			break;
		}
		case Value::Types::OBJECT:
		{
			int cnt = 0;
			auto& obj = val.GetValue<Value::Object>();
			Value::setDepth(val, val.depth_);
			os << "{\n";
			for (auto& [key, value] : obj)
			{
				Value::spaceDepth(os, val.depth_ + 1);
				os << '\"' << key << "\": " << value;
				if (cnt++ != obj.size() - 1)
				{
					os << ',';
				}
				else
				{
					os << '\n';
					Value::spaceDepth(os, val.depth_);
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

	using Array = Value::Array;
	using Object = Value::Object;
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
		static Value ToJson(T&& value)
		{
			return Value(std::forward<T>(value)); // RVO
		}

		static bool WriteJson(std::ofstream& file, Value& json)
		{
			return static_cast<bool>(file << json);
		}

		static Value LoadJson(std::ifstream& file)
		{
			std::istreambuf_iterator<char> begin(file), end;
			std::string str(begin, end);
			return Parse(std::move(str)); // RVO
		}

		static Value Parse(std::string src)
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
			static const std::map<char, Value(*)(Iter&)> parse
			{
				{'{', parseObject},
				{'[', parseArray},
				{'\"',parseString},
				{'n', parseNull},
				{'t', parseBoolean},
				{'f', parseBoolean}
			};
			if (std::isdigit(first) || first == '-')
				return parseNumber(outIter);
			return parse.at(first)(outIter);
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

	Value operator "" _Json(const char* str, std::size_t)
	{
		return Utility::Parse(str);
	}
}