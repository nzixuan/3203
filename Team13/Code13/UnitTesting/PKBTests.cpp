#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(PKBTests) {
    public:
        PKB pkb;

        TEST_METHOD_INITIALIZE(initializePKB) {
        }
        TEST_METHOD(addProcedureTest) {
            pkb.addProcedure("testProc1");
            pkb.addProcedure("testProc2");
            std::list<PKBEntity> testProcList = { {"testProc1", EntityType::Procedure}, {"testProc2", EntityType::Procedure } };
            Assert::IsTrue(pkb.getProcList() == testProcList);

            Assert::IsTrue(pkb.getProcList().size() == 2);
        }

        TEST_METHOD(addStatementTest) {
            pkb.addStatement(1, EntityType::Read);
            pkb.addStatement(2, EntityType::Print);
            pkb.addStatement(3, EntityType::Assign);
            pkb.addStatement(4, EntityType::While);
            pkb.addStatement(5, EntityType::If);
            pkb.addStatement(6, EntityType::Call);


            Assert::IsTrue(pkb.getAllStmts().size() == 6);

            PKBEntity result = pkb.getStmtEntity(2);
            Assert::IsTrue(result.entityType == EntityType::Print);

        }

        TEST_METHOD(addVariableTest) {
            pkb.addVariable("testVar1");
            pkb.addVariable("testVar2");
            std::list<PKBEntity> testVarList = { {"testVar1", EntityType::Variable}, {"testVar2", EntityType::Variable} };
            Assert::IsTrue(pkb.getVariables() == testVarList);

            std::list<PKBEntity> result = pkb.getVariables();

            Assert::IsTrue(result.size() == 2);
        }

        TEST_METHOD(addConstantTest) {
            pkb.addConstant("testConst1");
            pkb.addConstant("testConst2");
            std::list<PKBEntity> testConstList = { {"testConst1", EntityType::Constant}, {"testConst2", EntityType::Constant} };
            Assert::IsTrue(pkb.getConstants() == testConstList);
        }

        TEST_METHOD(addFollowsTest) {
            pkb.storeRel({ "1", EntityType::Assign }, { "2", EntityType::Assign }, RelRefType::Follows);
            pkb.storeRel({ "2", EntityType::Assign }, { "3", EntityType::Assign }, RelRefType::Follows);
            pkb.storeRel({ "3", EntityType::Assign }, { "4", EntityType::Assign }, RelRefType::Follows);
            pkb.storeRel({ "4", EntityType::Assign }, { "5", EntityType::Assign }, RelRefType::Follows);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Assign, RelRefType::Follows);
            Assert::IsTrue(results.size() == 4);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            const std::list<PKBEntityPair> results2 = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Assign, RelRefType::FollowsT);
            Assert::IsTrue(results2.size() == 10);
            for (auto pair = results2.begin(); pair != results2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }
        }

        TEST_METHOD(addParentTest) {
            pkb.storeRel({ "1", EntityType::While }, { "2", EntityType::While }, RelRefType::Parent);
            pkb.storeRel({ "2", EntityType::While }, { "3", EntityType::While }, RelRefType::Parent);
            pkb.storeRel({ "3", EntityType::While }, { "4", EntityType::While }, RelRefType::Parent);
            pkb.storeRel({ "4", EntityType::While }, { "5", EntityType::Assign }, RelRefType::Parent);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::While, EntityType::Assign, RelRefType::Parent);
            Assert::IsTrue(results.size() == 1);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::Assign);
            }

            const std::list<PKBEntityPair> results2 = pkb.getLhsSynRhsSynForRel(EntityType::While, EntityType::Assign, RelRefType::ParentT);
            Assert::IsTrue(results2.size() == 4);
            for (auto pair = results2.begin(); pair != results2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::Assign);
            }
        }

        TEST_METHOD(addModifiesTest) {
            pkb.storeRel({ "1", EntityType::Assign }, { "x", EntityType::Variable }, RelRefType::Modifies);
            pkb.storeRel({ "2", EntityType::Assign }, { "y", EntityType::Variable }, RelRefType::Modifies);
            pkb.storeRel({ "3", EntityType::Assign }, { "z", EntityType::Variable }, RelRefType::Modifies);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Variable, RelRefType::Modifies);
            Assert::IsTrue(results.size() == 3);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Variable);
            }
        }

        TEST_METHOD(addUseTest) {
            pkb.storeRel({ "1", EntityType::Assign }, { "x", EntityType::Variable }, RelRefType::Uses);
            pkb.storeRel({ "1", EntityType::Assign }, { "y", EntityType::Variable }, RelRefType::Uses);
            pkb.storeRel({ "1", EntityType::Assign }, { "4", EntityType::Constant }, RelRefType::Uses);
            pkb.storeRel({ "2", EntityType::Assign }, { "4", EntityType::Constant }, RelRefType::Uses);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Variable, RelRefType::Uses);
            Assert::IsTrue(results.size() == 2);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Variable);
            }

            const std::list<PKBEntity> results2 = pkb.getLhsSynRhsConstantForRel(EntityType::Assign, {"4", QueryConstantType::Identifier}, RelRefType::Uses);
            Assert::IsTrue(results2.size() == 2);
            for (auto entity = results2.begin(); entity != results2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }
        }

        TEST_METHOD(addCallsTest) {
            pkb.storeRel({ "x", EntityType::Procedure }, { "y", EntityType::Procedure }, RelRefType::Calls);
            pkb.storeRel({ "y", EntityType::Procedure }, { "z", EntityType::Procedure }, RelRefType::Calls);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::Procedure, EntityType::Procedure, RelRefType::Calls);
            Assert::IsTrue(results.size() == 2);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Procedure && (pair->rhs).entityType == EntityType::Procedure);
            }

            const std::list<PKBEntity> results2 = pkb.getLhsSynRhsConstantForRel(EntityType::Procedure, { "z", QueryConstantType::Identifier }, RelRefType::CallsT);
            Assert::IsTrue(results2.size() == 2);
            for (auto entity = results2.begin(); entity != results2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            const std::list<PKBEntityPair> results3 = pkb.getLhsSynRhsSynForRel(EntityType::Procedure, EntityType::Procedure, RelRefType::CallsT);
            Assert::IsTrue(results3.size() == 3);
            for (auto pair = results3.begin(); pair != results3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Procedure && (pair->rhs).entityType == EntityType::Procedure);
            }
        }

        /*
            TODO: Add tests for inserting Next relations and Affects Relations after unit tests for relevant storage is implemented
        */

        TEST_METHOD(addNextTest) {
            pkb.storeRel({ "1", EntityType::While }, { "2", EntityType::Assign }, RelRefType::Next);
            pkb.storeRel({ "2", EntityType::Assign }, { "3", EntityType::Assign }, RelRefType::Next);
            pkb.storeRel({ "3", EntityType::Assign }, { "1", EntityType::While }, RelRefType::Next);

            const std::list<PKBEntityPair> results = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Assign, RelRefType::Next);
            Assert::IsTrue(results.size() == 1);
            for (auto pair = results.begin(); pair != results.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }
        }
        
        TEST_METHOD(nextTQuerierTest) {
            pkb.storeRel({ "1", EntityType::While }, { "2", EntityType::Assign }, RelRefType::Next);
            pkb.storeRel({ "2", EntityType::Assign }, { "3", EntityType::Assign }, RelRefType::Next);
            pkb.storeRel({ "3", EntityType::Assign }, { "1", EntityType::While }, RelRefType::Next);

            const std::list<PKBEntityPair> results1 = pkb.getLhsSynRhsSynForRel(EntityType::Assign, EntityType::Assign, RelRefType::NextT);
            Assert::IsTrue(results1.size() == 4);
            for (auto pair = results1.begin(); pair != results1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            const std::list<PKBEntity> results2 = pkb.getLhsRhsSameSynObj(EntityType::Assign, RelRefType::NextT);
            Assert::IsTrue(results2.size() == 2);
            for (auto entity = results2.begin(); entity != results2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }
        }

        TEST_METHOD(ifStorageTest) {
            pkb.addControlVariable(EntityType::If, 3, "x");
            pkb.addControlVariable(EntityType::If, 6, "y");
            pkb.addControlVariable(EntityType::If, 6, "z");
            pkb.addControlVariable(EntityType::If, 15, "t");
            pkb.addControlVariable(EntityType::If, 29, "x");
            pkb.addControlVariable(EntityType::If, 300, "x");
            pkb.addControlVariable(EntityType::If, 298, "veryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableName");
            std::list<PKBEntity> result = pkb.getPatternWildcard(EntityType::If);
            Assert::IsTrue(result.size() == 7);
        }

        TEST_METHOD(whileStorageTest) {
            pkb.addControlVariable(EntityType::While, 3, "x");
            pkb.addControlVariable(EntityType::While, 6, "y");
            pkb.addControlVariable(EntityType::While, 6, "z");
            pkb.addControlVariable(EntityType::While, 15, "t");
            pkb.addControlVariable(EntityType::While, 29, "x");
            pkb.addControlVariable(EntityType::While, 300, "x");
            pkb.addControlVariable(EntityType::While, 298, "veryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableName");
            std::list<PKBEntity> result = pkb.getPatternWildcard(EntityType::While);
            Assert::IsTrue(result.size() == 7);
        }

        TEST_METHOD(affectsQuerierTest) {

            PKBEntity stmt1 = { "1", EntityType::While };
            PKBEntity stmt2 = { "2", EntityType::Assign };
            PKBEntity varX = { "x", EntityType::Variable };
            PKBEntity proc1 = { "proc1", EntityType::Procedure };

            pkb.addStatement(1, EntityType::While);
            pkb.addStatement(2, EntityType::Assign);

            CFGRoot* root1 = new CFGRoot(proc1.val);
            CFGWhile* node1 = new CFGWhile(root1);
            node1->addStmtNumber(1);
            CFGNode* node2 = new CFGNode(root1);
            node2->addStmtNumber(2);
            root1->setNext(node1);
            node1->setWhile(node2);
            node2->setNext(node1);

            root1->setLast(2);

            std::list<CFGRoot*> roots = { root1 };

            pkb.storeCFG(roots);
            pkb.storeRel(stmt1, varX, RelRefType::Uses);
            pkb.storeRel(stmt2, varX, RelRefType::Uses);
            pkb.storeRel(stmt2, varX, RelRefType::Modifies);

            std::list<PKBEntityPair> result1 = pkb.getLhsSynRhsSynForRel(EntityType::Stmt, EntityType::Stmt, RelRefType::Affects);
            Assert::IsTrue(result1.size() == 1);

            std::list<PKBEntity> result2 = pkb.getLhsRhsSameSynObj(EntityType::Stmt, RelRefType::Affects);
            Assert::IsTrue(result2.size() == 1);

            bool result3 = pkb.hasLhsWildcardRhsWildcardForRel(RelRefType::Affects);
            Assert::IsTrue(result3);
        }

        TEST_METHOD(affectsTQuerierTest) {

            PKBEntity stmt1 = { "1", EntityType::While };
            PKBEntity stmt2 = { "2", EntityType::Assign };
            PKBEntity stmt3 = { "3", EntityType::Assign };
            PKBEntity varX = { "x", EntityType::Variable };
            PKBEntity proc1 = { "proc1", EntityType::Procedure };

            pkb.addStatement(1, EntityType::While);
            pkb.addStatement(2, EntityType::Assign);
            pkb.addStatement(3, EntityType::Assign);

            CFGRoot* root1 = new CFGRoot(proc1.val);
            CFGWhile* node1 = new CFGWhile(root1);
            node1->addStmtNumber(1);
            CFGNode* node2 = new CFGNode(root1);
            node2->addStmtNumber(2);
            node2->addStmtNumber(3);
            root1->setNext(node1);
            node1->setWhile(node2);
            node2->setNext(node1);

            root1->setLast(3);

            std::list<CFGRoot*> roots = { root1 };

            pkb.storeCFG(roots);
            pkb.storeRel(stmt1, varX, RelRefType::Uses);
            pkb.storeRel(stmt2, varX, RelRefType::Uses);
            pkb.storeRel(stmt2, varX, RelRefType::Modifies);
            pkb.storeRel(stmt3, varX, RelRefType::Uses);
            pkb.storeRel(stmt3, varX, RelRefType::Modifies);

            std::list<PKBEntityPair> result1 = pkb.getLhsSynRhsSynForRel(EntityType::Stmt, EntityType::Stmt, RelRefType::AffectsT);
            Assert::IsTrue(result1.size() == 4);

            std::list<PKBEntity> result2 = pkb.getLhsRhsSameSynObj(EntityType::Stmt, RelRefType::AffectsT);
            Assert::IsTrue(result2.size() == 2);

            bool result3 = pkb.hasLhsWildcardRhsWildcardForRel(RelRefType::AffectsT);
            Assert::IsTrue(result3);

            std::list<PKBEntity> result4 = pkb.getLhsConstantRhsSynForRel({"2", QueryConstantType::Number}, EntityType::Assign, RelRefType::AffectsT);
            Assert::IsTrue(result4.size() == 2);

            std::list<PKBEntity> result5 = pkb.getLhsSynRhsConstantForRel(EntityType::Assign, { "2", QueryConstantType::Number }, RelRefType::AffectsT);
            Assert::IsTrue(result5.size() == 2);
        }

    };
}
