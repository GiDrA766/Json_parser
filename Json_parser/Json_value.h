#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <memory>

class JsonValue
{
public:
    using Object = std::vector<std::pair<std::string, JsonValue*>>;
    using Value = std::variant<std::string, long long, bool, double, std::nullptr_t, Object>;
    using Array = std::vector<Value>;
private:
    std::variant<Value, Array, Object> data;
    friend JsonValue parseJson(const std::string& text);
public:
    JsonValue() = default;
    JsonValue(Value value);
    JsonValue(Array array);

    JsonValue(JsonValue&& other);

    // Copy constructor
    JsonValue(const JsonValue& other);

    // Copy assignment operator
    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;
};