#pragma once
#include <iostream>
#include "json_value.h"
int main()
{
	JsonValue json("data.json", true);
	json["new_string"] = "i am the new column";
	std::cout << json;
}
