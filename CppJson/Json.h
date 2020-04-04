#pragma once
#include "Value.h"
#include <sstream>
#include <fstream>
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

		static bool WriteJson(std::ofstream& file, Json& json)
		{
			return (file << json).operator bool();
		}

		static Json ReadJson(std::istream& file)
		{
			
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
			return ParseValue(begin);
		}

	private:
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
			return Value(std::move(obj)); // RVO
		}

		static Value ParseArray(Iter& outIter)
		{
			Array arr;
			outIter++; // exclude '['
			for (; (*outIter)[0] != ']'; outIter++)
			{
				arr.push_back(std::move(ParseValue(outIter)));
			}
			return Value(std::move(arr)); // RVO
		}

		static Value ParseString(Iter& iter)
		{
			std::string str(iter->begin() + 1, iter->end() - 1);
			return Value(str.c_str()); // RVO
		}

		static Value ParseNumber(Iter& iter)
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

		static Value ParseBoolean(Iter& iter)
		{
			return Value(iter->front() == 't'); // RVO
		}

		static Value ParseNull(Iter& iter)
		{
			return Value(Null); // RVO
		}
	};
}