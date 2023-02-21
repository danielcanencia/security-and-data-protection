#include "group.h"

Group::Group(int gidx, Record& _centroid) : gindex(gidx) {
	this->insertCentroid(_centroid);
}

int Group::getIndex() { return gindex; }
vector<double> Group::getCentroid() const { return centroid; }
void Group::insertCentroid(Record& centroid) {
	this->centroid = centroid.getValues();
	centroid.setGroup(gindex);
	records.emplace_back(centroid);
}

void Group::addRecord(Record& record) {
	records.emplace_back(record);
}
void Group::removeRecord(Record& record) {
	records.erase(records.begin() + 1);
}

void Group::recalculateCentroid() {
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

void Group::writeToFile(ofstream& file) const {
	for (const Record& record : records) {
		file << "Cluster: " + to_string(gindex);
		file << ", Atts.Values: ";
		record.writeToFile(file);
	}

}

void Group::printRecords() {
	for(const Record& record : records) {
		record.printValues();
	}
}
