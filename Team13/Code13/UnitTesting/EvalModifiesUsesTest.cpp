#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalModifiesUsesTest) {
  struct EvalModifiesUsesStub : PKB {
    std::list<PKBEntityPair> getLhsSynRhsSynForRel(
        EntityType lhsType, EntityType rhsType,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Stmt && rhsType == EntityType::Variable &&
          relRefType == RelRefType::Modifies) {
        return {{{"3", EntityType::Assign}, {"abc3", EntityType::Assign}},
                {{"4", EntityType::Assign}, {"xy2z", EntityType::Assign}},
                {{"9", EntityType::Assign}, {"hjkl", EntityType::Assign}}};
      }
      if (lhsType == EntityType::Stmt && rhsType == EntityType::Variable &&
          relRefType == RelRefType::Uses) {
        return {{{"4", EntityType::Assign}, {"xy2z", EntityType::Assign}},
                {{"9", EntityType::Assign}, {"hjkl", EntityType::Assign}},
                {{"11", EntityType::Assign}, {"d4ef", EntityType::Assign}}};
      }
      if (lhsType == EntityType::Procedure && rhsType == EntityType::Variable &&
          relRefType == RelRefType::Modifies) {
        return {{{"C", EntityType::Procedure}, {"abc3", EntityType::Assign}},
                {{"main", EntityType::Procedure}, {"xy2z", EntityType::Assign}},
                {{"A", EntityType::Procedure}, {"hjkl", EntityType::Assign}}};
      }
      if (lhsType == EntityType::Procedure && rhsType == EntityType::Variable &&
          relRefType == RelRefType::Uses) {
        return {{{"main", EntityType::Procedure}, {"xy2z", EntityType::Assign}},
                {{"A", EntityType::Procedure}, {"hjkl", EntityType::Assign}},
                {{"B", EntityType::Procedure}, {"d4ef", EntityType::Assign}}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalModifiesUsesStub();

 public:
  TEST_METHOD(TestModifiesSUsesS) {
    // stmt s; variable v; Select <v, s> such that Modifies (s, v)
    // and Uses (s, v)
    ClauseArgs ca;
    ca.addRelArg(stmtSyn);
    ca.addRelArg(varSyn);
    SuchThat s1{ca, RelRefType::Modifies};
    SuchThat s2{ca, RelRefType::Uses};

    Query q;
    q.getClauseManager()->addClause(s1);
    q.getClauseManager()->addClause(s2);
    q.addSelectArg(varSyn);
    q.addSelectArg(stmtSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 2);
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "xy2z 4") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "hjkl 9") != lst.end());
  }
  TEST_METHOD(TestModifiesSUsesSModifiesPUsesP) {
    // procedure p; stmt s; variable v; Select <v, s, p> such that
    // Modifies (s, v) and Uses (s, v) and Modifies (p, v) and Uses (p, v)
    ClauseArgs ca;
    ca.addRelArg(stmtSyn);
    ca.addRelArg(varSyn);
    SuchThat s1{ca, RelRefType::Modifies};
    SuchThat s2{ca, RelRefType::Uses};
    ca = ClauseArgs{};
    ca.addRelArg(procSyn);
    ca.addRelArg(varSyn);
    SuchThat s3{ca, RelRefType::Modifies};
    SuchThat s4{ca, RelRefType::Uses};

    Query q;
    q.getClauseManager()->addClause(s1);
    q.getClauseManager()->addClause(s2);
    q.getClauseManager()->addClause(s3);
    q.getClauseManager()->addClause(s4);
    q.addSelectArg(varSyn);
    q.addSelectArg(stmtSyn);
    q.addSelectArg(procSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 2);
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "xy2z 4 main") !=
                   lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "hjkl 9 A") != lst.end());
  }
};
}  // namespace UnitTesting
