#include "Hashtable.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

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

// file has to contain atleast 30 lines, every lines gets read and the last 30 get put into another vector
void Hashtable::import(int index) {
	std::fstream source;
	source.open("./../import/" + m_table[index].abbreviation + ".csv");
	short int line_count = 0;
	std::string line;
	std::vector<std::vector<std::string>> history_tmp;
	while(std::getline(source, line)) {
		std::string line_value;
		std::vector<std::string> line_values;
		std::stringstream ss(line);
		while(std::getline(ss, line_value, ',')) {
			line_values.push_back(line_value);
		}
		history_tmp.emplace_back(line_values);
		line_count++;
	}

	// absolutely horrendous, but it works
	for(int i = line_count - 30; i < line_count; ++i) {
		Data data(history_tmp[i][0], std::stod(history_tmp[i][1]), std::stod(history_tmp[i][2]), std::stod(history_tmp[i][3]), std::stod(history_tmp[i][4]), std::stod(history_tmp[i][5]), std::stoull(history_tmp[i][6]));
		m_table[index].history.emplace_back(data);
	}

	source.close();
}