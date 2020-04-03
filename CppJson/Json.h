#pragma once
#include "Value.h"
#include <sstream>
#include <iterator>

namespace json
{
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

		static Value Parse(std::string src)
		{
			for (char& ch : src)
			{
				if (ch == ',' || ch == ':' || ch == '\t')
					ch = ' ';
			}
			std::istringstream iss(std::move(src));
			Iter begin(iss);
			return ParseObject(begin);
		}

	public: // public is just for test
		static Value ParseValue(Iter& outIter)
		{
			char first = outIter->front();
			static std::map<char, Value(*)(Iter&)> parse
			{
				{'{', ParseObject},
				{'[', ParseArray},
				{'\"',ParseString},
				{'n', ParseNull},
				{'t', ParseBoolean},
				{'f', ParseBoolean},
			};
			if (std::isdigit(first) || first == '-')
				return ParseNumber(outIter);
			return parse[first](outIter);
		}

		static Value ParseObject(Iter& outIter)
		{
			Object obj;
			outIter++; // exclude '{'
			for (; (*outIter)[0] != '}'; outIter++)
			{
				auto& value = *outIter;
				std::string key(value.begin() + 1, value.end() - 1);
				outIter++;
				obj[key] = ParseValue(outIter);
			}
			outIter++;
			return Value(std::move(obj)); // NRVO
		}

		static Value ParseArray(Iter& outIter)
		{
			Array arr;
			outIter++; // exclude '['
			for (; (*outIter)[0] != ']'; outIter++)
			{
				arr.push_back(std::move(ParseValue(outIter)));
			}
			outIter++;
			return Value(std::move(arr)); // RVO
		}

		static Value ParseString(Iter& outIter)
		{
			std::string str(outIter->begin() + 1, outIter->end() - 1);
			outIter++;
			return Value(str.c_str()); // RVO
		}

		static Value ParseNumber(Iter& outIter)
		{
			std::istringstream iss(*outIter);
			for (const auto& ch : *outIter)
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

		static Value ParseBoolean(Iter& outIter)
		{
			return (outIter++)->front() == 't';
		}

		static Value ParseNull(Iter& outIter)
		{
			outIter++;
			return Value(Null);
		}
	};
}