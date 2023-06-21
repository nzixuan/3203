#pragma once

#include <algorithm>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PKBEntity.h"

class Table {
  // Stores the list of variables in the table.
  std::unordered_set<std::string> vars;

  // Represents the table: each row is an element in the vector
  // that stores a map of variable to value.
  std::vector<std::unordered_map<std::string, std::string>> table;

  // Flag to determine if the table is new (no items added before).
  // To differentiate between new table and table with no valid results.
  bool isNewTable = true;

  // Returns true if the provided key is in the map, false otherwise.
  static bool mapContains(std::unordered_map<std::string, std::string>&,
                          const std::string&);

  // Given a synonym and list of values, initialise the table.
  void initTable(std::string, std::list<PKBEntity>&);

  // Given two synonyms and a list of value-pairs, initialise the table.
  void initTable(std::string, std::string, std::list<PKBEntityPair>&);

  // Given a synonym already in the table and a set of values,
  // shrink the table by removing rows with invalid values.
  void shrinkTable(const std::string&, std::set<std::string>);

  // Given two synonyms already in the table and a list of pairs of values,
  // shrink the table by removing rows with invalid values.
  void shrinkTableBothSyn(const std::string&, const std::string&,
                          const std::list<PKBEntityPair>&);

  // Given one synonym already in the table and one that is not, shrink the
  // table by removing rows with invalid values and update the table with the
  // additional synonym.
  void shrinkTableOneSyn(const std::string&, const std::string&, bool,
                         const std::list<PKBEntityPair>&);

  // Given a synonym not in the table and a set of values,
  // expand the table using a cross product.
  void crossProduct(const std::string&, const std::set<std::string>&);

 public:
  // Returns true if the table has no rows, false otherwise.
  bool isTableEmpty();

  // Returns true if the provided variable is in the table, false otherwise.
  bool contains(const std::string&);

  // Returns true if the provided variables are in the table, false otherwise.
  bool contains(const std::list<std::string>&);

  // Given a synonym and a list of PKBEntities, update the synonym to the
  // table if it is not already present or update the table otherwise.
  void update(const std::string&, std::list<PKBEntity>&);

  // Given a synonym and a list of PKBEntityPairs, update the synonym to the
  // table if it is not already present or update the table otherwise.
  void update(const std::string&, const std::string&,
              std::list<PKBEntityPair>&);

  // Combine the current table with the given table.
  void combineTable(Table);

  // Given two synonyms, removes rows of the table where the synonym values
  // do not match.
  void removeIfNotEqual(std::string syn1, std::string syn2);

  // Given a list of synonyms, return all valid values in the table.
  std::list<std::vector<std::string>> getValues(const std::list<std::string>&);

  std::list<PKBEntity> getPkbEntities(std::string);
};
