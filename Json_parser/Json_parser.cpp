#pragma once
#include <iostream>
#include "json_value.h"
int main()
{
	JsonValue json("data.json", true);
	std::cout << json;
}
