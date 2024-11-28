#include "Json_value.h"

JsonValue::JsonValue(Value value) : data(std::move(value)) {}

JsonValue::JsonValue(Array array) : data(std::move(array)) {}

JsonValue::JsonValue(JsonValue&& other)
{
}

// Note: The move constructor is defaulted in the header file, so we don't need to implement it here.
// JsonValue::JsonValue(JsonValue&& other) noexcept = default;

// If you want to add the Object constructor as suggested earlier:

// If you decide to add copy constructor and assignment operator:
JsonValue::JsonValue(const JsonValue& other) = default;
JsonValue& JsonValue::operator=(const JsonValue& other) = default;

//If you decide to add move assignment operator:
JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
	if (this != &other) {
		data = std::move(other.data);
	}
	return *this;
};