#include "hierarchy.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "Inentryid arguments. "
			"Use ./incognito [data directory] [k]"
		<< endl;
		return -1;
	}

	// Read csv data file
	string headers;
	vector<vector<string>> dataset;
	vector<vector<string>> hierarchies_set;
	read_directory(fs::path(argv[1]), dataset, hierarchies_set, headers);


	return 0;
}
