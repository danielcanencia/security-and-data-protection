#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;

class Record {
private:
	const int rindex;
	int gindex;
	vector<string> values;

public:
	Record(int ridx) : rindex(ridx) { }

	void push_back_value(string value) {
		values.emplace_back(value);
	}

	void setGroup(int gidx) {
		gindex = gidx;
	}


	int size() {
		 return values.size();
	}
	string at(int idx) {
		return values[idx];
	}
	void print_value(int i) const {
		cout << values.at(i) << endl;
	}
	void print_values() const {
		for(const string& val : values) {
			cout << val << endl;
		}
	}
};


class Group {
private:
	int gindex;
	vector<string> centroid;
	vector<Record> records;
public:
 	using iterator = vector<Record>::iterator;
    	using const_iterator = vector<Record>::const_iterator;

 	iterator begin() { return records.begin(); }	
 	iterator end() { return records.end(); }	
	const_iterator begin() const {
		return records.begin(); 
	}
	const_iterator end() const {
		return records.end();
	}

	Group(int gidx, Record centroid) : gindex(gidx) {
		this->insertRecord(centroid);
	
	}

	void insertRecord(Record record) {
		record.setGroup(gindex);
		records.emplace_back(record);
	}

	void print_records() {
		for(const Record& record : records) {
			record.print_values();
		}
	}
};

class Kmeans {
private:
	int k;
	string output_file;

public:
	Kmeans(int k, string output_file) {
		this->k = k;
		this->output_file = output_file;
	}
	void compute_all(vector<Record> &values) {
		cout << output_file << endl;
		return ;
	}

};


vector<Record> preprocessing(string file) {

	// Read CSV Input files
	ifstream input{file};
	if(!input.is_open()) {
		throw "Error reading file";
		//return vector<vector<string>>();
	}
	// Parse CSV Input file
	vector<Record> values;
	int index = 0;
	for(string line; getline(input, line);) {
		Record record = Record(index);
		istringstream strm(move(line));

		for(string val; getline(strm, val, ',');) {
			record.push_back_value(val);
		}
		values.push_back(move(record));
		index++;
	}

  	// Print out our table
 	values[4].setGroup(1);
	Group cluster(1, values[4]);
	// Recorrer cluster
	for (const Record& record : cluster) {
		record.print_value(0);
	}
	return values;
}

/*
vector<vector<string>> preprocessing(string file) {

	// Read CSV Input files
	ifstream input{file};
	if(!input.is_open()) {
		throw "Error reading file";
	}
	// Parse CSV Input file
	vector<vector<string>> values;
	for(string line; getline(input, line);) {
		vector<string> record;
		istringstream strm(move(line));

		for(string val; getline(strm, val, ',');) {
			record.emplace_back(val);
		}
		values.push_back(move(record));
	}

  	// Print out our table
  	for (const vector<string>& record : values) {
   		for (const string& value : record) {
     	 		cout << setw(10) << value;
    		}
    		cout << "\n";
	}

	return values;
}
*/

int main(int argc, char** argv) {
	/*Record x(2);
   	x.push_back_value("99");
   	x.push_back_value("22.222");
	for(size_t i = 0; i < 2; ++i)
  	{
     		x.print_value(i);	
    	}
	x.print_values();*/

  	// Arguments	
	if (argc != 3) {
		cout << "Invalid arguments. Use ./kmeans [filename] [output_filename] [k]" << endl;
		return -1;
	}
	// Input Filename
	string file{argv[1]};
	// File format
	strtok(argv[1], ".");
	string ptr = strtok(NULL, ".");
	if (ptr != "csv") {
		cout << "Use a csv input file" << endl;
		return -1;
	}
	// Output Filename
	//string output_file = argv[2]; 
	
	// k
	int k = atoi(argv[2]);


	// Data Preprocessing
	try {
		/*vector<vector<string>> values = preprocessing(file);*/
		vector<Record> values = preprocessing(file);
		file.insert(file.length()-4, "_out");
		cout << file << endl;	
		Kmeans kmeans(k, file);
		kmeans.compute_all(values);	
	}
	catch(exception& e) {
		cout << "Error reading file" << endl;
		return -1;
	}

	return 0;
}
