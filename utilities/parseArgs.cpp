#include "parseArgs.h"
#include <iostream>
vector<string> parseArgQids(const char* arg) {
	vector<string> qids;

	char* buff = (char*)str.substr(1, str.size()-2).c_str();
	std::cout << arg << std::endl;

	char *ptr;
	ptr = strtok(buff, ",");
	while (ptr != NULL) {
		qids.emplace_back(ptr);
		ptr = strtok(NULL, ",");
	}

	return qids;
}


