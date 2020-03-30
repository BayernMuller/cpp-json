#pragma once
#include "Value.h"
#include <sstream>

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
	};
}