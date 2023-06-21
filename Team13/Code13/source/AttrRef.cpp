#include "AttrRef.h"

AttrRef::AttrRef(Synonym s, AttrName an) : Synonym(s), attrName(an) {
  this->type = RelArgType::AttrRef;
}

AttrName AttrRef::getAttrName() { return attrName; }
