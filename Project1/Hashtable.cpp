#include "Hashtable.h"

Hashtable::Hashtable() {
	m_table = new Stock[2003];
}

Hashtable::~Hashtable() {
	delete[] m_table;
}