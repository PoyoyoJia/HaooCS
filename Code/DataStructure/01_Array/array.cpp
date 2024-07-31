#include "array_hao.h"

#include <iostream>

using Haoo::Array;

int main() {
	system("chcp 65001");

	int initVal = 0;
	Array<int> array(4, &initVal);
	array.Insert(0, 1);

	try {
		array[0] = 2;
		std::cout << array.Get(0) << std::endl;
		std::cout << array.Get(1) << std::endl;
		std::cout << array[0] << std::endl;
	}
	catch (std::exception& ex) {
		std::cout << "get error " << ex.what() << std::endl;
	}
	
	std::cout << array << std::endl;
}