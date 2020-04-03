#pragma once
#include "Value.h"
#include <sstream>
#include <iterator>

namespace json
{
	using value_type = Value::types;
	using Array = Value::Array;
	using Object = Value::Object;
	using JsonObject = Value::Object;
	constexpr Value::Null Null = nullptr;

	/* ---------------------------- */

	class Json
	{
		using Iter = std::istream_iterator<std::string>;
	public:
		static std::string Dumps(JsonObject& obj)
		{
			std::ostringstream oss;
			oss << obj;
			return oss.str();
		}

		static JsonObject Parse(std::string src)
		{
			JsonObject object;
			for (char& ch : src)
			{
				if (ch == ',' || ch == ':')
					ch = ' ';
			}
			std::istringstream iss(std::move(src));
			Iter begin(iss), end;
			return object;
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
				{'t', ParseBoolean },
				{'f', ParseBoolean },
			};
			if (std::isdigit(first) || first == '-')
				return ParseNumber(outIter);
			return parse[first];
		}

		static Value ParseObject(Iter& outIter)
		{
			
		}

		static Value ParseArray(Iter& outIter)
		{

		}

		static Value ParseString(Iter& outIter)
		{

		}

		static Value ParseNumber(Iter& outIter)
		{

		}

		static Value ParseBoolean(Iter& outIter)
		{
			return (outIter++)->front() == 't';
		}

		static Value ParseNull(Iter& outIter)
		{

		}
	};
}