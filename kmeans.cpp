#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <memory>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <cmath>
#include<numeric>

using namespace std;
namespace fs = std::filesystem;


class Record {
private:
	int rindex;
	int gindex = -1;
	vector<double> values;

public:
	Record(int ridx) {
		rindex = ridx;
	} 

	void push_back_value(double value) {
		values.emplace_back(value);
	}

	void setGroup(int gidx) { gindex = gidx; }
	int getGindex() const { return gindex; }
	int getIndex() { return rindex; }
	bool compareIdx(Record record) { return rindex == record.rindex; }

	int size() { return values.size(); }
	double at(int idx) const { return values[idx]; }
	double euclidean_distance(const vector<double> tuple) const {
		int sum = 0;
		for (int i=0; i < min(tuple.size(), values.size()); i++)
			sum += pow((double)(values[i]) - (double)(tuple[i]), 2);
		return sqrt(sum);
	}
	vector<double> getValues() { return values; }
	double valueAvg() const {
		return reduce(values.begin(), values.end(), 0.0) / values.size();
	}

	void writeToFile(ofstream& file) const {
		for (auto it = begin(values); it != end(values)-1; it++) {
			file << to_string(*it) + ",";
		}
		file << to_string(values.back()) << endl;
	}

	void print_value(int i) const {
		cout << values.at(i) << endl;
	}
	void print_values() const {
		for(const double& val : values) {
			cout << to_string(val) + ", ";
		}
		cout << '\n';
	}
};


class Group {
private:
	const int gindex;
	vector<double> centroid;
	vector<Record> records;
public:
	Group(int gidx, Record& _centroid) : gindex(gidx) {
		this->insertCentroid(_centroid);
	}

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


	vector<double> getCentroid() const { return centroid; }
	void insertCentroid(Record& centroid) {
		this->centroid = centroid.getValues();
		centroid.setGroup(gindex);
		records.emplace_back(centroid);
	}
	void addRecord(Record& record) {
		records.emplace_back(record);
	}
	void removeRecord(Record record) {
		records.erase(records.begin() + 1);
	}

	void recalculateCentroid() {
		vector<double> centroid;
		for (const Record& record : records) {
			centroid.emplace_back(record.valueAvg());
		}
		this->centroid = centroid;
	}

	void writeToFile(ofstream& file) const {
		for (const Record& record : records) {
			record.writeToFile(file);
		}

	}

	void print_records() {
		for(const Record& record : records) {
			record.print_values();
		}
	}


};

class Kmeans {
private:
	int K;

 	vector<Group> inicialize_centroids(vector<Record>& records) {
		vector<Group> groups;
		vector<int> aux;
		for(int i=0; i < K; i++) {
			while (1) {
				int random = rand() % records.size();
				// Check if random value is repeated
				if (find(aux.begin(), aux.end(), random) == aux.end()) {
					// Inicialize centroids and assign them a cluster
			 		aux.emplace_back(random);
					records.at(random).setGroup(i);
					Group group(i, records.at(random));
					groups.emplace_back(group);
					break;
				}
			}
		}
		return groups;
	}
	
	vector<double> centroids_distances(const vector<Group>& groups,
					  	   const vector<Record>& records) {
		vector<double> res;
		int gindex;
		double min, ed;
		vector<vector<double>> centroids;
		for (const Group& group : groups) {
			centroids.emplace_back(group.getCentroid());
		}
	
		// Calculate euclidian distance between records and all centroids
		for(const Record& record : records) {
			min = record.euclidean_distance(centroids[0]);
			gindex = 0;

			for(int i=1; i < centroids.size(); i++) {
				ed = record.euclidean_distance(centroids[i]);
				if (ed < min) {
					min = ed;
					gindex = i;
				}
			}
			// Array Representing Records & its Clusters
			res.emplace_back(gindex);
			
		}

		return res;
	}

	bool updateGroups (vector<Group>& groups,
	 	           const vector<double>& newGroups,
	     	           vector<Record>& curRecords) {
		bool end = 1;
		int newGroup, curGroup;
		for (int i=0; i < newGroups.size(); i++) {
			newGroup = newGroups[i];
			curGroup = curRecords[i].getGindex();
			if (newGroup != curGroup) {
				if (curGroup != -1) {
					groups[curGroup].removeRecord(curRecords[i]);
				}
				curRecords[i].setGroup(newGroup);
				groups[newGroup].addRecord(curRecords[i]);
				end = 0;
			}
		}	
		return end;
	}


public:
	Kmeans(int K) {
		this->K = K;
	}
	vector<Group> computeAll(vector<Record> &records) {

		// 1. Inicialize centroids
		vector<Group> groups = inicialize_centroids(records);

		while (1) {	
			// 2. Euclidean Distance And Group Classification
			vector<double> newGroups = centroids_distances(groups, records);
			// 3. Update all record's group
			bool end = updateGroups(groups, newGroups, records);
			if (end) break;

			// 4. Recalculate the centroid for each group/cluster
			for(Group& group : groups) {
				group.recalculateCentroid();
			}
		}
		return groups;
	}

	void writeOutput(vector<Group> groups, string filename, string headers) {
		ofstream file;
		file.open(filename, ios::trunc);

  		if (file.is_open())
  		{
   			file << headers << endl;
			for (const Group& group : groups) {
				group.writeToFile(file);
			}
   	 		file.close();
  		}
  		else cout << "Unable to open file" << endl;
	}
};





vector<Record> preprocessing(string file, string &headers) {

	// Read CSV Input files
	ifstream input{file};
	if(!input.is_open()) {
		throw "Error reading file";
	}

	// Parse CSV Input file
	vector<Record> values;
	int index = 0;
	string line;	
	try {
		getline(input, headers);
		for(line; getline(input, line);) {
			Record record = Record(index);
			istringstream strm(move(line));

			for(string val; getline(strm, val, ',');) {
				//cout << stod(val) << endl;
				record.push_back_value(stod(val));
			}
			values.push_back(move(record));
			index++;
		}
	}
	catch (exception e) {
		input.close();
		// It throws an expetion because of stod(val) conversion to double
		return values;
	}
	input.close();
	return values;

}




int main(int argc, char** argv) {

  	// Arguments	
	if (argc != 3) {
		cout << "Invalid arguments. Use ./kmeans [filename] [output_filename] [k]" << endl;
		return -1;
	}
	// Input Filename
	string filename{argv[1]};
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
	vector<Record> records;
	string headers;

	try {
		records = preprocessing(filename, headers);
	}
	catch(exception& e) {
		cout << "Error reading file" << endl;
		return -1;
	}
	
	cout << "* K-Means algorithm is being run..." << endl;
	// Especify the number of clusters/groups to use
	Kmeans kmeans(k);
	// Run the algorithm
	vector<Group> groups = kmeans.computeAll(records);
	cout << "* K-Means algorithm finished. A csv file will be generated...." << endl;

	// Write resulting groups to file
	filename.insert(filename.length()-4, "_out");
	kmeans.writeOutput(groups, filename, headers);
	cout << "* Filename: " + filename << endl;

	return 0;
}
