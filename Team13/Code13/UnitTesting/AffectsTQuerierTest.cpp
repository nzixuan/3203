#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "AffectsTQuerier.h"
#include "AffectsQuerier.h"
#include "CFGStorage.h"
#include "ModifiesStorage.h"
#include "UsesStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(AffectsTQuerierTest) {
        AffectsQuerier querier = AffectsQuerier();
        CFGStorage cfgStorage = CFGStorage();
        ModifiesStorage modifiesStorage = ModifiesStorage();
        UsesStorage usesStorage = UsesStorage();
        StmtToEntityMap stmtEntityMap;
        AffectsTQuerier affectsTQuerier = AffectsTQuerier(&querier);


        TEST_METHOD_INITIALIZE(initializeAllStorages) {
            querier.setStorages(&cfgStorage, &modifiesStorage, &usesStorage, &stmtEntityMap);
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
            std::list<PKBEntityPair> result1 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 13);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result2 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Assign);
            Assert::IsTrue(result2.size() == 13);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result3 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Stmt);
            Assert::IsTrue(result3.size() == 13);
            for (auto pair = result3.begin(); pair != result3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result4 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Assign);
            Assert::IsTrue(result4.size() == 13);
            for (auto pair = result4.begin(); pair != result4.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> noResults1 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Assign, EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntityPair> noResults2 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntityPair> noResults3 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Read, EntityType::If);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntityPair> noResults4 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntityPair> noResults5 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Call);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntityPair> noResults6 = affectsTQuerier.getLhsSynRhsSyn(EntityType::Call, EntityType::Assign);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntityPair> noResults7 = affectsTQuerier.getLhsSynRhsSyn(EntityType::While, EntityType::Assign);
            Assert::IsTrue(noResults7.size() == 0);
        }
        TEST_METHOD(testGetLhsSynRhsSameSynObj) {
            std::list<PKBEntity> result1 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 3);

            std::list<PKBEntity> result2 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Assign);
            Assert::IsTrue(result2.size() == 3);

            std::list<PKBEntity> noResults1 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = affectsTQuerier.getLhsRhsSameSynObj(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }
        TEST_METHOD(testGetLhsSynRhsWildcard) {
            std::list<PKBEntity> result1 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 8);

            std::list<PKBEntity> result2 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Assign);
            Assert::IsTrue(result2.size() == 8);

            std::list<PKBEntity> noResults1 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = affectsTQuerier.getLhsSynRhsWildcard(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "9", QueryConstantType::Number });
            Assert::IsTrue(result1.size() == 2);

            std::list<PKBEntity> result2 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Assign, { "10", QueryConstantType::Number });
            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> result3 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "16", QueryConstantType::Number });
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Assign, { "17", QueryConstantType::Number });
            Assert::IsTrue(result4.size() == 2);

            std::list<PKBEntity> result5 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "19", QueryConstantType::Number });
            Assert::IsTrue(result5.size() == 2);

            std::list<PKBEntity> result6 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Assign, { "20", QueryConstantType::Number });
            Assert::IsTrue(result6.size() == 2);

            std::list<PKBEntity> result7 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "21", QueryConstantType::Number });
            Assert::IsTrue(result7.size() == 2);

            std::list<PKBEntity> noResults1 = affectsTQuerier.getLhsSynRhsConstant(EntityType::While, { "9", QueryConstantType::Number });
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "1", QueryConstantType::Number });
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Assign, { "2", QueryConstantType::Number });
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "3", QueryConstantType::Number });
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Assign, { "4", QueryConstantType::Number });
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "14", QueryConstantType::Number });
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Read, { "17", QueryConstantType::Number });
            Assert::IsTrue(noResults7.size() == 0);

            std::list<PKBEntity> noResults8 = affectsTQuerier.getLhsSynRhsConstant(EntityType::If, { "20", QueryConstantType::Number });
            Assert::IsTrue(noResults8.size() == 0);

            std::list<PKBEntity> noResults9 = affectsTQuerier.getLhsSynRhsConstant(EntityType::Print, { "21", QueryConstantType::Number });
            Assert::IsTrue(noResults9.size() == 0);
        }

        TEST_METHOD(testGetLhsWildcardRhsSyn) {
            std::list<PKBEntity> result1 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Stmt);
            Assert::IsTrue(result1.size() == 8);

            std::list<PKBEntity> result2 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Assign);
            Assert::IsTrue(result2.size() == 8);

            std::list<PKBEntity> noResults1 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::If);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::While);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Call);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Print);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Read);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Variable);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = affectsTQuerier.getLhsWildcardRhsSyn(EntityType::Procedure);
            Assert::IsTrue(noResults7.size() == 0);
        }

        TEST_METHOD(testHasLhsWildcardRhsWildcard) {
            bool result = affectsTQuerier.hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        }
        TEST_METHOD(testHasLhsWildcardRhsConstant) {
            bool result1 = affectsTQuerier.hasLhsWildcardRhsConstant({ "9", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = affectsTQuerier.hasLhsWildcardRhsConstant({ "10", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = affectsTQuerier.hasLhsWildcardRhsConstant({ "16", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = affectsTQuerier.hasLhsWildcardRhsConstant({ "17", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = affectsTQuerier.hasLhsWildcardRhsConstant({ "19", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = affectsTQuerier.hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = affectsTQuerier.hasLhsWildcardRhsConstant({ "21", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = affectsTQuerier.hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = affectsTQuerier.hasLhsWildcardRhsConstant({ "2", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = affectsTQuerier.hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = affectsTQuerier.hasLhsWildcardRhsConstant({ "4", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = affectsTQuerier.hasLhsWildcardRhsConstant({ "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = affectsTQuerier.hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = affectsTQuerier.hasLhsWildcardRhsConstant({ "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = affectsTQuerier.hasLhsWildcardRhsConstant({ "15", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);
        }
        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = affectsTQuerier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 1);

            std::list<PKBEntity> result2 = affectsTQuerier.getLhsConstantRhsSyn({ "4", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> result3 = affectsTQuerier.getLhsConstantRhsSyn({ "7", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = affectsTQuerier.getLhsConstantRhsSyn({ "11", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result4.size() == 2);

            std::list<PKBEntity> result5 = affectsTQuerier.getLhsConstantRhsSyn({ "16", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result5.size() == 1);

            std::list<PKBEntity> result6 = affectsTQuerier.getLhsConstantRhsSyn({ "19", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result6.size() == 3);

            std::list<PKBEntity> result7 = affectsTQuerier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result7.size() == 3);

            std::list<PKBEntity> result8 = affectsTQuerier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result8.size() == 3);

            std::list<PKBEntity> noResults1 = affectsTQuerier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Read);
            Assert::IsTrue(noResults1.size() == 0);

            std::list<PKBEntity> noResults2 = affectsTQuerier.getLhsConstantRhsSyn({ "20", QueryConstantType::Number }, EntityType::Print);
            Assert::IsTrue(noResults2.size() == 0);

            std::list<PKBEntity> noResults3 = affectsTQuerier.getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::Print);
            Assert::IsTrue(noResults3.size() == 0);

            std::list<PKBEntity> noResults4 = affectsTQuerier.getLhsConstantRhsSyn({ "9", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults4.size() == 0);

            std::list<PKBEntity> noResults5 = affectsTQuerier.getLhsConstantRhsSyn({ "10", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults5.size() == 0);

            std::list<PKBEntity> noResults6 = affectsTQuerier.getLhsConstantRhsSyn({ "14", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults6.size() == 0);

            std::list<PKBEntity> noResults7 = affectsTQuerier.getLhsConstantRhsSyn({ "15", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults7.size() == 0);

            std::list<PKBEntity> noResults8 = affectsTQuerier.getLhsConstantRhsSyn({ "17", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(noResults8.size() == 0);

            std::list<PKBEntity> noResults9 = affectsTQuerier.getLhsConstantRhsSyn({ "21", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(noResults9.size() == 0);
        }

        TEST_METHOD(testHasLhsConstantRhsWildcard) {
            bool result1 = affectsTQuerier.hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = affectsTQuerier.hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = affectsTQuerier.hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = affectsTQuerier.hasLhsConstantRhsWildcard({ "11", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = affectsTQuerier.hasLhsConstantRhsWildcard({ "16", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = affectsTQuerier.hasLhsConstantRhsWildcard({ "19", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = affectsTQuerier.hasLhsConstantRhsWildcard({ "20", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = affectsTQuerier.hasLhsConstantRhsWildcard({ "9", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = affectsTQuerier.hasLhsConstantRhsWildcard({ "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = affectsTQuerier.hasLhsConstantRhsWildcard({ "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = affectsTQuerier.hasLhsConstantRhsWildcard({ "17", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = affectsTQuerier.hasLhsConstantRhsWildcard({ "12", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = affectsTQuerier.hasLhsConstantRhsWildcard({ "6", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = affectsTQuerier.hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = affectsTQuerier.hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);

            bool invalidResult9 = affectsTQuerier.hasLhsConstantRhsWildcard({ "18", QueryConstantType::Number });
            Assert::IsFalse(invalidResult9);

            bool invalidResult10 = affectsTQuerier.hasLhsConstantRhsWildcard({ "21", QueryConstantType::Number });
            Assert::IsFalse(invalidResult10);
        };

        TEST_METHOD(testHasLhsConstantRhsConstant) {
            bool result1 = affectsTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = affectsTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = affectsTQuerier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = affectsTQuerier.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "16", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = affectsTQuerier.hasLhsConstantRhsConstant({ "16", QueryConstantType::Number }, { "17", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = affectsTQuerier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "20", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = affectsTQuerier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "21", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = affectsTQuerier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = affectsTQuerier.hasLhsConstantRhsConstant({ "23", QueryConstantType::Number }, { "23", QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool result10 = affectsTQuerier.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "17", QueryConstantType::Number });
            Assert::IsTrue(result10);

            bool result11 = affectsTQuerier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsTrue(result11);

            bool result12 = affectsTQuerier.hasLhsConstantRhsConstant({ "19", QueryConstantType::Number }, { "21", QueryConstantType::Number });
            Assert::IsTrue(result12);

            bool result13 = affectsTQuerier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "20", QueryConstantType::Number });
            Assert::IsTrue(result13);

            bool invalidResult1 = affectsTQuerier.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsFalse(invalidResult1);

            bool invalidResult2 = affectsTQuerier.hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult2);

            bool invalidResult3 = affectsTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult3);

            bool invalidResult4 = affectsTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult4);

            bool invalidResult5 = affectsTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsFalse(invalidResult5);

            bool invalidResult6 = affectsTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsFalse(invalidResult6);

            bool invalidResult7 = affectsTQuerier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsFalse(invalidResult7);

            bool invalidResult8 = affectsTQuerier.hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsFalse(invalidResult8);

            bool invalidResult9 = affectsTQuerier.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "14", QueryConstantType::Number });
            Assert::IsFalse(invalidResult9);

            bool invalidResult10 = affectsTQuerier.hasLhsConstantRhsConstant({ "14", QueryConstantType::Number }, { "16", QueryConstantType::Number });
            Assert::IsFalse(invalidResult10);

            bool invalidResult11 = affectsTQuerier.hasLhsConstantRhsConstant({ "14", QueryConstantType::Number }, { "17", QueryConstantType::Number });
            Assert::IsFalse(invalidResult11);

            bool invalidResult12 = affectsTQuerier.hasLhsConstantRhsConstant({ "16", QueryConstantType::Number }, { "19", QueryConstantType::Number });
            Assert::IsFalse(invalidResult12);

            bool invalidResult13 = affectsTQuerier.hasLhsConstantRhsConstant({ "20", QueryConstantType::Number }, { "18", QueryConstantType::Number });
            Assert::IsFalse(invalidResult13);
        }
    };

};

