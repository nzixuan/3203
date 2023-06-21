#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "AffectsQuerier.h"
#include "CFGStorage.h"
#include "ModifiesStorage.h"
#include "UsesStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(AffectsQuerierTest) {
        AffectsQuerier querier = AffectsQuerier();
        CFGStorage cfgStorage = CFGStorage();
        ModifiesStorage modifiesStorage = ModifiesStorage();
        UsesStorage usesStorage = UsesStorage();
        StmtToEntityMap stmtEntityMap;


        TEST_METHOD_INITIALIZE(initializeAllStorages) {
            querier.setStorages(&cfgStorage, &modifiesStorage, &usesStorage , &stmtEntityMap);
            cfgStorage.setStmtEntityMap(&stmtEntityMap);

            PKBEntity stmt1 = { "1", EntityType::Assign };
            PKBEntity stmt2 = { "2", EntityType::While };
            PKBEntity stmt3 = { "3", EntityType::Call };
            PKBEntity stmt4 = { "4", EntityType::Assign };
            PKBEntity stmt5 = { "5", EntityType::If };
            PKBEntity stmt6 = { "6", EntityType::Read };
            PKBEntity stmt7 = { "7", EntityType::Assign };
            PKBEntity stmt8 = { "8", EntityType::Print };
            PKBEntity stmt9 = { "9", EntityType::Assign };
            PKBEntity stmt10 = { "10", EntityType::Assign };

            PKBEntity stmt11 = { "11", EntityType::Assign };
            PKBEntity stmt12 = { "12", EntityType::If };
            PKBEntity stmt13 = { "13", EntityType::Read };
            PKBEntity stmt14 = { "14", EntityType::Assign };
            PKBEntity stmt15 = { "15", EntityType::Print };
            PKBEntity stmt16 = { "16", EntityType::Assign };
            PKBEntity stmt17 = { "17", EntityType::Assign };

            PKBEntity stmt18 = { "18", EntityType::While };
            PKBEntity stmt19 = { "19", EntityType::Assign };
            PKBEntity stmt20 = { "20", EntityType::Assign };
            PKBEntity stmt21 = { "21", EntityType::Assign };

            PKBEntity stmt22 = { "22", EntityType::While };
            PKBEntity stmt23 = { "23", EntityType::Assign };

            PKBEntity varX = { "x", EntityType::Variable };
            PKBEntity varY = { "y", EntityType::Variable };
            PKBEntity varZ = { "z", EntityType::Variable };

            PKBEntity proc1 = { "proc1", EntityType::Procedure };
            PKBEntity proc2 = { "proc2", EntityType::Procedure };
            PKBEntity proc3 = { "proc3", EntityType::Procedure };
            PKBEntity proc4 = { "proc4", EntityType::Procedure };

            stmtEntityMap.insert({ 1, stmt1 });
            stmtEntityMap.insert({ 2, stmt2 });
            stmtEntityMap.insert({ 3, stmt3 });
            stmtEntityMap.insert({ 4, stmt4 });
            stmtEntityMap.insert({ 5, stmt5 });
            stmtEntityMap.insert({ 6, stmt6 });
            stmtEntityMap.insert({ 7, stmt7 });
            stmtEntityMap.insert({ 8, stmt8 });
            stmtEntityMap.insert({ 9, stmt9 });
            stmtEntityMap.insert({ 10, stmt10 });

            stmtEntityMap.insert({ 11, stmt11 });
            stmtEntityMap.insert({ 12, stmt12 });
            stmtEntityMap.insert({ 13, stmt13 });
            stmtEntityMap.insert({ 14, stmt14 });
            stmtEntityMap.insert({ 15, stmt15 });
            stmtEntityMap.insert({ 16, stmt16 });
            stmtEntityMap.insert({ 17, stmt17 });

            stmtEntityMap.insert({ 18, stmt18 });
            stmtEntityMap.insert({ 19, stmt19 });
            stmtEntityMap.insert({ 20, stmt20 });
            stmtEntityMap.insert({ 21, stmt21 });

            stmtEntityMap.insert({ 22, stmt22 });
            stmtEntityMap.insert({ 23, stmt23 });

            CFGRoot* root1 = new CFGRoot(proc1.val);
            CFGNode* node1 = new CFGNode(root1);
            node1->addStmtNumber(1);
            CFGWhile* node2 = new CFGWhile(root1);
            node2->addStmtNumber(2);
            CFGNode* node34 = new CFGNode(root1);
            node34->addStmtNumber(3);
            node34->addStmtNumber(4);
            CFGIf* node5 = new CFGIf(root1);
            node5->addStmtNumber(5);
            CFGNode* node67 = new CFGNode(root1);
            node67->addStmtNumber(6);
            node67->addStmtNumber(7);
            CFGNode* node89 = new CFGNode(root1);
            node89->addStmtNumber(8);
            node89->addStmtNumber(9);
            CFGNode* node10 = new CFGNode(root1);
            node10->addStmtNumber(10);

            root1->setNext(node1);
            node1->setNext(node2);
            node2->setWhile(node34);
            node34->setNext(node2);
            node2->setNext(node5);
            node5->setThen(node67);
            node5->setElse(node89);
            node5->setNext(node10);
            node67->setNext(node10);
            node89->setNext(node10);

            CFGRoot* root2 = new CFGRoot(proc2.val);
            CFGNode* node11 = new CFGNode(root2);
            node11->addStmtNumber(11);
            CFGIf* node12 = new CFGIf(root2);
            node12->addStmtNumber(12);
            CFGNode* node1314 = new CFGNode(root2);
            node1314->addStmtNumber(13);
            node1314->addStmtNumber(14);
            CFGNode* node1516 = new CFGNode(root2);
            node1516->addStmtNumber(15);
            node1516->addStmtNumber(16);
            CFGNode* node17 = new CFGNode(root2);
            node17->addStmtNumber(17);

            root2->setNext(node11);
            node11->setNext(node12);
            node12->setElse(node1314);
            node12->setThen(node1516);
            node12->setNext(node17);
            node1314->setNext(node17);
            node1516->setNext(node17);

            CFGRoot* root3 = new CFGRoot(proc3.val);
            CFGWhile* node18 = new CFGWhile(root3);
            node18->addStmtNumber(18);
            CFGNode* node192021 = new CFGNode(root3);
            node192021->addStmtNumber(19);
            node192021->addStmtNumber(20);
            node192021->addStmtNumber(21);
            root3->setNext(node18);
            node18->setWhile(node192021);
            node192021->setNext(node18);

            CFGRoot* root4 = new CFGRoot(proc4.val);
            CFGWhile* node22 = new CFGWhile(root4);
            node22->addStmtNumber(22);
            CFGNode* node23 = new CFGNode(root4);
            node23->addStmtNumber(23);
            root4->setNext(node22);
            node22->setWhile(node23);
            node23->setNext(node22);

            root1->setLast(10);
            root2->setLast(17);
            root3->setLast(21);
            root4->setLast(23);

            std::list<CFGRoot*> roots = { root1, root2, root3, root4 };

            cfgStorage.storeCfg(roots);

            modifiesStorage.set(stmt1, varX);
            modifiesStorage.set(stmt2, varX);
            modifiesStorage.set(stmt2, varY);
            modifiesStorage.set(stmt3, varX);
            modifiesStorage.set(stmt3, varY);
            modifiesStorage.set(stmt3, varZ);
            modifiesStorage.set(stmt4, varX);
            modifiesStorage.set(stmt5, varX);
            modifiesStorage.set(stmt5, varY);
            modifiesStorage.set(stmt6, varX);
            modifiesStorage.set(stmt7, varY);
            modifiesStorage.set(stmt9, varX);
            modifiesStorage.set(stmt10, varY);
            modifiesStorage.set(proc1, varX);
            modifiesStorage.set(proc1, varY);

            modifiesStorage.set(stmt11, varY);
            modifiesStorage.set(stmt12, varY);
            modifiesStorage.set(stmt12, varZ);
            modifiesStorage.set(stmt13, varY);
            modifiesStorage.set(stmt14, varY);
            modifiesStorage.set(stmt16, varZ);
            modifiesStorage.set(stmt17, varX);
            modifiesStorage.set(proc2, varX);
            modifiesStorage.set(proc2, varY);
            modifiesStorage.set(proc2, varZ);

            modifiesStorage.set(stmt18, varY);
            modifiesStorage.set(stmt18, varZ);
            modifiesStorage.set(stmt19, varY);
            modifiesStorage.set(stmt20, varZ);
            modifiesStorage.set(stmt21, varY);
            modifiesStorage.set(proc3, varY);
            modifiesStorage.set(proc3, varZ);

            modifiesStorage.set(stmt22, varX);
            modifiesStorage.set(stmt23, varX);
            modifiesStorage.set(proc4, varX);

            usesStorage.set(stmt1, varX);
            usesStorage.set(stmt2, varX);
            usesStorage.set(stmt3, varY);
            usesStorage.set(stmt3, varZ);
            usesStorage.set(stmt4, varX);
            usesStorage.set(stmt5, varX);
            usesStorage.set(stmt7, varX);
            usesStorage.set(stmt8, varX);
            usesStorage.set(stmt9, varX);
            usesStorage.set(stmt9, varY);
            usesStorage.set(stmt10, varY);
            usesStorage.set(proc1, varX);
            usesStorage.set(proc1, varY);

            usesStorage.set(stmt12, varY);
            usesStorage.set(stmt14, varY);
            usesStorage.set(stmt15, varY);
            usesStorage.set(stmt16, varY);
            usesStorage.set(stmt17, varZ);
            usesStorage.set(proc2, varY);
            usesStorage.set(proc2, varZ);

            usesStorage.set(stmt18, varZ);
            usesStorage.set(stmt19, varZ);
            usesStorage.set(stmt20, varY);
            usesStorage.set(stmt21, varZ);
            usesStorage.set(proc3, varZ);

            usesStorage.set(stmt22, varX);
            usesStorage.set(stmt23, varX);
            usesStorage.set(proc4, varX);
        }

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = querier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 9);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result2 = querier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Assign);
            Assert::IsTrue(result2.size() == 9);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result3 = querier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Stmt);
            Assert::IsTrue(result3.size() == 9);
            for (auto pair = result3.begin(); pair != result3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result4 = querier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Assign);
            Assert::IsTrue(result4.size() == 9);
            for (auto pair = result4.begin(); pair != result4.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> noResults1 = querier.getLhsSynRhsSyn(EntityType::Assign, EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntityPair> noResults2 = querier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntityPair> noResults3 = querier.getLhsSynRhsSyn(EntityType::Read, EntityType::If);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntityPair> noResults4 = querier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntityPair> noResults5 = querier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Call);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntityPair> noResults6 = querier.getLhsSynRhsSyn(EntityType::Call, EntityType::Assign);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntityPair> noResults7 = querier.getLhsSynRhsSyn(EntityType::While, EntityType::Assign);
            Assert::IsTrue(noResults7.size() == 0);
        }
        TEST_METHOD(testGetLhsSynRhsSameSynObj) {
            std::list<PKBEntity> result1 = querier.getLhsRhsSameSynObj(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 1);

            std::list<PKBEntity> result2 = querier.getLhsRhsSameSynObj(EntityType::Assign);
            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> noResults1 = querier.getLhsRhsSameSynObj(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = querier.getLhsRhsSameSynObj(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = querier.getLhsRhsSameSynObj(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = querier.getLhsRhsSameSynObj(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = querier.getLhsRhsSameSynObj(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = querier.getLhsRhsSameSynObj(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = querier.getLhsRhsSameSynObj(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }
        TEST_METHOD(testGetLhsSynRhsWildcard) {
            std::list<PKBEntity> result1 = querier.getLhsSynRhsWildcard(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 8);

            std::list<PKBEntity> result2 = querier.getLhsSynRhsWildcard(EntityType::Assign);
            Assert::IsTrue(result2.size() == 8);

            std::list<PKBEntity> noResults1 = querier.getLhsSynRhsWildcard(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = querier.getLhsSynRhsWildcard(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = querier.getLhsSynRhsWildcard(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = querier.getLhsSynRhsWildcard(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = querier.getLhsSynRhsWildcard(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = querier.getLhsSynRhsWildcard(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = querier.getLhsSynRhsWildcard(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "9", QueryConstantType::Number });
            Assert::IsTrue(result1.size() == 2);

            std::list<PKBEntity> result2 = querier.getLhsSynRhsConstant(EntityType::Assign, { "10", QueryConstantType::Number });
            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> result3 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "16", QueryConstantType::Number });
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = querier.getLhsSynRhsConstant(EntityType::Assign, { "17", QueryConstantType::Number });
            Assert::IsTrue(result4.size() == 1);

            std::list<PKBEntity> result5 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "19", QueryConstantType::Number });
            Assert::IsTrue(result5.size() == 1);

            std::list<PKBEntity> result6 = querier.getLhsSynRhsConstant(EntityType::Assign, { "20", QueryConstantType::Number });
            Assert::IsTrue(result6.size() == 1);

            std::list<PKBEntity> result7 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "21", QueryConstantType::Number });
            Assert::IsTrue(result7.size() == 1);

            std::list<PKBEntity> noResults1 = querier.getLhsSynRhsConstant(EntityType::While, { "9", QueryConstantType::Number });
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "1", QueryConstantType::Number });
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = querier.getLhsSynRhsConstant(EntityType::Assign, { "2", QueryConstantType::Number });
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "3", QueryConstantType::Number });
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = querier.getLhsSynRhsConstant(EntityType::Assign, { "4", QueryConstantType::Number });
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = querier.getLhsSynRhsConstant(EntityType::Stmt, { "14", QueryConstantType::Number });
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = querier.getLhsSynRhsConstant(EntityType::Read, { "17", QueryConstantType::Number });
            Assert::IsTrue(noResults7.size() == 0);

            std::list<PKBEntity> noResults8 = querier.getLhsSynRhsConstant(EntityType::If, { "20", QueryConstantType::Number });
            Assert::IsTrue(noResults8.size() == 0);

            std::list<PKBEntity> noResults9 = querier.getLhsSynRhsConstant(EntityType::Print, { "21", QueryConstantType::Number });
            Assert::IsTrue(noResults9.size() == 0);
        }

        TEST_METHOD(testGetLhsWildcardRhsSyn) {
            std::list<PKBEntity> result1 = querier.getLhsWildcardRhsSyn(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 8);

            std::list<PKBEntity> result2 = querier.getLhsWildcardRhsSyn(EntityType::Assign);
            Assert::IsTrue(result2.size() == 8);

            std::list<PKBEntity> noResults1 = querier.getLhsWildcardRhsSyn(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = querier.getLhsWildcardRhsSyn(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = querier.getLhsWildcardRhsSyn(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = querier.getLhsWildcardRhsSyn(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = querier.getLhsWildcardRhsSyn(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = querier.getLhsWildcardRhsSyn(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = querier.getLhsWildcardRhsSyn(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }

        TEST_METHOD(testHasLhsWildcardRhsWildcard) {
            bool result = querier.hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        }
        TEST_METHOD(testHasLhsWildcardRhsConstant) {
            bool result1 = querier.hasLhsWildcardRhsConstant({ "9", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = querier.hasLhsWildcardRhsConstant({ "10", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = querier.hasLhsWildcardRhsConstant({ "16", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = querier.hasLhsWildcardRhsConstant({ "17", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = querier.hasLhsWildcardRhsConstant({ "19", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = querier.hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = querier.hasLhsWildcardRhsConstant({ "21", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = querier.hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = querier.hasLhsWildcardRhsConstant({ "2", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = querier.hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = querier.hasLhsWildcardRhsConstant({ "4", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = querier.hasLhsWildcardRhsConstant({ "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = querier.hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = querier.hasLhsWildcardRhsConstant({ "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = querier.hasLhsWildcardRhsConstant({ "15", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);
        }
        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = querier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 1);

            std::list<PKBEntity> result2 = querier.getLhsConstantRhsSyn({ "4", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> result3 = querier.getLhsConstantRhsSyn({ "7", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = querier.getLhsConstantRhsSyn({ "11", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result4.size() == 1);

            std::list<PKBEntity> result5 = querier.getLhsConstantRhsSyn({ "16", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result5.size() == 1);

            std::list<PKBEntity> result6 = querier.getLhsConstantRhsSyn({ "19", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result6.size() == 1);

            std::list<PKBEntity> result7 = querier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result7.size() == 2);

            std::list<PKBEntity> result8 = querier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result8.size() == 2);

            std::list<PKBEntity> noResults1 = querier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Read);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = querier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Print);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = querier.getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::Print);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = querier.getLhsConstantRhsSyn({ "9", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = querier.getLhsConstantRhsSyn({ "10", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = querier.getLhsConstantRhsSyn({ "14", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = querier.getLhsConstantRhsSyn({ "15", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults7.size() == 0);

            std::list<PKBEntity> noResults8 = querier.getLhsConstantRhsSyn({ "17", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults8.size() == 0);

            std::list<PKBEntity> noResults9 = querier.getLhsConstantRhsSyn({ "21", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults9.size() == 0);
        }

        TEST_METHOD(testHasLhsConstantRhsWildcard) {
            bool result1 = querier.hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = querier.hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = querier.hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = querier.hasLhsConstantRhsWildcard({ "11", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = querier.hasLhsConstantRhsWildcard({ "16", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = querier.hasLhsConstantRhsWildcard({ "19", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = querier.hasLhsConstantRhsWildcard({ "20", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = querier.hasLhsConstantRhsWildcard({ "9", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = querier.hasLhsConstantRhsWildcard({ "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = querier.hasLhsConstantRhsWildcard({ "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = querier.hasLhsConstantRhsWildcard({ "17", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = querier.hasLhsConstantRhsWildcard({ "12", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = querier.hasLhsConstantRhsWildcard({ "6", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = querier.hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = querier.hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);

            bool invalidResult9 = querier.hasLhsConstantRhsWildcard({ "18", QueryConstantType::Number });
            Assert::IsFalse(invalidResult9);

            bool invalidResult10 = querier.hasLhsConstantRhsWildcard({ "21", QueryConstantType::Number });
            Assert::IsFalse(invalidResult10);
        };

        TEST_METHOD(testHasLhsConstantRhsConstant) {
            bool result1 = querier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = querier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = querier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = querier.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "16", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = querier.hasLhsConstantRhsConstant({ "16", QueryConstantType::Number }, { "17", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = querier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "20", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = querier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "21", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = querier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = querier.hasLhsConstantRhsConstant({ "23", QueryConstantType::Number }, { "23", QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool invalidResult1 = querier.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = querier.hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = querier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = querier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = querier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = querier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = querier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = querier.hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);

            bool invalidResult9 = querier.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult9);

            bool invalidResult10 = querier.hasLhsConstantRhsConstant({ "14", QueryConstantType::Number }, { "16", QueryConstantType::Number });
            Assert::IsFalse(invalidResult10);

            bool invalidResult11 = querier.hasLhsConstantRhsConstant({ "14", QueryConstantType::Number }, { "17", QueryConstantType::Number });
            Assert::IsFalse(invalidResult11);

            bool invalidResult12 = querier.hasLhsConstantRhsConstant({ "16", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsFalse(invalidResult12);

            bool invalidResult13 = querier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "21", QueryConstantType::Number });
            Assert::IsFalse(invalidResult13);

            bool invalidResult14 = querier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsFalse(invalidResult14);

            bool invalidResult15 = querier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "18", QueryConstantType::Number });
            Assert::IsFalse(invalidResult15);

            bool invalidResult16 = querier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "20", QueryConstantType::Number });
            Assert::IsFalse(invalidResult16);
        }
    };

};

