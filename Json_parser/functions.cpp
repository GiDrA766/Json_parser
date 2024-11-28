#include "functions.h"

void SkipSpaces(const std::string& text, size_t& position)
{
	while (text.size() != position and isspace(text[position]) == ' ')
		position++;
}

JsonValue parseJson(const std::string& json)
{
	return JsonValue();
}

JsonValue parseValue(const std::string& json, size_t& position)
{
	return JsonValue();
}

JsonValue parseObject(const std::string& json, size_t& pos)
{
	return JsonValue();
}

JsonValue parseArray(const std::string& json, size_t& pos)
{
	return JsonValue();
}

std::string parseString(const std::string& json, size_t& pos)
{
	return std::string();
}


