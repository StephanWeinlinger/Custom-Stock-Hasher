#pragma once
#include <string>
#include <vector>

typedef struct Entry {
	Entry() // only used for initial allocation
	: m_deleted(false) {}
	bool m_deleted;
	std::string m_name;
	std::string m_abbreviation;
} Entry;

typedef struct Data {
	Data(std::string date, double open, double high, double low, double close, double adjclose, unsigned long long int volume)
		: m_date(date), m_open(open), m_high(high), m_low(low), m_close(close), m_adjclose(adjclose), m_volume(volume) {}
	std::string m_date;
	double m_open;
	double m_high;
	double m_low;
	double m_close;
	double m_adjclose;
	unsigned long long int m_volume; // could be reduced, is pretty big
} Data;

typedef struct Stock {
	Stock()
		: filled(false), deleted(false), isin(0) {}
	bool filled;
	bool deleted;
	std::string name;
	int isin;
	std::string abbreviation;
	std::vector<Data> history;
} Stock;

class Hashtable {
public:
	Hashtable();
	~Hashtable();
	Stock* m_table; // atleast 2000, 2003 is first prime number over 2000
	Entry* m_dictionary; 
	int hash(std::string abbreviation);
	void addStock(uint32_t index, Stock stock, uint32_t qu_pr);
	void addEntry(uint32_t index, Stock stock, uint32_t qu_pr);
	void import(int index);
	bool save();
	bool load();
};