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
#include <numeric>
#include <ctime>

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

	void set_group(int gidx) { gindex = gidx; }
	int get_gindex() const { return gindex; }

	int size() { return values.size(); }
	double at(int idx) const { return values[idx]; }
	double euclidean_distance(const vector<double> tuple) const {
		int sum = 0;
		for (int i=0; i < min(tuple.size(), values.size()); i++)
			sum += pow((double)(values[i]) - (double)(tuple[i]), 2);
		return sqrt(sum);
	}
	vector<double> getValues() { return values; }

	void write_to_file(ofstream& file) const {
		for (auto it = begin(values); it != end(values)-1; it++) {
			file << to_string(*it) + ",";
		}
		file << to_string(values.back()) << endl;
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
		this->insert_centroid(_centroid);
	}

	int get_index() { return gindex; }
	vector<double> get_centroid() const { return centroid; }
	void insert_centroid(Record& centroid) {
		this->centroid = centroid.getValues();
		centroid.set_group(gindex);
		records.emplace_back(centroid);
	}
	void add_record(Record& record) {
		records.emplace_back(record);
	}
	void remove_record(Record& record) {
		records.erase(records.begin() + 1);
	}

	void recalculate_centroid() {
		int atts = this->records[0].size();
		vector<double> centroid(atts, 0.0);
		for(Record& record : this->records) {
			for (int j=0; j < atts; j++) {
				centroid[j] += record.at(j);
			}
		}
		for(int j=0; j < atts; j++)
			centroid[j] /= records.size();

		this->centroid = centroid;
	}

	void write_to_file(ofstream& file) const {
		for (const Record& record : records) {
			file << "Cluster: " + to_string(gindex);
			file << ", Atts.Values: ";
			record.write_to_file(file);
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

		srand(time(NULL));

		for(int i=0; i < K; i++) {
			while (1) {
				int random = rand() % records.size();
				cout << random << endl;
				// Check if random value is repeated
				if (find(aux.begin(), aux.end(), random) == aux.end()) {
					// Inicialize centroids and assign them a cluster
			 		aux.emplace_back(random);
					records.at(random).set_group(i);
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
			centroids.emplace_back(group.get_centroid());
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

	bool update_groups (vector<Group>& groups,
	 	           const vector<double>& newGroups,
	     	           vector<Record>& curRecords) {
		bool end = 1;
		int newGroup, curGroup;
		for (int i=0; i < newGroups.size(); i++) {
			newGroup = newGroups[i];
			curGroup = curRecords[i].get_gindex();
			if (newGroup != curGroup) {
				if (curGroup != -1) {
					groups[curGroup].remove_record(curRecords[i]);
				}
				curRecords[i].set_group(newGroup);
				groups[newGroup].add_record(curRecords[i]);
				end = 0;
			}
		}	
		return end;
	}


public:
	Kmeans(int K) {
		this->K = K;
	}
	vector<Group> compute_all(vector<Record> &records) {

		// 1. Inicialize centroids
		vector<Group> groups = inicialize_centroids(records);

		// Loop
		while (1) {	
			// 2. Euclidean Distance And Group Classification
			vector<double> newGroups = centroids_distances(groups, records);
			// 3. Update all record's group
			bool end = update_groups(groups, newGroups, records);
			if (end) break;

			// 4. Recalculate the centroid for each group/cluster
			for(Group& group : groups) {
				group.recalculate_centroid();
			}
			/*for(Group& group : groups) {
				cout << "////////////////" << endl;
				cout << "GIndex: " + to_string(group.get_index()) << endl;
				cout << "---------------" << endl;
				for(double& x: group.get_centroid()) {
					cout << to_string(x) << endl;
				}
				cout << "----- Vals-----" << endl;
				group.print_records();
			}*/
		}
		return groups;
	}

	void write_output(vector<Group> groups, string filename, string headers) {
		ofstream file;
		file.open(filename, ios::trunc);

  		if (file.is_open())
  		{
   			file << headers << endl;
			for (const Group& group : groups) {
				group.write_to_file(file);
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

			for(string val; getline(strm, val, ';');) {
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
		cout << "Invalid arguments. Use ./kmeans [filename] [k]" << endl;
		return -1;
	}
	// Input Filename
	string filename{argv[1]};
	// Check File format
	if (filename.substr(filename.size()-4, filename.size()) != ".csv") {
		cout << "Please, use a csv input file" << endl;
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
	vector<Group> groups = kmeans.compute_all(records);
	cout << "* K-Means algorithm finished. A csv file will be generated...." << endl;

	// Write resulting groups to file
	filename = filename.substr(filename.find("/")+1, filename.size());
	filename.insert(filename.length()-4, string("_") + "k" + to_string(k) + "_out");
	filename.insert(0, "outputs/");
	fs::create_directory("outputs");
	kmeans.write_output(groups, filename, headers);
	cout << "* Filename: " + filename << endl;

	return 0;
}
