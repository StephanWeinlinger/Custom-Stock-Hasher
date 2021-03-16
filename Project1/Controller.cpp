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
				//import();
				break;
			case 4:
				//search();
				break;
			case 5:
				//plot();
				break;
			case 6:
				//save();
				break;
			case 7:
				//load();
				break;
			case 8:
				m_quit = true;
				break;
		}
	}
}

void Controller::add() {
	Stock stock;
	std::cout << "Name: " << std::endl;
	std::cin >> stock.name;
	std::cout << "ISIN: " << std::endl;
	std::cin >> stock.isin;
	std::cout << "Abbreviation: " << std::endl;
	std::cin >> stock.abbreviation;
	stock.filled = true;
	int hash = m_hashtable.hash(stock.abbreviation);
	m_hashtable.add(hash, stock);
}
