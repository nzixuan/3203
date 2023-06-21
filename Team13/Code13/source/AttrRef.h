#pragma once
#include "Synonym.h"

class AttrRef : public Synonym {
 private:
  AttrName attrName;

 public:
  AttrRef(Synonym s, AttrName an);

  AttrName getAttrName();

  friend bool operator==(const AttrRef& lhs, const AttrRef& rhs) {
    return lhs.attrName == rhs.attrName && (Synonym)lhs == (Synonym)rhs;
  }
};
