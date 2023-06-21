#include "stdafx.h"

#include <algorithm>

#include "CppUnitTest.h"
#include "QueryEvaluator.h"
#include "common.h"
#include "testUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(EvalFollowsPatternTest) {
  struct EvalFollowsPatternStub : PKB {
    std::list<PKBEntity> getLhsSynRhsWildcardForRel(
        EntityType lhsType, RelRefType relRefType) override {
      if (lhsType == EntityType::Assign && relRefType == RelRefType::Follows) {
        return {
            {"3", EntityType::Assign},
            {"4", EntityType::Assign},
            {"5", EntityType::Assign},
        };
      }
      return {};
    }

    std::list<PKBEntity> getPatternAssignLhsWildcardRhsExp(
        std::string exp, bool isExactMatch) override {
      if (exp == "x" && !isExactMatch) {
        return {
            {"2", EntityType::Assign},
            {"3", EntityType::Assign},
            {"4", EntityType::Assign},
        };
      }
      return {};
    }
  };

  PKB* pkb = new EvalFollowsPatternStub();

  TEST_METHOD(TestFollowsPattern) {
    // assign a; Select a such that Follows(a, _) pattern a(_, _"x"_)
    ClauseArgs ca;
    ca.addRelArg(Synonym{"a", EntityType::Assign});
    ca.addRelArg(wildcard);
    SuchThat s{ca, RelRefType::Follows};

    ca = ClauseArgs{};
    ca.addRelArg(wildcard);
    ca.addRelArg(SourceArg{"x", false});
    Pattern p = {ca, {"a", EntityType::Assign}};

    Query q;
    q.addSelectArg(assignSyn);
    q.getClauseManager()->addClause(s);
    q.getClauseManager()->addClause(p);
    q.finalize();

    std::list<std::string> lst;
    QueryEvaluator evaluator{q, pkb, lst};
    evaluator.evaluate();

    Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
    Assert::IsTrue(std::find(lst.begin(), lst.end(), "4") != lst.end());
    Assert::IsTrue(lst.size() == 2);
  }
};
}  // namespace UnitTesting
