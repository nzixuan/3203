#include "stdafx.h"

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalParentTest) {
  struct EvalParentStub : PKB {
    bool hasLhsConstantRhsWildcardForRel(QueryConstant lhsConstant,
                                         RelRefType relRefType) override {
      return lhsConstant.val == "1" &&
             lhsConstant.type == QueryConstantType::Number &&
             relRefType == RelRefType::Parent;
    }
  };

  PKB* pkb = new EvalParentStub();

 public:
  TEST_METHOD(TestConstWildcard) {
    // Select BOOLEAN such that Parent (1, _)
    ClauseArgs ca;
    ca.addRelArg(number);
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Parent};

    Query q;
    q.getClauseManager()->addClause(s);
    q.setBooleanSelectFlag();
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 1);
    Assert::IsTrue(lst.front() == "TRUE");
  }
};
}  // namespace UnitTesting
