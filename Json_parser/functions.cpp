#include "functions.h"
#include <typeinfo>

void SkipSpaces(const std::string& text, size_t& position)
{
	while (text.size() != position and (isspace(text[position]) == ' ' or text[position] == '\n'))
		position++;
}

JsonValue parseJson(const std::string& text)
{
	size_t position = 0;
	SkipSpaces(text, position);
	JsonValue json;
	switch (text[position]) 
	{
	case '{':
		json.data = JsonValue::Object();
		break;
	case '[':
		json.data = JsonValue::Array();
		break;
	default:
		throw std::runtime_error("Invalid JSON format");
		
	}
	if (typeid(json.data) == typeid(JsonValue::Object)) 
	{
		json.data = std::get<JsonValue::Object>(parseObject(text, position));
	}
}

JsonValue parseValue(const std::string& json, size_t& position)
{
	return JsonValue();
}

JsonValue::Value parseObject(const std::string& json, size_t& pos)
{
	return JsonValue::Value();
}

JsonValue parseArray(const std::string& json, size_t& pos)
{
	return JsonValue();
}

std::string parseString(const std::string& json, size_t& pos)
{
	return std::string();
}


