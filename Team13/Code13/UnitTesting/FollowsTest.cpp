#include "stdafx.h"
#include "CppUnitTest.h"
#include "Query.h"
#include "QueryEvaluator.h"
#include "PKBStub.h"

#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(FollowsTest) {
        PKB* pkb = new PKBStub();

    public:
        /*======================== Boolean Follows ========================*/
        TEST_METHOD(TestConstConstTrue) {
            // assign a; Select a such that Follows(1, 2)
            std::string varName = "a";
            std::map<std::string, EntityType> map{ {varName, EntityType::Assign} };
            SuchThat s = { { "1", "2", RelItemType::StmtNo, RelItemType::StmtNo },
               RelRefType::Follows };
            Query q;
            q.addSelectSyn({ varName, EntityType::Assign });
            q.setDeclarations(map);
            q.addSuchThat(s);

            std::list<std::string> lst;
            QueryEvaluator evaluator{ q, pkb, lst };
            evaluator.evaluate();

            Assert::IsTrue(std::find(lst.begin(), lst.end(), "1") != lst.end());
            Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
            Assert::IsTrue(std::find(lst.begin(), lst.end(), "3") != lst.end());
        }
        TEST_METHOD(TestConstConstFalse) {
            // assign a; Select a such that Follows(1, 3)
            std::string varName = "a";
            std::map<std::string, EntityType> map{ {varName, EntityType::Assign} };
            SuchThat s = { { "1", "3", RelItemType::StmtNo, RelItemType::StmtNo },
                RelRefType::Follows };
            Query q;
            q.addSelectSyn({ varName, EntityType::Assign });
            q.setDeclarations(map);
            q.addSuchThat(s);

            std::list<std::string> lst;
            QueryEvaluator evaluator{ q, pkb, lst };
            evaluator.evaluate();

            Assert::IsTrue(lst.empty());
        }
        TEST_METHOD(TestConstWildcardTrue) {
            // variable v; Select v such that Follows(1, _)
            std::string varName = "v";
            std::map<std::string, EntityType> map{ {varName, EntityType::Variable} };
            SuchThat s = { { "1", "_", RelItemType::StmtNo, RelItemType::Wildcard},
                RelRefType::Follows };
            Query q;
            q.addSelectSyn({ varName, EntityType::Assign });
            q.setDeclarations(map);
            q.addSuchThat(s);

            std::list<std::string> lst;
            QueryEvaluator evaluator{ q, pkb, lst };
            evaluator.evaluate();

            std::list<std::string> vars{ "a", "b", "x", "y", "count", "west1" };
            for (auto i : vars) {
                Assert::IsTrue(std::find(lst.begin(), lst.end(), i) != lst.end());
            }
            Assert::IsTrue(lst.size() == 6);
        }
        TEST_METHOD(TestConstWildcardFalse) {
            // variable v; Select v such that Follows(3, _)
            std::string varName = "v";
            std::map<std::string, EntityType> map{ {varName, EntityType::Variable} };
            SuchThat s = { { "3", "_", RelItemType::StmtNo, RelItemType::Wildcard},
                RelRefType::Follows };
            Query q;
            q.addSelectSyn({ varName, EntityType::Assign });
            q.setDeclarations(map);
            q.addSuchThat(s);

            std::list<std::string> lst;
            QueryEvaluator evaluator{ q, pkb, lst };
            evaluator.evaluate();

            Assert::IsTrue(lst.empty());
        }
        TEST_METHOD(TestWildcardConst) {
            // variable v; Select v such that Follows(_, 2)
            std::string varName = "v";
            std::map<std::string, EntityType> map{ {varName, EntityType::Variable} };
            SuchThat s = { { "_", "2", RelItemType::Wildcard, RelItemType::StmtNo},
                RelRefType::Follows };
            Query q;
            q.addSelectSyn({ varName, EntityType::Assign });
            q.setDeclarations(map);
            q.addSuchThat(s);

            std::list<std::string> lst;
            QueryEvaluator evaluator{ q, pkb, lst };
            evaluator.evaluate();

            std::list<std::string> vars{ "a", "b", "x", "y", "count", "west1" };
            for (auto i : vars) {
                Assert::IsTrue(std::find(lst.begin(), lst.end(), i) != lst.end());
            }
            Assert::IsTrue(lst.size() == 6);
        }

        /*====================== One synonym Follows ======================*/

        TEST_METHOD(TestSynWildcard) {
            // assign a; Select a such that Follows(a, _)
            std::string var = "a";
            std::map<std::string, EntityType> map{ { var, EntityType::Assign} };
            SuchThat s = { { var, "_", RelItemType::Synonym,
                RelItemType::Wildcard } , RelRefType::Follows };
            Query q;
            q.setDeclarations(map);
            q.addSelectSyn({ var, EntityType::Assign });
            q.addSuchThat(s);

            // Evaluate query
            std::list<std::string> lst;
            QueryEvaluator ev = QueryEvaluator(q, pkb, lst);
            ev.evaluate();

            std::list<PKBEntity> a = pkb->getLhsSynRhsWildcardForRel(EntityType::Stmt, RelRefType::Follows);

            Assert::IsTrue(std::find(lst.begin(), lst.end(), "1") != lst.end());
            Assert::IsTrue(std::find(lst.begin(), lst.end(), "2") != lst.end());
        }
    };
}
