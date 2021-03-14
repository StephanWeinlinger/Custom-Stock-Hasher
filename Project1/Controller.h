#pragma once
#include "Hashtable.h"

class Controller {
public:
	Controller();
	void run();
	void printMenu();
private:
	Hashtable m_hashtable;
	bool m_quit;
};