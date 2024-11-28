	#pragma once
	#include <iostream>
	#include "Json_value.h"


	class Json
	{
	
	public:
		Json() = default;
		Json(std::string path);
		~Json() = default;

		// getting value from key with reference, that can be changable
		JsonValue& operator[](const std::string& key);
		// getting value from key with constant
		const JsonValue& operator[](const std::string& key) const;
		//checking keys presence is json
		bool contains(const std::string& key) const;

		std::string LoadFromFile(const std::string& key);
		//print json
		void print() const;
	private:
		std::map <std::string, JsonValue> file;

	}; 



	