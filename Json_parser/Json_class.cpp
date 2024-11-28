#pragma once

#include "Json_class.h"
#include <fstream>
#include <sstream>

Json:: Json(std::string path) 
{
	std::string text = this->LoadFromFile(path);

}

JsonValue& Json:: operator[](const std::string& key) 
{
	// Access or insert the value for the given key
	return file[key];
}

const JsonValue& Json:: operator[](const std::string& key) const {
	// Check if the key exists using the contains method
	if (this->contains(key)) {
		return file.at(key);  // Safely return the value associated with the key
	}
	throw std::out_of_range("Key '" + key + "' not found in JSON.");
}

bool Json::contains(const std::string& key) const 
{
	return this->file.find(key) != this->file.end();
}


std::string Json::LoadFromFile(const std::string& fileName) {
	std::fstream json_file(fileName, std::ios::in);
	if (!json_file.is_open()) 
	{
		 throw std::runtime_error("Failed to open file: " + fileName);
	}
	std::ostringstream buffer;
	buffer << json_file.rdbuf();
	std::cout << buffer.str();	
	return buffer.str();

}	


void Json::print() const {}