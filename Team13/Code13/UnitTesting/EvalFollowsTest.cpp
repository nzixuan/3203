#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalFollowsTest) {
  struct EvalFollowsStub : PKB {
    std::list<PKBEntity> getVariables() override {
      return {{"abc3", EntityType::Variable},
              {"b", EntityType::Variable},
              {"test2", EntityType::Variable}};
    }

    std::list<PKBEntity> getAssignStmts() override {
      return {{"1", EntityType::Assign},
              {"2", EntityType::Assign},
              {"3", EntityType::Assign}};
    }

    std::list<PKBEntity> getPrintStmts() override {
      return {{"4", EntityType::Print}};
    }

    bool hasLhsConstantRhsConstantForRel(QueryConstant lhsConstant,
                                         QueryConstant rhsConstant,
                                         RelRefType relRefType) override {
      return lhsConstant.val == "1" &&
             lhsConstant.type == QueryConstantType::Number &&
             rhsConstant.val == "2" &&
             rhsConstant.type == QueryConstantType::Number &&
             relRefType == RelRefType::Follows;
    }

    bool hasLhsConstantRhsWildcardForRel(QueryConstant lhsConstant,
                                         RelRefType relRefType) override {
      return lhsConstant.val == "1" &&
             lhsConstant.type == QueryConstantType::Number &&
             relRefType == RelRefType::Follows;
    }

    bool hasLhsWildcardRhsConstantForRel(QueryConstant rhsConstant,
                                         RelRefType relRefType) override {
      return rhsConstant.val == "2" &&
             rhsConstant.type == QueryConstantType::Number &&
             relRefType == RelRefType::Follows;
    }

    bool hasLhsWildcardRhsWildcardForRel(RelRefType relRefType) override {
      return relRefType == RelRefType::Follows;
    }

    std::list<PKBEntity> getLhsSynRhsWildcardForRel(
        EntityType lhsType, RelRefType relRefType) override {
      if (lhsType == EntityType::Assign && relRefType == RelRefType::Follows) {
        return {{"3", EntityType::Assign}, {"4", EntityType::Assign}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalFollowsStub();

 public:
  /*======================== Boolean Follows ========================*/
  TEST_METHOD(TestConstConstTrue) {
    // assign a; Select a such that Follows(1, 2)
    ClauseArgs ca;
    ca.addRelArg(number);
    ca.addRelArg(number2);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
  }
  TEST_METHOD(TestConstConstFalse) {
    // assign a; Select a such that Follows(1, 3)
    ClauseArgs ca;
    ca.addRelArg(number);
    ca.addRelArg(Number{"3"});
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.empty());
  }
  TEST_METHOD(TestConstConstBool) {
    // Select BOOLEAN such that Follows(1, 3)
    ClauseArgs ca;
    ca.addRelArg(number);
    ca.addRelArg(Number{"3"});
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 1);
    Assert::IsTrue(lst.front() == "FALSE");
  }
  TEST_METHOD(TestConstWildcardTrue) {
    // variable v; Select v such that Follows(1, _)
    ClauseArgs ca;
    ca.addRelArg(number);
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(varSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    std::list<std::string> vars{"abc3", "b", "test2"};
    for (auto i : vars) {
      Assert::IsTrue(std::find(lst.begin(), lst.end(), i) != lst.end());
    }
    Assert::IsTrue(lst.size() == 3);
  }
  TEST_METHOD(TestConstWildcardFalse) {
    // variable v; Select v such that Follows(5, _)
    ClauseArgs ca;
    ca.addRelArg(Number{"5"});
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(varSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.empty());
  }
  TEST_METHOD(TestWildcardConstVar) {
    // variable v; Select v such that Follows(_, 2)
    ClauseArgs ca;
    ca.addRelArg(wildcard);
    ca.addRelArg(number2);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(varSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    std::list<std::string> vars{"abc3", "b", "test2"};
    for (auto i : vars) {
      Assert::IsTrue(std::find(lst.begin(), lst.end(), i) != lst.end());
    }
    Assert::IsTrue(lst.size() == 3);
  }
  TEST_METHOD(TestWildcardConstAssign) {
    // assign a; Select a such that Follows(_, 2)
    ClauseArgs ca;
    ca.addRelArg(wildcard);
    ca.addRelArg(number2);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 3);
  }
  TEST_METHOD(TestWildcardWildcard) {
    // print pn; Select pn such that Follows (_,_)
    std::string syn = "pn";

    ClauseArgs ca;
    ca.addRelArg(wildcard);
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(Synonym{syn, EntityType::Print});
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 1);
  }

  /*====================== One synonym Follows ======================*/

  TEST_METHOD(TestSynWildcard) {
    // assign a; Select a such that Follows(a, _)
    ClauseArgs ca;
    ca.addRelArg(assignSyn);
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Follows};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(assignSyn);
    q.finalize();

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
