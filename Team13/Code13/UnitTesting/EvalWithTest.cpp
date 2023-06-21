#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalWithTest) {
  struct EvalWithStub : PKB {
    std::list<PKBEntity> getAssignStmts() override {
      return {{"4", EntityType::Assign}, {"5", EntityType::Assign}};
    }

    std::list<PKBEntity> getPrintStmts() override {
      return {{"2", EntityType::Print},
              {"3", EntityType::Print},
              {"4", EntityType::Print}};
    }

    std::list<PKBEntity> getReadStmts() override {
      return {{"7", EntityType::Read},
              {"9", EntityType::Read},
              {"10", EntityType::Read}};
    }

    PKBEntity getVarEntityFromPrint(int printStmtNo) override {
      if (printStmtNo == 2) {
        return {"v1", EntityType::Variable};
      } else if (printStmtNo == 3 || printStmtNo == 4) {
        return {"v", EntityType::Variable};
      }
      return {};
    }

    PKBEntity getVarEntityFromRead(int readStmtNo) override {
      if (readStmtNo == 10) {
        return {"v1", EntityType::Variable};
      } else if (readStmtNo == 7) {
        return {"v", EntityType::Variable};
      } else if (readStmtNo = 9) {
        return {"abc", EntityType::Variable};
      }
      return {};
    }

    std::list<PKBEntity> getLhsSynRhsConstantForRel(
        EntityType lhsType, QueryConstant rhsConstant,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Print && rhsConstant.val == "1" &&
          rhsConstant.type == QueryConstantType::Number &&
          relRefType == RelRefType::FollowsT) {
        return {{"3", EntityType::Print},
                {"4", EntityType::Print},
                {"5", EntityType::Print}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalWithStub();

 public:
  TEST_METHOD(TestBoolWithTrue) {
    // assign a; Select a with 12 = 12
    ClauseArgs ca;
    Number s{"12"};
    ca.addRelArg(s);
    ca.addRelArg(s);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "5") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestBoolWithFalse) {
    // Select BOOLEAN with 1=2
    ClauseArgs ca;
    Number s{"1"};
    ca.addRelArg(s);
    s = {"2"};
    ca.addRelArg(s);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.front() == "FALSE");
  }
  TEST_METHOD(TestOneSynWithTrue) {
    // assign a; Select a with 5 = a.stmt#
    ClauseArgs ca;
    Number sa{"5"};
    ca.addRelArg(sa);
    AttrRef attrRef{assignSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "5") != lst.end());
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestOneSynWithFalse) {
    // assign a; Select a with a.stmt# = 10
    ClauseArgs ca;
    AttrRef attrRef{assignSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    Number sa{"10"};
    ca.addRelArg(sa);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.empty());
  }
  TEST_METHOD(TestOneSynWithSpecialTrue) {
    // print pn; Select pn with pn.varName = "v"
    ClauseArgs ca;
    AttrRef attrRef{printSyn, AttrName::VarName};
    ca.addRelArg(attrRef);
    SourceArg sa{"v", true};
    ca.addRelArg(sa);
    With w{ca, WithRefType::Name};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(printSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
  TEST_METHOD(TestOneSynWithSpecialFalse) {
    // print pn; Select pn with pn.varName = "x"
    ClauseArgs ca;
    AttrRef attrRef{printSyn, AttrName::VarName};
    ca.addRelArg(attrRef);
    SourceArg sa{"x", true};
    ca.addRelArg(sa);
    With w{ca, WithRefType::Name};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(printSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.empty());
  }
  TEST_METHOD(TestTwoSynWithTrue) {
    // assign a; print pn; Select <a, pn> with a.stmt# = pn.stmt#
    ClauseArgs ca;
    AttrRef attrRef{assignSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    attrRef = {printSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(assignSyn);
    q.addSelectArg(printSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4 4") != lst.end());
    Assert::IsTrue(lst.size() == 1);
  }
  TEST_METHOD(TestTwoSynWithFalse) {
    // read r; print pn; Select <r, pn> with r.stmt# = pn.stmt#
    ClauseArgs ca;
    AttrRef attrRef{readSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    attrRef = {printSyn, AttrName::StmtNo};
    ca.addRelArg(attrRef);
    With w{ca, WithRefType::Integer};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(readSyn);
    q.addSelectArg(printSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.empty());
  }
  TEST_METHOD(TestTwoSynWithSpecial) {
    // read r; print pn; Select <r, pn, pn.varName> with c.varName = pn.varName
    ClauseArgs ca;
    AttrRef attrRef{readSyn, AttrName::VarName};
    ca.addRelArg(attrRef);
    attrRef = {printSyn, AttrName::VarName};
    ca.addRelArg(attrRef);
    With w{ca, WithRefType::Name};

    Query q;
    q.getClauseManager()->addClause(w);
    q.addSelectArg(readSyn);
    q.addSelectArg(printSyn);
    attrRef = {printSyn, AttrName::VarName};
    q.addSelectArg(attrRef);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "7 3 v") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "7 4 v") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "10 2 v1") != lst.end());
    Assert::IsTrue(lst.size() == 3);
  }
};
}  // namespace UnitTesting
