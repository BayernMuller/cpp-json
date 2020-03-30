#pragma once
#include "Value.h"
#include <sstream>
#include <iterator>

namespace json
{
	constexpr Value::Null Null = nullptr;
	using value_type = Value::types;
	using Array = Value::Array;
	using Object = Value::Object;
	using JsonMaker = Value::Object;

	class Json
	{
	public:
		static std::string Dumps(JsonMaker& obj)
		{
			std::ostringstream oss;
			oss << obj;
			return oss.str();
		}

		static void Parse(std::string src, Object& dst)
		{
			for (char& ch : src)
			{
				if (ch == ',' || ch == '\"' || ch == ':')
					ch = ' ';
			}
			std::istringstream iss(src);
			std::istream_iterator<std::string> begin(iss), end;
			
			std::vector<std::string> vec(begin, end);
			std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
		}
	};
}