#pragma once
#include <algorithm>
#include <memory>

#include "Node.h"

class SynNode;
typedef std::shared_ptr<SynNode> SynNodePtr;

class SynNode : public Node {
 public:
  bool hasVisitedAll = false;
  explicit SynNode(int id);
};
