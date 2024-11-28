#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <optional>

class JsonValue
{
private:	
	using Object = std::vector<std::pair<std::string, std::unique_ptr<JsonValue>>>;
	using Value = std::variant<std::string, long long, bool, double, std::nullptr_t, Object>;
	using Array = std::vector<Value>;

	std::variant<Value, Array, Object> data;
public:
	JsonValue() = default;
	JsonValue(Value value);
	JsonValue(Array array);
	JsonValue(JsonValue&& other) noexcept = default;
	// Copy constructor
	JsonValue(const JsonValue& other) = default;

	// Copy assignment operator
	JsonValue& operator=(const JsonValue& other) = default;
	JsonValue& operator=(JsonValue&& other) noexcept = default;

};