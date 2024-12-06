#pragma once
#include "functions.h"
#include <typeinfo>
#include <fstream>
#include <sstream>
// skip Spaces in text 

void SkipSpaces(const std::string& text, size_t& position)
{
	while (text.size() != position and (isspace(text[position]) == ' ' || text[position] == '\n' || text[position] == 32))
		position++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//parsing given text

JsonValue parseJson(const std::string& text)
{
	size_t position = 0;
	SkipSpaces(text, position);
	JsonValue json;
	switch (text[position]) 
	{
	case '{':
		json.type = JsonValue::Type::Object;
		break;
	case '[':
        json.type = JsonValue::Type::Array;
		break;
	default:
        json.type = JsonValue::Type::Value;
	}
	if (json.type == JsonValue::Type::Object) 
		json.data = parseObject(text, position);
    else if (json.type == JsonValue::Type::Array)
        json.data = parseArray(text, position);
    else
        json.data = parseValue(text, position);
	return json;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Parse Value type from JsonValue, long long, double, string, bool, nullptr
JsonValue::Value parseValue(const std::string& json, size_t& position)
{
    SkipSpaces(json, position);
    char ch = json[position];

    if (ch == '"') {
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Parse Object type from JsonValue
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
        JsonValue value;

        if (json[position] == '[') {
            value.data = parseArray(json, position);
            value.type = JsonValue::Type::Array;
            obj.push_back({ key, std::make_shared<JsonValue>(value) });
        }
        else if (json[position] == '{') 
        {
            value.data = parseObject(json, position);
            value.type = JsonValue::Type::Object;
            obj.push_back({ key, std::make_shared<JsonValue>(value) });
		}
        else {
            value.data = parseValue(json, position);
            value.type = JsonValue::Type::Value;
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Parse Array type from JsonValue
JsonValue::Array parseArray(const std::string& json, size_t& position)
{
    position++;
	JsonValue::Array arr;
    SkipSpaces(json, position);
	if (json[position] == ']') {
        position++;  // Empty object
        return arr;
	}
    while (true) {
        SkipSpaces(json, position);
        JsonValue value;

		if (json[position] == '[') {
			value.data = parseArray(json, position);
            value.type = JsonValue::Type::Array;
			arr.push_back(std::make_shared<JsonValue>(value));
		}
		else if (json[position] == '{') {
			value.data = parseObject(json, position);
            value.type = JsonValue::Type::Object;
			arr.push_back(std::make_shared<JsonValue>(value));
		}
		else {
			value.data = parseValue(json, position);
            value.type = JsonValue::Type::Value;
			arr.push_back(std::make_shared<JsonValue>(value));
		}
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Parse String 
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Load Json Text from file
std::string LoadFromFile(const std::string& fileName) {
    std::fstream json_file(fileName, std::ios::in);
    if (!json_file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
    std::ostringstream buffer;
    buffer << json_file.rdbuf();
    return buffer.str();

}


const JsonValue& JsonValue::operator[](const size_t& index) const {
    if (type != Type::Array) {
        throw std::runtime_error("JsonValue is not an array");
    }

    // Access the element by index
    return *(std::get<Array>(data).at(index));
}



//output
std::ostream& operator<<(std::ostream& os, const JsonValue& jsonValue) {
    std::string indent(2, '\t');  // Create a string of tabs for indentation

    switch (jsonValue.type) {
    case JsonValue::Type::Object:
        os << "{\n";
        for (const auto& pair : std::get<JsonValue::Object>(jsonValue.data)) {
            os << "\"" << pair.first << "\": " << *pair.second;
            if (&pair != &std::get<JsonValue::Object>(jsonValue.data).back()) {
                os << ", ";
            }
            os << "{\n";

        }
        os << indent << "}";
        break;

    case JsonValue::Type::Array:
        os << "[\n";
        for (const auto& item : std::get<JsonValue::Array>(jsonValue.data)) {
            os << *item;
            if (&item != &std::get<JsonValue::Array>(jsonValue.data).back()) {
                os << ", ";
            }
            os << "\n";

        }
        os << indent << "]";
        break;

    case JsonValue::Type::Value:
        std::visit([&os](const auto& value) {
            if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
                os << "\"" << value << "\"";  // Print string values in quotes
            }
            else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, long long>) {
                os << value;  // Print integer values as they are
            }
            else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
                os << (value ? "true" : "false");  // Print booleans as true/false
            }
            else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, double>) {
                os << value;  // Print floating-point numbers as they are
            }
            else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::nullptr_t>) {
                os << "null";  // Print null values as "null"
            }
            }, jsonValue.data);  // Use std::visit to handle the variant types
        break;

    case JsonValue::Type::None:
        os << "null";  // If the type is "None", output "null"
        break;
    }


    return os;
}
