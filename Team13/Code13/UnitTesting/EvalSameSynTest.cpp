#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalSameSynTest) {
  struct EvalSameSynStub : PKB {
    std::list<PKBEntity> getLhsRhsSameSynObj(EntityType type,
                                             RelRefType relRefType) override {
      if (type == EntityType::Stmt && relRefType == RelRefType::NextT) {
        return {{"3", EntityType::Stmt}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalSameSynStub();

 public:
  TEST_METHOD(TestSameSynNextT) {
    ClauseArgs ca;
    ca.addRelArg(stmtSyn);
    ca.addRelArg(stmtSyn);
    SuchThat s{ca, RelRefType::NextT};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(stmtSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 1);
  }
};
}  // namespace UnitTesting
