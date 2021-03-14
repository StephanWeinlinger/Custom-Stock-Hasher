#include <iostream>
#include <string>
#include "Hashtable.h"

int main() {
	Hashtable hashtable;
	std::cout << hashtable.m_table[0].name << std::endl;
	return 0;
}