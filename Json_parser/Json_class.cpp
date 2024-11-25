#pragma once;
#include "Json_class.h"

Json:: Json(std::string path) 
{
	this->LoadFromFile(path);
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


void Json::LoadFromFile(const std::string& key) {}


void Json::print() const {}