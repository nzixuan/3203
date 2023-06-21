#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalSelectAttrTest) {
  struct EvalSelectAttrStub : PKB {
    std::list<PKBEntityPair> getLhsSynRhsSynForRel(
        EntityType lhsType, EntityType rhsType,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Call && rhsType == EntityType::Variable &&
          relRefType == RelRefType::Uses) {
        return {
            {{"3", EntityType::Call}, {"x", EntityType::Variable}},
            {{"4", EntityType::Call}, {"y", EntityType::Variable}},
        };
      }
      return {};
    }

    std::list<PKBEntity> getPrintStmts() override {
      return {{"2", EntityType::Print}, {"7", EntityType::Print}};
    }

    std::list<PKBEntity> getConstants() override {
      return {{"35", EntityType::Constant}, {"190", EntityType::Constant}};
    }

    PKBEntity getProcEntityFromCall(int callsStmtNo) override {
      if (callsStmtNo == 3) {
        return {"proc1", EntityType::Procedure};
      }
      if (callsStmtNo == 4) {
        return {"B", EntityType::Procedure};
      }
      return {};
    }

    PKBEntity getVarEntityFromPrint(int printStmtNo) override {
      if (printStmtNo == 2) {
        return {"var1", EntityType::Variable};
      }
      if (printStmtNo == 7) {
        return {"abc", EntityType::Variable};
      }
      return {};
    }
  };

  PKB* pkb = new EvalSelectAttrStub();

 public:
  TEST_METHOD(TestSelectCall) {
    // call c; variable v; Select c.stmt# such that Uses(c, s);
    ClauseArgs ca;
    ca.addRelArg(callSyn);
    ca.addRelArg(varSyn);
    SuchThat s{ca, RelRefType::Uses};

    Query q;
    q.getClauseManager()->addClause(s);
    AttrRef ar{callSyn, AttrName::StmtNo};
    q.addSelectArg(ar);
    q.finalize();

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSelectCallProcName) {
    // call c; variable v; Select c.procName such that Uses(c, s);
    ClauseArgs ca;
    ca.addRelArg(callSyn);
    ca.addRelArg(varSyn);
    SuchThat s{ca, RelRefType::Uses};

    Query q;
    q.getClauseManager()->addClause(s);
    AttrRef ar{callSyn, AttrName::ProcName};
    q.addSelectArg(ar);
    q.finalize();

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "proc1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "B") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSelectPrintVarName) {
    // print pn; Select pn.varName;
    Query q;
    AttrRef ar{printSyn, AttrName::VarName};
    q.addSelectArg(ar);

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "var1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "abc") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestSelectConstVal) {
    // constant c; Select c.value
    Query q;
    AttrRef ar{constSyn, AttrName::Value};
    q.addSelectArg(ar);

    // Evaluate query
    std::list<std::string> lst;
    QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
    ev.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "35") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "190") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
