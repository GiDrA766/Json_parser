#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>

// Use an include guard to prevent multiple inclusions.
#ifndef JSON_VALUE_H
#define JSON_VALUE_H

// Define the possible types for a JSON value.
using JsonValue =  std::variant<
    std::nullptr_t,        // null
    bool,                  // boolean
    int,                   // integer
    double,                // floating-point number
    std::string,           // string
    std::vector<JsonValue>,// array
    std::map<std::string, std::variant<
    std::nullptr_t, bool, int, double, std::string, std::vector<JsonValue>>> // object
    >;

#endif // JSON_VALUE_H
