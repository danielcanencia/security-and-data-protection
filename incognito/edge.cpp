#include "edge.h"

Edge::Edge(int from, int to) {
  this->from = from;
  this->to = to;
}

int Edge::getParent() const { return this->from; }

int Edge::getChild() const { return this->to; }

void Edge::setChild(const int to) { this->to = to; }

bool Edge::isEqual(int from, int to) const {
  if (this->from == from && this->to == to)
    return true;
  return false;
}

void Edge::print() const {
  std::cout << "from: " + std::to_string(from) +
                   ", "
                   "to: " +
                   std::to_string(to)
            << std::endl;
}
