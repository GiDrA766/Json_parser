#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <memory>

class Json;

class JsonValue
{
public:
    using Object = std::vector<std::pair<std::string, std::shared_ptr<JsonValue>>>;
    using Array = std::vector<std::shared_ptr<JsonValue>>;
    using Value = std::variant<std::string, long long, bool, double, std::nullptr_t, Object, Array>;
private:
    Value data;
    friend JsonValue parseJson(const std::string& text);
	friend JsonValue::Object parseObject(const std::string& json, size_t& pos);
    friend JsonValue::Array parseArray(const std::string& json, size_t& pos);
	friend std::string parseString(const std::string& json, size_t& pos);
    friend JsonValue::Value parseValue(const std::string& json, size_t& position);
    friend class Json;
public:
    JsonValue() = default;
    JsonValue(Value value);
    JsonValue(Array array);

    JsonValue(JsonValue&& other) noexcept;

    // Copy constructor
    JsonValue(const JsonValue& other);
	

    //destructor
	~JsonValue() noexcept;


    // Copy assignment operator
    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;
};