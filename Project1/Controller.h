#pragma once
#include "Hashtable.h"

class Controller {
public:
	Controller();
private:
	Hashtable m_hashtable;
	bool m_quit;
	void run();
	void printMenu();
	void add();
	int decision();
	void import();
	void save();
	void load();
	void plot();
	void search();
	void delete_stock();
};