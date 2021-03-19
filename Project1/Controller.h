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
	void import();
	void save();
	void load();
};