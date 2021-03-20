#include <iostream>
#include "Controller.h"

Controller::Controller()
	: m_quit(false) {
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

void Controller::run() {
	while(!m_quit) {
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
				m_quit = true;
				break;
		}
	}
}

void Controller::add() { // deleted gets set to false (constructor)
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
	int indexStock = m_hashtable.hash(stock.abbreviation);
	int indexEntry = m_hashtable.hash(stock.name);
	m_hashtable.addStock(indexStock, stock, 0);
	m_hashtable.addEntry(indexEntry, indexStock, stock, 0);
}

int Controller::decision() {
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
		int indexEntry = m_hashtable.hash(input);
		m_hashtable.searchEntry(indexEntry, input, 0); // update if value isn't correct
		if(indexEntry == -1) {
			return indexEntry; // returns -1 if not found
		}
		return m_hashtable.m_dictionary[indexEntry].m_indexStock;
	}
	int indexStock = m_hashtable.hash(input);
	m_hashtable.searchStock(indexStock, input, 0); // returns -1 if not found
	return indexStock;
}

void Controller::import() {
	int index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else if(m_hashtable.import(index)) {
		std::cout << "Stock data imported!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't import stock data (maybe file is opened or not existent)" << std::endl;
	}
}

void Controller::save() {
	std::cout << "Saving in process..." << std::endl;
	if(m_hashtable.save()) {
		std::cout << "Saving completed! Hashtable and dictionary files created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't save hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}

void Controller::load() {
	std::cout << "Loading in process..." << std::endl;
	if(m_hashtable.load()) {
		std::cout << "Loading completed! Hashtable and dictionary created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't load hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}

void Controller::plot() {
	int index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else {
		m_hashtable.plot(index);
	}
}

void Controller::search() {
	int index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else {
		m_hashtable.printStock(index);
	}
};

void Controller::deleteStock() {
	int index = decision();
	if(index == -1) {
		std::cout << "Stock not found!" << std::endl;
	}
	else {
		m_hashtable.deleteStock(index);
	}
}