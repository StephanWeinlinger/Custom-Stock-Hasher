#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

/*
std::vector<std::vector<std::string>> readFileFromCSV() {
	std::fstream source;
	source.open("./../../ListeVonStocks.csv");

	std::string line;
	std::vector<std::vector<std::string>> data;
	while(std::getline(source, line)) {
		std::string line_value;
		std::vector<std::string> line_values;
		std::stringstream ss(line);
		while(std::getline(ss, line_value, ',')) {
			line_values.push_back(line_value);
		}
		data.emplace_back(line_values);
	}

	source.close();
	return data;
}
*/

std::vector<std::vector<std::string>> readFileFromCSV() {
	std::fstream source;
	source.open("./../../ListeVonStocks.csv");

	std::string line;
	std::vector<std::vector<std::string>> data;
	while(std::getline(source, line)) {
		std::string line_value;
		std::vector<std::string> line_values;
		std::stringstream ss(line);
		while(std::getline(ss, line_value, ',')) {
			line_values.push_back(line_value);
		}
		data.emplace_back(line_values);
	}

	source.close();
	return data;
}

int hash(std::string abbreviation) {
	
	
	return value;
}

int main() {
	std::vector<std::vector<std::string>> data = readFileFromCSV();

	std::ofstream destination("./../../ListeVonHash.csv");
	if(!destination.is_open()) {
		std::cout << "Could not open file!" << std::endl;
		return 1;
	}

	for(std::vector<std::vector<std::string>>::iterator it1 = data.begin(); it1 != data.end(); ++it1) {
		destination << hash(it1[0][0]) << "\n";
	}

	destination.close();

	/* Printer
	for(std::vector<std::vector<std::string>>::iterator it1 = data.begin(); it1 != data.end(); ++it1) {
		for(std::vector<std::string>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2) {
			std::cout << *it2 << std::endl;
		}
	}
	*/
	return 0;
}