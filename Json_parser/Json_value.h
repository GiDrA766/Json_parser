#pragma once
#include <variant>
#include <vector>
#include <string>
#include <map>
#include <optional>

using JsonValuePtr = std::unique_ptr<JsonValue>;

using JsonValue = std::variant<
    std::nullptr_t,
    bool,
    int,
    double,
    std::string,
    std::vector<JsonValuePtr>,
    std::map<std::string, JsonValuePtr>
>;
