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
	//Record() {}
 	/*	
	using iterator = vector<double>::iterator;
    	using const_iterator = vector<double>::const_iterator;

 	iterator begin() { return values.begin(); }	
 	iterator end() { return values.end(); } 
	const_iterator begin() const {
		return values.begin(); 
	}
	const_iterator end() const {
		return values.end();
	}*/
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
		//vector<double> tuple = record.values;
		int sum = 0;
		//record.print_values();
		//cout << values.size() << endl;	
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
		//this->centroid = _centroid;
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
		/*if (!record.compareIdx(centroid)) {
			//Record rec = record;
			//records.emplace_back(rec);
			records.emplace_back(record);
		}
		else {
			cout << "Its the centroid" << endl;
		}*/
	}
	void removeRecord(Record record) {
		//iterator it = records.begin();
		/*for (iterator it = records.begin(); it != records.end(); it++) {
			if(record.compareIdx(*it))
				records.erase(it);
		}*/
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
		//vector<Record> centroids;
		vector<Group> groups;
		vector<int> aux;
		for(int i=0; i < K; i++) {
			while (1) {
				int random = rand() % records.size();
				// Check if random value is repeated
				if (find(aux.begin(), aux.end(), random) == aux.end()) {
					//records.at(random).print_values();
					// Inicialize centroids and assign them a cluster
			 		aux.emplace_back(random);
					records.at(random).setGroup(i);
					Group group(i, records.at(random));
					groups.emplace_back(group);
					break;
				}
			}
		}
		//return centroids;
		return groups;
	}
	
	vector<double> centroids_distances(const vector<Group>& groups,
					  	   const vector<Record>& records) {
		vector<double> res;
		int gindex;
		double min, ed;
		vector<vector<double>> centroids;
		for (const Group& group : groups) {
			/*for (const double& y : group.getCentroid()) {
				cout << to_string(y) + "," << endl;
			}*/
			centroids.emplace_back(group.getCentroid());
		}
		/*	
		cout << "CENTROIDS" << endl;
		for (const vector<double>& rec : centroids) {
			for (const double& y : rec) {
				cout << to_string(y) + "," << endl;
			}
		}
		cout << "------------------------" << endl;*/
	
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
			/*cout << "Final: ";
			cout << min << endl;
			cout << "GID: " + to_string(gindex) << endl;
			cout << endl;*/
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
			//cout << to_string(newGroup) + "==" + to_string(curGroup) << endl;
			if (newGroup != curGroup) {
				if (curGroup != -1) {
					groups[curGroup].removeRecord(curRecords[i]);
				}
				curRecords[i].setGroup(newGroup);
				groups[newGroup].addRecord(curRecords[i]);
				//groups[newGroup].print_records();
				end = 0;
			}
		}	
		/*for(Group& group : groups) {
			cout << "---------------" << endl;
			//cout << "GIndex: " + to_string(group.getCentroid().getGindex()) << endl;
			group.print_records();
		}*/
		return end;

	}


public:
	Kmeans(int K) {
		this->K = K;
	}
	vector<Group> computeAll(vector<Record> &records) {

		// 1. Inicialize centroids
		vector<Group> groups = inicialize_centroids(records);
		//cout << "Paso 1" << endl;
		/*for(Group& group : groups) {
			for (double& val : group.getCentroid())
				cout << to_string(val) + ", " << endl;
			group.print_records();
		}
		for (Record& record : records) {
			cout << record.getGindex() << endl;
		}*/

		int i=0;
		while (1) {	
			// 2. Euclidean Distance And Group Classification
			vector<double> newGroups = centroids_distances(groups, records);
			//cout << "Paso 2" << endl;
			for (const double& x : newGroups) {
				cout << x << endl;
			}
			// 3. (de 4 records) Cur Cluster: records[newGroup index].gindex
			//                   NeW Cluster = newGroupS[newGroup index] 
			//                   Si son distintos, actualizar gindex al nuevo (records), setGroup(valor newgroup)
			// 		     y seguir iterando. end=0; Si son iguales todos los gindex id de todos los record
			//                   hemos terminado. Yay.
			//cout << "Paso 3" << endl;
			bool end = updateGroups(groups, newGroups, records);
			cout << "END? =>" + to_string(end) << endl;
			if (end) break;

			// 4. Recalculate the centroid for each group/cluster
			for(Group& group : groups) {
				group.recalculateCentroid();
			}
			for(Group& group : groups) {
				cout << "---------------" << endl;
				for (double& cen : group.getCentroid()) {
					cout << to_string(cen) + ", " << endl;	
				}
				cout << "records =>  " << endl;
				group.print_records();
			}
			i++;
			//if (i==5) break;
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
		//return vector<vector<string>>();
	}

	// Parse CSV Input file
	vector<Record> values;
	int index = 0;
	string line;	
	try {
		getline(input, headers);
		for(line; getline(input, line);) {
			Record record = Record(index);
			//Record record = Record();
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
		// It throws an expetion because of stod(val), conversion to double
		return values;
	}
	input.close();
	return values;

}




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


	vector<Record> records;
	string headers;
	// Data Preprocessing
	try {
		records = preprocessing(filename, headers);
	}
	catch(exception& e) {
		cout << "Error reading file" << endl;
		return -1;
	}
	
	cout << "* K-Means algorithm is being run...";
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
