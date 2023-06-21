#include "Table.h"

bool Table::isTableEmpty() { return table.empty(); }

bool Table::contains(const std::string& var) {
  return vars.find(var) != vars.end();
}

bool Table::contains(const std::list<std::string>& lst) {
  bool res = true;
  for (const auto& var : lst) {
    res = res && vars.find(var) != vars.end();
  }
  return res;
}

bool Table::mapContains(std::unordered_map<std::string, std::string>& map,
                        const std::string& targetKey) {
  return map.find(targetKey) != map.end();
}

void Table::initTable(std::string syn, std::list<PKBEntity>& vals) {
  vars.insert(syn);
  for (const auto& [val, entityType] : vals) {
    table.push_back({{syn, val}});
  }
}

void Table::initTable(std::string syn1, std::string syn2,
                      std::list<PKBEntityPair>& valPairs) {
  vars.insert(syn1);
  vars.insert(syn2);
  for (const auto& [lhs, rhs] : valPairs) {
    table.push_back({{syn1, lhs.val}, {syn2, rhs.val}});
  }
}

void Table::update(const std::string& syn, std::list<PKBEntity>& lst) {
  if (isNewTable) {
    initTable(syn, lst);
    isNewTable = false;
    return;
  }
  // Generate set of valid values for searching
  std::set<std::string> validVals;
  for (auto& i : lst) {
    validVals.insert(i.val);
  }
  if (contains(syn)) {
    shrinkTable(syn, validVals);
  } else {
    crossProduct(syn, validVals);
  }
}

void Table::update(const std::string& syn1, const std::string& syn2,
                   std::list<PKBEntityPair>& lst) {
  if (isNewTable) {
    initTable(syn1, syn2, lst);
    isNewTable = false;
    return;
  }
  if (contains(syn1) && contains(syn2)) {
    shrinkTableBothSyn(syn1, syn2, lst);
  } else if (contains(syn1)) {
    shrinkTableOneSyn(syn1, syn2, true, lst);
  } else if (contains(syn2)) {
    shrinkTableOneSyn(syn1, syn2, false, lst);
  } else {
    // Generate set of valid values for searching
    std::set<std::string> validValsLhs;
    std::set<std::string> validValsRhs;
    for (auto const& [lhs, rhs] : lst) {
      validValsLhs.insert(lhs.val);
      validValsRhs.insert(rhs.val);
    }
    crossProduct(syn1, validValsLhs);
    crossProduct(syn2, validValsRhs);
  }
}

void Table::crossProduct(const std::string& syn,
                         const std::set<std::string>& validVals) {
  vars.insert(syn);
  std::vector<std::unordered_map<std::string, std::string>> newTable;
  for (auto const& it : validVals) {
    for (auto row : table) {
      row[syn] = it;
      newTable.push_back(row);
    }
  }
  table = newTable;
}

void Table::shrinkTable(const std::string& syn,
                        std::set<std::string> validVals) {
  // Remove rows from table if synonym has an invalid value
  const auto it =
      std::remove_if(table.begin(), table.end(), [syn, &validVals](auto& map) {
        return validVals.find(map[syn]) == validVals.end();
      });
  table.erase(it, table.end());
}

void Table::shrinkTableOneSyn(const std::string& syn1, const std::string& syn2,
                              bool isLeft,
                              const std::list<PKBEntityPair>& pkbEntityPairs) {
  // Set of values for target syn
  std::set<std::string> validVals;
  // Map of value of syn in table to list of values of other syn
  std::unordered_map<std::string, std::list<std::string>> validPairs;
  for (auto const& [lhs, rhs] : pkbEntityPairs) {
    if (isLeft) {
      // LHS variable exists in table
      validVals.insert(lhs.val);
      validPairs[lhs.val].emplace_back(rhs.val);
    } else {
      // RHS variable exists in table
      validVals.insert(rhs.val);
      validPairs[rhs.val].emplace_back(lhs.val);
    }
  }

  const std::string targetSyn = isLeft ? syn1 : syn2;
  const std::string otherSyn = isLeft ? syn2 : syn1;
  shrinkTable(targetSyn, validVals);

  vars.insert(otherSyn);
  std::vector<std::unordered_map<std::string, std::string>> newTable;
  for (std::unordered_map<std::string, std::string> row : table) {
    auto it = validPairs.find(row[targetSyn]);
    if (it != validPairs.end()) {
      for (auto const& val : it->second) {
        row[otherSyn] = val;
        newTable.push_back(row);
      }
    }
  }
  table = newTable;
}

void Table::shrinkTableBothSyn(const std::string& syn1, const std::string& syn2,
                               const std::list<PKBEntityPair>& pkbEntityPairs) {
  std::list<std::pair<std::string, std::string>> vals;
  for (auto const& [lhs, rhs] : pkbEntityPairs) {
    vals.emplace_back(lhs.val, rhs.val);
  }
  const auto it = std::remove_if(
      table.begin(), table.end(), [syn1, syn2, &vals](auto& map) {
        return std::find(vals.begin(), vals.end(),
                         make_pair(map[syn1], map[syn2])) == vals.end();
      });
  table.erase(it, table.end());
}

std::list<std::vector<std::string>> Table::getValues(
    const std::list<std::string>& lst) {
  std::list<std::vector<std::string>> values;
  if (contains(lst)) {
    for (auto& row : table) {
      std::vector<std::string> tuple;
      for (auto const& syn : lst) {
        tuple.push_back(row[syn]);
      }
      values.push_back(tuple);
    }
  }
  return values;
}

std::list<PKBEntity> Table::getPkbEntities(std::string syn) {
  std::list<PKBEntity> values;
  if (contains(syn)) {
    for (auto& row : table) {
      values.push_back({row[syn], EntityType::Assign});
    }
  }
  return values;
}

void Table::combineTable(Table other) {
  if (isNewTable) {
    vars = other.vars;
    table = other.table;
    isNewTable = false;
    return;
  }
  vars.merge(other.vars);
  std::vector<std::unordered_map<std::string, std::string>> newTable;
  for (auto row : table) {
    for (auto otherRow : other.table) {
      auto tempRow = row;
      tempRow.merge(otherRow);
      newTable.push_back(tempRow);
    }
  }
  table = newTable;
}

void Table::removeIfNotEqual(std::string syn1, std::string syn2) {
  const auto it = std::remove_if(
      table.begin(), table.end(),
      [syn1, syn2](auto& map) { return map[syn1] != map[syn2]; });
  table.erase(it, table.end());
}
