#pragma once
#include <iostream>
#include "Json_value.h"

//skip unnecessary spaces in json
void  SkipSpaces(const std::string& text, size_t& position);

// starting point of parsing
JsonValue parseJson(const std::string& json);
// Parse Value
JsonValue parseValue(const std::string& json, size_t& position);
// Parse Value
JsonValue parseObject(const std::string& json, size_t& pos);
// Parse Array
JsonValue parseArray(const std::string& json, size_t& pos);
// Parse String
std::string parseString(const std::string& json, size_t& pos);
