#pragma once
#include <string>
#include <vector>

typedef struct Date {
	short int year;
	short int month;
	short int day;
} Date;

typedef struct Data {
	Date date;
	double open;
	double high;
	double low;
	double close;
	double adjclose;
	unsigned long long int volume; // could be reduced, is pretty big
} Data;

typedef struct Stock {
	Stock()
		: filled(false), deleted(false) {} // to remove error
	bool filled;
	bool deleted;
	std::string name;
	std::string wkn;
	std::string abbreviation;
	std::vector<Data> History;
} Stock;

class Hashtable {
public:
	Hashtable();
	~Hashtable();
	Stock* m_table; // atleast 2000, 2003 is first prime number over 2000
};