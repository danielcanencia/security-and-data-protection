#ifndef _KMEANS_H
#define _KMEANS_H

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
	Record(int ridx);
	void push_back_value(double value);
	void set_group(int gidx);
	int get_gindex() const;
	int size();
	double at(int idx) const;
	double euclidean_distance(const vector<double> tuple) const;
	vector<double> getValues();
	void write_to_file(ofstream& file) const;
	void print_values() const;
};


class Group {
private:
	const int gindex;
	vector<double> centroid;
	vector<Record> records;
public:
	Group(int gidx, Record& _centroid);
	int get_index();
	vector<double> get_centroid() const;
	void insert_centroid(Record& centroid);
	void add_record(Record& record);
	void remove_record(Record& record);
	void recalculate_centroid();
	void write_to_file(ofstream& file) const;
	void print_records();
};

class Kmeans {
private:
	int K;
 	vector<Group> inicialize_centroids(vector<Record>& records);
	vector<double> centroids_distances(const vector<Group>& groups,
					  	   const vector<Record>& records);
	bool update_groups (vector<Group>& groups,
	 	           const vector<double>& newGroups,
	     	           vector<Record>& curRecords);
public:
	Kmeans(int K);
	vector<Group> compute_all(vector<Record> &records);
	void write_output(vector<Group> groups, string filename, string headers);
};

vector<Record> preprocessing(string file, string &headers);

#endif
