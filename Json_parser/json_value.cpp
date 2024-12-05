#include "Json_value.h"

JsonValue::JsonValue() : type(JsonValue::Type::None), data(nullptr){}
JsonValue::JsonValue(Value value) :type(JsonValue::Type::Value), data(std::move(value)) {}

JsonValue::JsonValue(Array array) :type(JsonValue::Type::Array), data(std::move(array)) {}

JsonValue::JsonValue(const JsonValue& other)
{
    if (std::holds_alternative<JsonValue::Object>(other.data)) {
        // Deep copy the Object, iterating through the pairs
        JsonValue::Object newObject;
        for (const auto& pair : std::get<JsonValue::Object>(other.data)) {
            // Recursively copy each element (shared_ptr<JsonValue>)
            newObject.push_back({ pair.first, std::make_shared<JsonValue>(*pair.second) });
        }
        data = std::move(newObject);
        type = other.type;

    }
    else if (std::holds_alternative<JsonValue::Array>(other.data)) {
        // Deep copy the Array
        JsonValue::Array newArray;
        for (const auto& item : std::get<JsonValue::Array>(other.data)) {
            // Recursively copy each shared_ptr<JsonValue> in the Array
            newArray.push_back(std::make_shared<JsonValue>(*item));
        }
        data = std::move(newArray);
        type = other.type;
    }
    else {
        // For primitive types (like string, long long, etc.), just copy them
        data = other.data;
		type = other.type;
    }
}


JsonValue::JsonValue(JsonValue&& other) noexcept
	: data(std::move(other.data)), type(other.type) // Move the variant (internal data) from the other object to this one
{

}

JsonValue:: ~JsonValue() {}


// Note: The move constructor is defaulted in the header file, so we don't need to implement it here.
// JsonValue::JsonValue(JsonValue&& other) noexcept = default;

// If you want to add the Object constructor as suggested earlier:

// If you decide to add copy constructor and assignment operator:
JsonValue& JsonValue::operator=(const JsonValue& other) {
    if (this != &other) {
        data = std::visit([](const auto& value) -> Value {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, Object>) {
                Object newObject;
                for (const auto& pair : value) {
                    newObject.emplace_back(pair.first, std::make_shared<JsonValue>(*pair.second));
                }
                return newObject;
            }
            else if constexpr (std::is_same_v<T, Array>) {
                Array newArray;
                for (const auto& elem : value) {
                    newArray.push_back(std::make_shared<JsonValue>(*elem));
                }
                return newArray;
            }
            else {
                return value;  // Primitive types can be copied directly.
            }
            }, other.data);
    }
    return *this;
}


//If you decide to add move assignment operator:
JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
    }
    return *this;
}
