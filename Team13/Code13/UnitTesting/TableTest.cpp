#include "stdafx.h"

#include "CppUnitTest.h"
#include "PKBEntity.h"
#include "Table.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TableTest) {
  std::string syn = "a";
  std::list<PKBEntity> pkbEntities{{"1", EntityType::Assign},
                                   {"4", EntityType::Assign}};

 public:
  TEST_METHOD(TestInit) {
    Table t;
    t.update(syn, pkbEntities);
    Assert::IsFalse(t.isTableEmpty());
  }
  TEST_METHOD(TestShrink) {
    Table t;
    t.update(syn, pkbEntities);
    std::list<PKBEntity> validVals{{"4", EntityType::Assign}};
    t.update(syn, validVals);
    Assert::IsTrue(t.getValues({syn}).size() == 1);
  }
  TEST_METHOD(TestCrossProduct) {
    Table t;
    t.update(syn, pkbEntities);
    t.update("b", pkbEntities);
    Assert::IsTrue(t.getValues({syn}).size() == 4);
  }
  TEST_METHOD(TestGetValues) {
    Table t;
    t.update(syn, pkbEntities);
    std::list<std::vector<std::string>> vals = t.getValues({syn});
    Assert::IsTrue(vals.size() == 2);
  }
  TEST_METHOD(TestRemoveIfNotEqual) {
    Table t;
    t.update(syn, pkbEntities);
    std::list<PKBEntity> lst = {{"4", EntityType::Assign},
                                {"5", EntityType::Assign}};
    t.update("b", lst);
    t.removeIfNotEqual(syn, "b");
    std::vector<std::string> res = t.getValues({"a", "b"}).front();
    for (auto const& i : res) {
      Assert::IsTrue(i == "4");
    }
  }
  TEST_METHOD(TestCombineTable) {
    Table t;
    t.update(syn, pkbEntities);

    Table other;
    std::list<PKBEntityPair> lst{
        {{"3", EntityType::Stmt}, {"5", EntityType::Stmt}},
        {{"4", EntityType::Stmt}, {"3", EntityType::Stmt}},
        {{"7", EntityType::Stmt}, {"9", EntityType::Stmt}},
        {{"10", EntityType::Stmt}, {"2", EntityType::Stmt}}};
    other.update("b", "c", lst);
    t.combineTable(other);
    auto vals = t.getValues({"a", "b"});
    Assert::IsTrue(vals.size() == 8);
  }
};
}  // namespace UnitTesting
