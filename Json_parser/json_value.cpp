#include "json_value.h"
#include "functions.h"

// Default Constructor
JsonValue::JsonValue() : data(nullptr), type(Type::None) {}

// Constructor from Value
JsonValue::JsonValue(Value value) : data(std::move(value)), type(Type::Value) {}

// Constructor from Array
JsonValue::JsonValue(Array array) : data(std::move(array)), type(Type::Array) {}

// Constructor from Object
JsonValue::JsonValue(Object object) : data(std::move(object)), type(Type::Object) {}

// Constructor from Filepath
JsonValue::JsonValue(const std::string& filepath, bool isFile) : data(nullptr), type(Type::None) {
    std::string text = LoadFromFile(filepath);
	*this = parseJson(text);
}

// Constructor from JSON string
JsonValue::JsonValue(const std::string& text) : data(nullptr), type(Type::None) {
    *this = parseJson(text);
}

// Move Constructor
JsonValue::JsonValue(JsonValue&& other) noexcept : data(std::move(other.data)), type(other.type) {
    other.type = Type::None;
}

// Copy Constructor
JsonValue::JsonValue(const JsonValue& other)
    : data(std::visit([](const auto& v) -> Value {
    using T = std::decay_t<decltype(v)>;
    if constexpr (std::is_same_v<T, JsonValue::Object>) {
        // Deep copy the Object
        JsonValue::Object copiedObject;
        for (const auto& pair : v) {
            copiedObject.emplace_back(pair.first, std::make_shared<JsonValue>(*pair.second));
        }
        return copiedObject;
    }
    else if constexpr (std::is_same_v<T, JsonValue::Array>) {
        // Deep copy the Array
        JsonValue::Array copiedArray;
        for (const auto& item : v) {
            copiedArray.push_back(std::make_shared<JsonValue>(*item));
        }
        return copiedArray;
    }
    else {
        // Primitive types and nullptr_t can be copied directly
        return v;
    }
        }, other.data)),
    type(other.type) {}

// Destructor
JsonValue::~JsonValue() noexcept = default;

// Copy Assignment Operator
JsonValue& JsonValue::operator=(const JsonValue& other) {
    if (this != &other) {
        // Perform a deep copy for complex types in `data`
        data = std::visit([](const auto& v) -> Value {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, JsonValue::Object>) {
                // Deep copy the Object
                JsonValue::Object copiedObject;
                for (const auto& pair : v) {
                    copiedObject.emplace_back(pair.first, std::make_shared<JsonValue>(*pair.second));
                }
                return copiedObject;
            }
            else if constexpr (std::is_same_v<T, JsonValue::Array>) {
                // Deep copy the Array
                JsonValue::Array copiedArray;
                for (const auto& item : v) {
                    copiedArray.push_back(std::make_shared<JsonValue>(*item));
                }
                return copiedArray;
            }
            else {
                // Primitive types and nullptr_t can be copied directly
                return v;
            }
            }, other.data);

        // Copy the type
        type = other.type;
    }
    return *this;
}


// Move Assignment Operator
JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        type = other.type;
        other.type = Type::None;
    }
    return *this;
}

// Utility Methods
bool JsonValue::isObject() const {
    return type == Type::Object;
}

bool JsonValue::isArray() const {
    return type == Type::Array;
}

bool JsonValue::isValue() const {
    return type == Type::Value;
}

bool JsonValue::isNull() const {
    return std::holds_alternative<std::nullptr_t>(data);
}

JsonValue& JsonValue::operator[](const std::string& key) {
    if (type != Type::Object) {
        throw std::runtime_error("JsonValue is not an object");
    }

    // Search for the key in the Object
    auto it = std::find_if(std::get<Object>(data).begin(), std::get<Object>(data).end(),
        [&key](const std::pair<std::string, std::shared_ptr<JsonValue>>& p) {
            return p.first == key;
        });

    if (it != std::get<Object>(data).end()) {
        return *(it->second);  // Return the value associated with the key
    }

    // If key doesn't exist, create a new pair and add it to the Object
    std::get<Object>(data).emplace_back(key, std::make_shared<JsonValue>());
    return *(std::get<Object>(data).back().second);
}

JsonValue& JsonValue::operator[](const size_t& index) {
    if (type != Type::Array) {
        throw std::runtime_error("JsonValue is not an array");
    }

    // Access the element by index
    return *(std::get<Array>(data).at(index));
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    if (type != Type::Object) {
        throw std::runtime_error("JsonValue is not an object");
    }

    // Search for the key in the Object
    auto it = std::find_if(std::get<Object>(data).begin(), std::get<Object>(data).end(),
        [&key](const std::pair<std::string, std::shared_ptr<JsonValue>>& p) {
            return p.first == key;
        });

    if (it != std::get<Object>(data).end()) {
        return *(it->second);  // Return the value associated with the key
    }

    throw std::runtime_error("Key not found");
}

