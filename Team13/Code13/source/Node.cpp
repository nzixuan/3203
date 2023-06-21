#include "Node.h"

Node::Node(NodeType nodeType, int id) : type(nodeType), id(id) {}

void Node::addNeighbour(NodePtr n) { neighbours.push_back(n); }

NodeType Node::getNodeType() { return type; }

std::vector<NodePtr> Node::getNeighbours() { return neighbours; }

int Node::getId() { return id; }
