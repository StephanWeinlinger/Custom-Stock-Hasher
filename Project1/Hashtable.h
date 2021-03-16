#pragma once
#include <string>
#include <vector>

typedef struct Date { // string would probably be better
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
		: filled(false), deleted(false), isin(0) {}
	bool filled;
	bool deleted;
	std::string name;
	int isin;
	std::string abbreviation;
	std::vector<Data> History;
} Stock;

class Hashtable {
public:
	Hashtable();
	~Hashtable();
	Stock* m_table; // atleast 2000, 2003 is first prime number over 2000
	int hash(std::string abbreviation);
	void add(uint32_t index, Stock stock, uint32_t qu_pr);
};