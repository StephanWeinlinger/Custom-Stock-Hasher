#include "Hashtable.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

Hashtable::Hashtable() {
	m_table = new Stock[2003];
	m_dictionary = new Entry[2003];
}

Hashtable::~Hashtable() {
	delete[] m_table;
	delete[] m_dictionary;
}

int Hashtable::hash(std::string abbreviation) {
	uint64_t value = 1;
	for(int i = 0; i < (int)abbreviation.length(); ++i) {
		value *= (int)abbreviation[i];
	}
	return value % 2003;
}

void Hashtable::addStock(uint32_t index, Stock stock, uint32_t qu_pr) {
	uint32_t index_tmp = (index + qu_pr * qu_pr) % 2003;
	if(!m_table[index_tmp].filled) { // if not filled
		m_table[index_tmp] = stock;
	}
	else {
		addStock(index, stock, qu_pr + 1);
	}
}

void Hashtable::addEntry(uint32_t indexEntry, uint32_t indexStock, Stock stock, uint32_t qu_pr) {
	uint32_t index_tmp = (indexEntry + qu_pr * qu_pr) % 2003;
	if(m_dictionary[index_tmp].m_name == "") {
		m_dictionary[index_tmp].m_deleted = false;
		m_dictionary[index_tmp].m_name = stock.name;
		m_dictionary[index_tmp].m_indexStock = indexStock;
	}
	else {
		addEntry(indexEntry, indexStock, stock, qu_pr + 1);
	}
}

// file has to contain atleast 30 lines, every lines gets read and the last 30 get put into another vector
bool Hashtable::import(int index) {
	std::fstream source;
	source.open("./../import/" + m_table[index].abbreviation + ".csv");
	if(!source.is_open()) {
		return false;
	}
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
	m_table[index].history.clear(); // if new import happens
	for(int i = line_count - 30; i < line_count; ++i) {
		Data data(history_tmp[i][0], std::stod(history_tmp[i][1]), std::stod(history_tmp[i][2]), std::stod(history_tmp[i][3]), std::stod(history_tmp[i][4]), std::stod(history_tmp[i][5]), std::stoull(history_tmp[i][6]));
		m_table[index].history.emplace_back(data);
	}
	source.close();
	return true;
}

// hashtable and dictionary get saved in save.csv (note that the entries are separated by semicolons, since some companies have "," in their name)
bool Hashtable::save() {
	std::ofstream destination("./../save/save.csv", std::ios::trunc);
	if(!destination.is_open()) {
		return false;
	}

	for(int i = 0; i < 2003; ++i) {
		if(m_table[i].deleted) {
			destination << i << "\n"; // only index indicates deleted stock
		}
		else if(m_table[i].filled) {
			destination << i << "," << m_table[i].name << "," << m_table[i].isin << "," << m_table[i].abbreviation;
			if(!m_table[i].history.empty()) {
				for(std::vector<Data>::iterator it = m_table[i].history.begin(); it != m_table[i].history.end(); ++it) {
					destination << "," << it->m_date << "," << it->m_open << "," << it->m_high << "," << it->m_low << "," << it->m_close << "," << it->m_adjclose << "," << it->m_volume;
				}
			}
			destination << "\n";
		}
		// rest is completely empty
	}
	destination << "START_DICTIONARY\n";
	for(int i = 0; i < 2003; ++i) {
		if(m_dictionary[i].m_deleted) {
			destination << i << "\n"; // only index indicates deleted entry
		}
		else if(!m_dictionary[i].m_name.empty()) {
			destination << i << "," << m_dictionary[i].m_name << "," << m_dictionary[i].m_indexStock << "\n";
		}
		// rest is completely empty
	}

	destination.close();
	return true;
}

bool Hashtable::load() {
	std::fstream source;
	source.open("./../save/save.csv");
	if(!source.is_open()) {
		return false;
	}
	// clear tables
	delete[] m_table;
	m_table = new Stock[2003];
	delete[] m_dictionary;
	m_dictionary = new Entry[2003];

	bool dic_start = false;
	std::string line;
	while(std::getline(source, line)) {
		std::string line_value;
		std::vector<std::string> line_values;
		std::stringstream ss(line);
		while(std::getline(ss, line_value, ',')) {
			line_values.push_back(line_value);
		}
		if(line_values[0] == "START_DICTIONARY") {
			dic_start = true;
			continue;
		}
		short int index = stoi(line_values[0]);
		if(!dic_start) {
			if(line_values.size() == 1) {
				m_table[index].deleted = true;
				continue;
			}
			m_table[index].filled = true;
			m_table[index].name = line_values[1];
			m_table[index].isin = stoi(line_values[2]);
			m_table[index].abbreviation = line_values[3];
			if(line_values.size() > 4) {
				for(std::vector<std::string>::iterator it = std::next(line_values.begin(), 4); it != line_values.end(); it = it + 7) {
					Data data(*it, std::stod(*(it + 1)), std::stod(*(it + 2)), std::stod(*(it + 3)), std::stod(*(it + 4)), std::stod(*(it + 5)), std::stoull(*(it + 6)));
					m_table[index].history.emplace_back(data);
				}
			}
		}
		else {
			if(line_values.size() > 1) {
				m_dictionary[index].m_name = line_values[1];
				m_dictionary[index].m_indexStock = stoi(line_values[2]);
			}
			else {
				m_dictionary[index].m_deleted = true;
			}
		}
	}
	source.close();
	return true;
}

void Hashtable::searchEntry(int& indexEntry, std::string input, uint32_t qu_pr) {
	uint32_t index_tmp = (indexEntry + qu_pr * qu_pr) % 2003;
	if((m_dictionary[index_tmp].m_name != input && !m_dictionary[index_tmp].m_name.empty()) || m_dictionary[index_tmp].m_deleted) {
		searchEntry(indexEntry, input, qu_pr + 1);
	}
	else if (m_dictionary[index_tmp].m_name == input) {
		indexEntry = index_tmp;
	}
	else {
		indexEntry = -1;
	}
};

void Hashtable::searchStock(int& indexStock, std::string input, uint32_t qu_pr) {
	uint32_t index_tmp = (indexStock + qu_pr * qu_pr) % 2003;
	if((m_table[index_tmp].abbreviation != input && !m_table[index_tmp].abbreviation.empty()) || m_table[index_tmp].deleted) {
		searchStock(indexStock, input, qu_pr + 1);
	}
	else if (m_table[index_tmp].abbreviation == input) {
		indexStock = index_tmp;
	}
	else {
		indexStock = -1;
	}
};

void Hashtable::printStock(int index) {
	std::cout << "Index: " << index << std::endl;
	std::cout << "Name: " << m_table[index].name << std::endl;
	std::cout << "ISIN: " << m_table[index].isin << std::endl;
	std::cout << "Abbreviation: " << m_table[index].abbreviation << std::endl;
	if(!m_table[index].history.empty()) {
		std::cout << "Date: " << m_table[index].history[29].m_date << std::endl;
		std::cout << "Open: " << m_table[index].history[29].m_open << std::endl;
		std::cout << "High: " << m_table[index].history[29].m_high << std::endl;
		std::cout << "Low: " << m_table[index].history[29].m_low << std::endl;
		std::cout << "Close: " << m_table[index].history[29].m_close << std::endl;
		std::cout << "Volume: " << m_table[index].history[29].m_volume << std::endl;
		std::cout << "Adj_Close: " << m_table[index].history[29].m_adjclose << std::endl;
	}
};

void::Hashtable::deleteStock(int index) {
	int dic_index = hash(m_table[index].name);
	searchEntry(dic_index, m_table[index].name, 0);
	m_dictionary[dic_index].m_indexStock = -1;
	m_dictionary[dic_index].m_name.clear();
	m_dictionary[dic_index].m_deleted = true;
	m_table[index].name.clear();
	m_table[index].abbreviation.clear();
	m_table[index].isin = 0;
	m_table[index].filled = false;
	m_table[index].deleted = true;
	m_table[index].history.clear();
	std::cout << "Stock has been deleted!" << std::endl;
};