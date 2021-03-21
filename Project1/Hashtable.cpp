#include "Hashtable.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

Hashtable::Hashtable() {
	m_table = new Stock[2003];
	m_dictionary = new Entry[2003];
	m_amount = 0;
}

Hashtable::~Hashtable() {
	delete[] m_table;
	delete[] m_dictionary;
}

// returns hash value for given string
uint32_t Hashtable::hash(std::string input) {
	uint64_t value = 1;
	for(int i = 0; i < (int) input.length(); ++i) {
		value *= (int) input[i];
	}
	return value % 2003;
}

// searches for free index and if abbreviation is already in use
// adds stock at index
void Hashtable::addStock(uint32_t& indexStock, Stock stock, uint32_t qu_pr) {
	uint32_t index_tmp = (indexStock + qu_pr * qu_pr) % 2003;
	if(m_table[index_tmp].abbreviation == stock.abbreviation) {
		indexStock = -1;
	}
	else if(!m_table[index_tmp].filled) { // if not filled
		m_table[index_tmp] = stock;
		indexStock = index_tmp;
		m_amount++;
	}
	else {
		addStock(indexStock, stock, qu_pr + 1);
	}
}

// searches for free index and if name is already in use
// adds entry at index
// if name is already in use it clears the entry in the hashtable
void Hashtable::addEntry(uint32_t indexEntry, uint32_t indexStock, Stock stock, uint32_t qu_pr) {
	uint32_t index_tmp = (indexEntry + qu_pr * qu_pr) % 2003;
	if(m_dictionary[index_tmp].m_name == stock.name) { // if same name, stock in m_table has to be deleted (note that deleted is set to false)
		m_table[indexStock].name.clear();
		m_table[indexStock].abbreviation.clear();
		m_table[indexStock].isin = 0;
		m_table[indexStock].filled = false;
		m_table[indexStock].deleted = false;
		m_table[indexStock].history.clear();
		m_amount--;
		std::cout << "Stock with same name already exists!" << std::endl;
	}
	else if(m_dictionary[index_tmp].m_name == "") {
		m_dictionary[index_tmp].m_deleted = false;
		m_dictionary[index_tmp].m_name = stock.name;
		m_dictionary[index_tmp].m_indexStock = indexStock;
	}
	else {
		addEntry(indexEntry, indexStock, stock, qu_pr + 1);
	}
}

// imports stock data from ABBREVIATION.csv file
// csv file has to contain atleast 30 lines, otherwise error message will be displayed
bool Hashtable::import(uint32_t index) {
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
	if(line_count < 30) {
		return false;
	}
	m_table[index].history.clear(); // overwrite existing data
	for(int i = line_count - 30; i < line_count; ++i) { // fill actual history with last 30 entries
		Data data(history_tmp[i][0], std::stod(history_tmp[i][1]), std::stod(history_tmp[i][2]), std::stod(history_tmp[i][3]), std::stod(history_tmp[i][4]), std::stod(history_tmp[i][5]), std::stoull(history_tmp[i][6]));
		m_table[index].history.emplace_back(data);
	}
	source.close();
	return true;
}

// saves hashtable and dicionary in a file called save.csv
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

// loads hashtable and dictionary from a file called save.csv
bool Hashtable::load() {
	std::fstream source;
	source.open("./../save/save.csv");
	if(!source.is_open()) {
		return false;
	}
	// clear tables and reset amount
	delete[] m_table;
	m_table = new Stock[2003];
	delete[] m_dictionary;
	m_dictionary = new Entry[2003];
	m_amount = 0;

	bool dic_start = false; // gets set to true if start of dictionary is detected
	std::string line;
	while(std::getline(source, line)) {
		std::string line_value;
		std::vector<std::string> line_values;
		std::stringstream ss(line);
		while(std::getline(ss, line_value, ',')) {
			line_values.push_back(line_value);
		}
		if(line_values[0] == "START_DICTIONARY") { // check if dictionary starts
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
			m_amount++;
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

// searches for entry by given name and sets new indexEntry
// if not found indexEntry gets set to -1
void Hashtable::searchEntry(uint32_t& indexEntry, std::string input, uint32_t qu_pr) {
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

// searches for stock by given abbreviation and sets new indexStock
// if not found indexStock gets set to -1
void Hashtable::searchStock(uint32_t& indexStock, std::string input, uint32_t qu_pr) {
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

// prints stock data
void Hashtable::printStock(uint32_t index) {
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
		std::cout << "Adjusted Close: " << m_table[index].history[29].m_adjclose << std::endl;
	}
};

// searches for dictionary entry and deletes it and the stock
// deleted gets set to true
void Hashtable::deleteStock(uint32_t index) {
	uint32_t dic_index = hash(m_table[index].name);
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
	m_amount--;
	std::cout << "Stock has been deleted!" << std::endl;
};

// plots data of given stock
void Hashtable::plot(uint32_t index) {
	double sum = 0;
	double low = m_table[index].history[0].m_close;
	double high = m_table[index].history[0].m_close;
	for(std::vector<Data>::iterator it = m_table[index].history.begin(); it != m_table[index].history.end(); ++it) {
		sum += it->m_close;
		if(it->m_close < low) {
			low = it->m_close;
		}
		else if(it->m_close > high) {
			high = it->m_close;
		}
	}
	double difference = high - low;
	std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
	for(double i = 14; i >= 0; --i) {
		std::cout << "|";
		for(std::vector<Data>::iterator it = m_table[index].history.begin(); it != m_table[index].history.end(); ++it) {
			if(it->m_close > low + (difference / 15) * i && it->m_close <= low + (difference / 15) * (i + 1)) {
				std::cout << " o ";
			}
			else if(i == 0 && it->m_close == low) { // otherwise it wouldn't be included
				std::cout << " o ";
			}
			else {
				std::cout << "   ";
			}
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Low: " << low << std::endl;
	std::cout << "High: " << high << std::endl;
}