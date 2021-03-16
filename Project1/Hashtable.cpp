#include "Hashtable.h"
#include <iostream>
#include <cstdint>

Hashtable::Hashtable() {
	m_table = new Stock[2003];
}

Hashtable::~Hashtable() {
	delete[] m_table;
}

int Hashtable::hash(std::string abbreviation) {
	uint64_t value = 1;
	for(int i = 0; i < (int)abbreviation.length(); ++i) {
		value *= (int)abbreviation[i];
	}

	return value % 2003;
}

void Hashtable::add(uint32_t index, Stock stock, uint32_t qu_pr) {
	uint32_t index_tmp = (index + qu_pr * qu_pr) % 2003;
	if(!m_table[index_tmp].filled) { // if not filled
		m_table[index_tmp] = stock;
	}
	else {
		//std::cout << "Finding new place..." << std::endl;
		add(index, stock, qu_pr + 1);
	}
}