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
		json.data = parseObject(text, position);
	if (typeid(json.data) == typeid(JsonValue::Array))
		json.data = parseArray(text, position);
		return json;
}

JsonValue::Value parseValue(const std::string& json, size_t& position)
{
    SkipSpaces(json, position);
    char ch = json[position];

    if (ch == '{') {
        return parseObject(json, position);
    }
    else if (ch == '[') {
        return parseArray(json, position);
    }
    else if (ch == '"') {
        return parseString(json, position);
    }
    else if (isdigit(ch) || ch == '-') {
        size_t start = position;
        while (position < json.size() && (isdigit(json[position]) || json[position] == '.' || json[position] == '-')) {
            position++;
        }
        std::string numStr = json.substr(start, position - start);
        try {
            if (numStr.find('.') != std::string::npos) {
                return std::stod(numStr);  // Parse as double
            }
            else {
                return std::stoll(numStr);  // Parse as long long
            }
        }
        catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid number format");
        }
    }
    else if (json.substr(position, 4) == "true") {
        position += 4;
        return true;
    }
    else if (json.substr(position, 5) == "false") {
        position += 5;
        return false;
    }
    else if (json.substr(position, 4) == "null") {
        position += 4;
        return nullptr;
    }
    else {
        throw std::runtime_error("Invalid JSON value");
    }
}


JsonValue::Object parseObject(const std::string& json, size_t& position)
{
    JsonValue::Object obj;
    position++;  // Skip the '{'

    SkipSpaces(json, position);
    if (json[position] == '}') {
        position++;  // Empty object
        return obj;
    }

    while (true) {
        SkipSpaces(json, position);

        if (json[position] != '"') {
            throw std::runtime_error("Expected '\"' for key in object");
        }

        std::string key = parseString(json, position);  // Parse the key
        SkipSpaces(json, position);

        if (json[position] != ':') {
            throw std::runtime_error("Expected ':' after key");
        }
        position++;  // Skip ':'
        SkipSpaces(json, position);
        if (json[position] == '[') {
            JsonValue value = parseArray(json, position);
            obj.push_back({ key, std::make_shared<JsonValue>(value) });
        }
        else {
            JsonValue value = parseValue(json, position);
            obj.push_back({ key, std::make_shared<JsonValue>(value) });
        }
        SkipSpaces(json, position);
        if (json[position] == '}') {
            position++;  // End of object
            break;
        }
        else if (json[position] == ',') {
            position++;  // Skip the comma and continue
        }
        else {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }
    return obj;
}

JsonValue::Array parseArray(const std::string& json, size_t& position)
{
	JsonValue::Array arr;
    SkipSpaces(json, position);
	if (json[position] == ']') {
        position++;  // Empty object
        return arr;
	}
    while (true) {
        SkipSpaces(json, position);
        JsonValue value;
        value.data = parseValue(json, position);
        arr.push_back(std::make_shared<JsonValue>(value));
        SkipSpaces(json, position);
		if (json[position] == ']') {
			position++;  // End of array
			break;
		}
		else if (json[position] == ',') {
			position++;  // Skip the comma and continue
		}
		else {
			throw std::runtime_error("Expected ',' or ']' in array");
		}
	}
    return arr;

}

std::string parseString(const std::string& json, size_t& position)
{
    if (json[position] != '"') {
        throw std::runtime_error("Expected '\"' at the start of a string");
    }
    position++;  // Skip the opening quote

    std::string result;
    while (position < json.size() && json[position] != '"') {
        if (json[position] == '\\') {  // Handle escape sequences
            position++;
            if (position < json.size()) {
                switch (json[position]) {
                case '"':
                case '\\':
                    result.push_back(json[position]);
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                default:
                    throw std::runtime_error("Unknown escape character");
                }
            }
        }
        else {
            result.push_back(json[position]);
        }
        position++;
    }

    if (json[position] != '"') {
        throw std::runtime_error("Expected '\"' at the end of the string");
    }
    position++;  // Skip the closing quote
    return result;
}

