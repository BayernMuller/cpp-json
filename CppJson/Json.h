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

	private:
		static Value ParseValue(Iter& outIter)
		{
			
		}
	};
}