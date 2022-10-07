#ifndef _EDGE_H
#define _EDGE_H

#include <iostream>
#include <string>


class Edge {
private:
	int nodeId;
	int from;
	int to;
public:
	Edge(int nodeId, int from, int to);
	int getChild() const;
	int getParent() const;
	int getNodeId() const;
	void setChild(const int to);
	bool isEqual(int from, int to) const;
	void print() const;
};

#endif

