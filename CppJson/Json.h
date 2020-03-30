#pragma once
#include "Value.h"

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
		static std::string Dumps(Object& obj)
		{
			
		}
	};
}