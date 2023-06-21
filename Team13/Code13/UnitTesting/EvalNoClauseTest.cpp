#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalNoClauseTest) {
  struct EvalNoClauseStub : PKB {
    std::list<PKBEntity> getVariables() override {
      return {{"a", EntityType::Variable},
              {"xyz", EntityType::Variable},
              {"b", EntityType::Variable}};
    }

    std::list<PKBEntity> getAllStmts() override {
      return {{"1", EntityType::Assign},
              {"2", EntityType::Print},
              {"3", EntityType::Read}};
    }

    std::list<PKBEntity> getProcList() override {
      return {{"A", EntityType::Procedure}, {"main", EntityType::Procedure}};
    }
  };

  PKB* pkb = new EvalNoClauseStub();

 public:
  TEST_METHOD(TestGetAllVars) {
    // variable v; Select v
    Query q;
    q.addSelectArg(varSyn);

    // Evaluate query
    std::list<std::string> varLst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, varLst);
    ev.evaluate();

    // Check that all vars in result list
    std::list<std::string> vars{"a", "b", "xyz"};
    for (auto const& i : vars) {
      Assert::IsTrue(std::find(varLst.begin(), varLst.end(), i) !=
                     varLst.end());
    }
    Assert::IsTrue(varLst.size() == 3);
  }
  TEST_METHOD(TestGetAllStmts) {
    // stmt s; Select s
    Query q;
    q.addSelectArg(stmtSyn);

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
  }
  TEST_METHOD(TestGetAllProc) {
    // proc p; Select p
    Query q;
    q.addSelectArg(procSyn);

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "main") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "A") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestGetAllStmtProc) {
    // stmt s; proc p; Select <s, p>
    std::string syn = "p";
    std::map<std::string, EntityType> map{{"s", EntityType::Stmt},
                                          {"p", EntityType::Procedure}};
    Query q;

    q.addSelectArg(Synonym{"s", EntityType::Stmt});
    q.addSelectArg(Synonym{"p", EntityType::Procedure});

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(lst.size() == 6);
  }
  TEST_METHOD(TestBoolNoClause) {
    // Select BOOLEAN
    Query q;
    q.setBooleanSelectFlag();

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(lst.size() == 1);
    Assert::IsTrue(lst.front() == "TRUE");
  }
};
}  // namespace UnitTesting
