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
				//plot();
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
	std::cout << "Name: ";
	std::cin >> stock.name;
	std::cout << "ISIN: ";
	std::cin >> stock.isin;
	std::cout << "Abbreviation: ";
	std::cin >> stock.abbreviation;
	stock.filled = true;
	int index = m_hashtable.hash(stock.abbreviation);
	m_hashtable.add(index, stock, 0);
}

void Controller::import() {
	std::string abbreviation;
	std::cout << "Abbreviation: ";
	std::cin >> abbreviation;
	//int index = m_hashtable.search(abbreviation)
	m_hashtable.import(999); // replace with index
}

void Controller::save() {
	std::cout << "Saving in process..." << std::endl;
	if(m_hashtable.save()) {
		std::cout << "Saving completed! Hashtable and dictonary files created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't save hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}

void Controller::load() {
	std::cout << "Loading in process..." << std::endl;
	if(m_hashtable.load()) {
		std::cout << "Loading completed! Hashtable and dictonary created!" << std::endl;
	}
	else {
		std::cout << "Error: Couldn't load hashtable or dictionary (maybe file is opened)" << std::endl;
	}
}