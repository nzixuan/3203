#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalPatternAssignTest) {
  struct EvalPatternAssignStub : PKB {
    std::list<PKBEntity> getPatternAssignLhsWildcardRhsExp(
        std::string exp, bool isExactMatch) override {
      if (exp == "x" && !isExactMatch) {
        return {{"2", EntityType::Assign}, {"3", EntityType::Assign}};
      }
      return {};
    }

    std::list<PKBEntityPair> getPatternAssignLhsSynRhsExp(
        std::string exp, bool isExactMatch) override {
      if (exp == "y" && !isExactMatch) {
        return {
            {{"10", EntityType::Assign}, {"g", EntityType::Variable}},
            {{"7", EntityType::Assign}, {"var", EntityType::Variable}},
        };
      }
      if (exp == "1" && !isExactMatch) {
        return {{{"13", EntityType::Assign}, {"d", EntityType::Variable}}};
      }
      if (exp == "x + y" && isExactMatch) {
        return {{{"3", EntityType::Assign}, {"d", EntityType::Variable}}};
      }
      return {};
    }

    std::list<PKBEntityPair> getPatternAssignLhsSynRhsWildcard() override {
      return {
          {{"30", EntityType::Assign}, {"abc", EntityType::Variable}},
          {{"17", EntityType::Assign}, {"var1", EntityType::Variable}},
      };
    }
  };

  PKB* pkb = new EvalPatternAssignStub();

  /*====================== One synonym Pattern ====================== */

  TEST_METHOD(TestWildcardArg) {
    // assign a; Select a pattern a(_, _"x"_)
    ClauseArgs ca;
    ca.addRelArg(wildcard);
    ca.addRelArg(SourceArg{"x", false});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(assignSyn);
    q.finalize();

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }

  /*====================== Two synonym Pattern ====================== */

  TEST_METHOD(TestSynPExpValidAssign) {
    // LHS: syn, RHS: partial exp, Valid result: true, Return type: single,
    // assign assign a; variable v; Select a pattern a(v, _"y"_)
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"y", false});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "10") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "7") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSynPExpValidVar) {
    // LHS: syn, RHS: partial exp, Valid result: true, Return type: single, var
    // assign a; variable v; Select v pattern a(v, _"y"_)
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"y", false});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(varSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "g") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "var") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSynPExpValidTuple) {
    // LHS: syn, RHS: partial exp, Valid result: true, Return type: tuple
    // assign a; variable v; Select <v, a> pattern a(v, _"y"_)
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"y", false});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(varSyn);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "g 10") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "var 7") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSynFExpValidBoolean) {
    // LHS: syn, RHS: full exp, Valid result: true, Return type: boolean
    // assign a; variable v; Select BOOLEAN pattern a(v, "x + y")
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"x + y"});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.front() == "TRUE");
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestSynFExpInvalidBoolean) {
    // LHS: syn, RHS: full exp, Valid result: false, Return type: boolean
    // assign a; variable v; Select BOOLEAN pattern a(v, "z + a")
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"z + a"});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.front() == "FALSE");
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestSynConst) {
    // assign a; variable v; Select a pattern a(v, _"1"_)
    ClauseArgs ca;
    ca.addRelArg(varSyn);
    ca.addRelArg(SourceArg{"1", false});
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "13") != lst.end());
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestSynWildcard) {
    // assign a; variable var; Select var pattern a (var, _)
    ClauseArgs ca;
    ca.addRelArg(Synonym{"var", EntityType::Variable});
    ca.addRelArg(wildcard);
    Pattern p{ca, assignSyn};

    Query q;
    q.getClauseManager()->addClause(p);
    q.addSelectArg(Synonym{"var", EntityType::Assign});
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "abc") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "var1") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
