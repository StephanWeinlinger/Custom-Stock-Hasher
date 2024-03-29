#include <iostream>
#include "Controller.h"

Controller::Controller() {
	run();
}

void Controller::printMenu() {
	std::cout << "[1] ADD" << std::endl;
	std::cout << "[2] DELETE" << std::endl;
	std::cout << "[3] IMPORT" << std::endl;
	std::cout << "[4] SEARCH" << std::endl;
	std::cout << "[5] PLOT" << std::endl;
	std::cout << "[6] SAVE" << std::endl;
	std::cout << "[7] LOAD" << std::endl;
	std::cout << "[8] QUIT" << std::endl;
}

// main menu loop
void Controller::run() {
	bool quit = false;
	while(!quit) {
		printMenu();
		short int input;
		std::cin >> input;
		switch(input) {
			case 1:
				add();
				break;
			case 2:
				deleteStock();
				break;
			case 3:
				import();
				break;
			case 4:
				search();
				break;
			case 5:
				plot();
				break;
			case 6:
				save();
				break;
			case 7:
				load();
				break;
			case 8:
				quit = true;
				break;
		}
	}
}

// adds new stock to hashtable and new entry to dictionary
// if fill grade of hashtable would exceed 50% it stops the insertion
// if name or abbreviation is already existent it stops the insertion
void Controller::add() {
	if(m_hashtable.m_amount < 1001) {
		Stock stock;
		char tmp[41];
		std::cout << "Name (max. 40 characters): ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // everything until newline gets discarded
		std::cin.getline(tmp, 40);
		stock.name = tmp;
		std::cout << "ISIN: ";
		std::cin >> stock.isin;
		std::cout << "Abbreviation: ";
		std::cin >> stock.abbreviation;
		stock.filled = true;
		uint32_t indexStock = m_hashtable.hash(stock.abbreviation);
		uint32_t indexEntry = m_hashtable.hash(stock.name);
		m_hashtable.addStock(indexStock, stock, 0); // sets indexStock to actual index (if probing happens) or to -1 if abbreviation is already used
		if(indexStock != -1) {
			m_hashtable.addEntry(indexEntry, indexStock, stock, 0);
		}
		else {
			std::cout << "Stock with same abbreviation aready exists!" << std::endl;
		}
	}
	else {
		std::cout << "Hashtable is full!" << std::endl;
	}
}

// returns index of stock or -1 if stock wasn't found
// user can decide between searching by name or abbreviation
uint32_t Controller::decision() {
	short int type;
	char tmp[41];
	std::string input;
	std::cout << "Abbreviation or Name [0 or 1]: ";
	std::cin >> type;
	if (type == 0) {
		std::cout << "Abbreviation: ";
		std::cin >> input;
	}
	else {
		std::cout << "Name (max. 40 characters): ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // everything until newline gets discarded
		std::cin.getline(tmp, 40);
		input = tmp;
	}
	if (type == 1) {
		uint32_t indexEntry = m_hashtable.hash(input);
		m_hashtable.searchEntry(indexEntry, input, 0); // sets indexEntry to actual value (if probing happens) or to -1 if name not found
		if(indexEntry == -1) {
			return indexEntry; // returns -1 if not found
		}
		return m_hashtable.m_dictionary[indexEntry].m_indexStock; // returns index in hashtable
	}
	uint32_t indexStock = m_hashtable.hash(input);
	m_hashtable.searchStock(indexStock, input, 0); // sets indexStock to actual value (if probing happens) or to -1 if name not found
	return indexStock;
}

// handles import
void Controller::import() {
	uint32_t index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else if(m_hashtable.import(index)) {
		std::cout << "Stock data imported!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't import stock data (maybe file is opened, not existent or contains less than 30 entries)" << std::endl;
	}
}

// handles saving
void Controller::save() {
	std::cout << "Saving in process..." << std::endl;
	if(m_hashtable.save()) {
		std::cout << "Saving completed! Hashtable and dictionary files created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't save hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}

// handles loading
void Controller::load() {
	std::cout << "Loading in process..." << std::endl;
	if(m_hashtable.load()) {
		std::cout << "Loading completed! Hashtable and dictionary created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't load hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}

// handles plotting
void Controller::plot() {
	uint32_t index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else if(m_hashtable.m_table[index].history.empty()) {
		std::cout << "Stock doesn't have data!" << std::endl;
	}
	else {
		m_hashtable.plot(index);
	}
}

// handles searching
void Controller::search() {
	uint32_t index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else {
		m_hashtable.printStock(index);
	}
};

// handles deleting
void Controller::deleteStock() {
	uint32_t index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else {
		m_hashtable.deleteStock(index);
	}
}