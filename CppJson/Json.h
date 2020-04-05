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

		template<class T>
		static Json ToJson(T&& value)
		{
			return Json(std::forward<T>(value)); // RVO
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
			Iter end;
			std::string str;
			while (outIter != end)
			{
				str.append(*outIter);
				if (outIter->back() != '\"')
				{
					str.append(" ");
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