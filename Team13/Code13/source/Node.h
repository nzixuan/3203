#pragma once
#include <memory>
#include <vector>

enum class NodeType { ClauseNode, SynNode };

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node {
 protected:
  NodeType type;
  std::vector<NodePtr> neighbours;
  const int id;

 public:
  Node(NodeType nodeType, int id);
  void addNeighbour(NodePtr n);

  std::vector<NodePtr> getNeighbours();
  NodeType getNodeType();
  int getId();

  virtual ~Node() {}
};
