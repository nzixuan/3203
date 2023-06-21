#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalFollowsTTest) {
  struct EvalFollowsTStub : PKB {
    std::list<PKBEntity> getLhsSynRhsConstantForRel(
        EntityType lhsType, QueryConstant rhsConstant,
        RelRefType relRefType) override {
      if (lhsType == EntityType::Assign && rhsConstant.val == "4" &&
          rhsConstant.type == QueryConstantType::Number &&
          relRefType == RelRefType::FollowsT) {
        return {{"1", EntityType::Assign},
                {"2", EntityType::Assign},
                {"3", EntityType::Assign}};
      }
      return {};
    }
  };

  PKB* pkb = new EvalFollowsTStub();

 public:
  TEST_METHOD(TestFollowsT) {
    // assign a; Select a such that Follows*(a, 4)
    ClauseArgs ca;
    ca.addRelArg(assignSyn);
    ca.addRelArg(Number{"4"});
    SuchThat s{ca, RelRefType::FollowsT};

    Query q;
    q.getClauseManager()->addClause(s);
    q.addSelectArg(assignSyn);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(lst.size() == 3);
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "1") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
  }
};
}  // namespace UnitTesting
