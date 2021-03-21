#pragma once
#include "Hashtable.h"

class Controller {
public:
	Controller();
private:
	Hashtable m_hashtable;
	void run();
	void printMenu();
	void add();
	uint32_t decision();
	void import();
	void save();
	void load();
	void plot();
	void search();
	void deleteStock();
};