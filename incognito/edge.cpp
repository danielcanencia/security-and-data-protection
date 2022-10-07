#include "edge.h"

Edge::Edge(int nodeId, int from, int to) {
	this->nodeId = nodeId;
	this->from = from;
	this->to = to;
}

int Edge::getParent() const {
	return this->from;
}

int Edge::getChild() const {
	return this->to;
}

int Edge::getNodeId() const {
	return this->nodeId;
}

void Edge::setChild(const int to) {
	this->to = to;
}

bool Edge::isEqual(int from, int to) const {
	if (this->from == from &&
	    this->to == to) 
		return true;
	return false;
}

void Edge::print() const {
	std::cout << "nodeId: " + std::to_string(nodeId) +
		", from: " + std::to_string(from) + ", "
		"to: " + std::to_string(to) << std::endl;
}

