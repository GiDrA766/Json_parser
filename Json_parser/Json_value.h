#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

class Json;

class JsonValue {
public:
    using Object = std::vector<std::pair<std::string, std::shared_ptr<JsonValue>>>;
    using Array = std::vector<std::shared_ptr<JsonValue>>;
    using Value = std::variant<std::string, long long, bool, double, std::nullptr_t, Object, Array>;

    enum class Type { Object, Array, Value, None };

private:
    Value data;
    Type type;

    friend std::ostream& operator<<(std::ostream& os, const JsonValue& jsonValue);
    friend JsonValue parseJson(const std::string& text);
    friend JsonValue::Object parseObject(const std::string& json, size_t& pos);
    friend JsonValue::Array parseArray(const std::string& json, size_t& pos);
    friend std::string parseString(const std::string& json, size_t& pos);
    friend JsonValue::Value parseValue(const std::string& json, size_t& position);
    friend class Json;

public:
    // Constructors
    JsonValue();
    explicit JsonValue(Value value);
    explicit JsonValue(Array array);
    explicit JsonValue(Object object);
    explicit JsonValue(const std::string& filepath, bool isFile);
    explicit JsonValue(const std::string& text);
    JsonValue(JsonValue&& other) noexcept;
    JsonValue(const JsonValue& other);

    // Destructor
    ~JsonValue() noexcept;

    // Assignment Operators
    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;
    JsonValue& operator=(const Value& value);

    // Utility Methods (optional)
    bool isObject() const;
    bool isArray() const;
    bool isValue() const;
    bool isNull() const;

    // operators[] for objects
    JsonValue& operator[](const std::string& key);
	const JsonValue& operator[](const std::string& key) const;
    
    // operator[] for arrays
    JsonValue& operator[](const std::size_t& index);
	const JsonValue& operator[](const std::size_t& index) const;
private:
    static void printWithIndent(std::ostream& os, const JsonValue& jsonValue, int indentLevel);
};
