#include <iostream>
#include "Json.h"

int main()
{
	Value null(nullptr);
	Value boolean(false);
	Value integer(45);
	Value rational(1.9);
	Value str("string");

	std::cout << null << std::endl;
	std::cout << boolean << std::endl;
	std::cout << integer << std::endl;
	std::cout << rational << std::endl;
	std::cout << str << std::endl;
	std::cin.get();
	return 0;
}