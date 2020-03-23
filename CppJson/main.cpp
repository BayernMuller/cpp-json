#include <iostream>
#include "Json.h"

int main()
{
	json::Value null(nullptr);
	json::Value boolean(false);
	json::Value integer(45);
	json::Value rational(1.9);
	json::Value str("string");

	std::cout << null << std::endl;
	std::cout << boolean << std::endl;
	std::cout << integer << std::endl;
	std::cout << rational << std::endl;
	std::cout << str << std::endl;
	return 0;
}