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
				//del();
				break;
			case 3:
				import();
				break;
			case 4:
				//search();
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
	m_hashtable.addEntry(indexEntry, stock, 0);
	std::cout << stock.name << std::endl;
}

int Controller::decision() {
	short int type;
	char tmp[40];
	std::string input;
	std::cout << "Abbreviation or Name [0 or 1]: ";
	std::cin >> type;
	if(type == 0) {
		std::cout << "Abbreviation: ";
	}
	else {
		std::cout << "Name (max. 40 characters): ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.getline(tmp, 40);
		input = tmp;
	}
	std::cin >> input;
	if(type == 1) {
		//int indexEntry = m_hashtable.hash(input);
		//input = m_hashtable.m_dictionary[m_hashtable.searchEntry(indexEntry, input)];
	} // add searchEntry and searchStock
	//int indexStock = m_hashtable.hash(input);
	//return m_hashtable.searchStock(indexStock, input);
	return 0;
}

void Controller::import() {
	int index = decision();
	m_hashtable.import(999); // replace with index
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
	int index = 999;

}