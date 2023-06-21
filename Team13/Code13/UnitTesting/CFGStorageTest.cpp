#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CFGStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(CFGStorageTest) {

        StmtToEntityMap stmtEntityMap;
        CFGStorage cfgStorage = CFGStorage();

        TEST_METHOD_INITIALIZE(initializeCFGStorage) {

            PKBEntity stmt1 = { "1", EntityType::Assign };
            PKBEntity stmt2 = { "2", EntityType::While };
            PKBEntity stmt3 = { "3", EntityType::Call };
            PKBEntity stmt4 = { "4", EntityType::Read };
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

            cfgStorage.setStmtEntityMap(&stmtEntityMap);

            CFGRoot* root1 = new CFGRoot("proc1");
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

            CFGRoot* root2 = new CFGRoot("proc2");
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

            root1->setLast(10);
            root2->setLast(17);

            std::list<CFGRoot*> roots = { root1, root2 };

            cfgStorage.storeCfg(roots);
        }

        TEST_METHOD(testStoringCFG) {
            Assert::IsTrue(!cfgStorage.getRoots().empty());
        };

        TEST_METHOD(testIsIfNode) {
            CFGRoot rootStub = CFGRoot("stub");

            CFGIf ifNode1 = new CFGNode(&rootStub);
            ifNode1.addStmtNumber(5);
            CFGIf ifNode2 = new CFGNode(&rootStub);
            ifNode2.addStmtNumber(12);
            CFGNode node1 = new CFGNode(&rootStub);
            node1.addStmtNumber(3);
            node1.addStmtNumber(4);
            CFGWhile whileNode1 = new CFGNode(&rootStub);
            whileNode1.addStmtNumber(2);

            Assert::IsTrue(cfgStorage.isIfNode(&ifNode1));
            Assert::IsTrue(cfgStorage.isIfNode(&ifNode2));
            Assert::IsFalse(cfgStorage.isIfNode(&node1));
            Assert::IsFalse(cfgStorage.isIfNode(&whileNode1));
        };

        TEST_METHOD(testIsWhileNode) {
            CFGRoot rootStub = CFGRoot("stub");

            CFGIf ifNode1 = new CFGNode(&rootStub);
            ifNode1.addStmtNumber(5);
            CFGIf ifNode2 = new CFGNode(&rootStub);
            ifNode2.addStmtNumber(12);
            CFGNode node1 = new CFGNode(&rootStub);
            node1.addStmtNumber(3);
            node1.addStmtNumber(4);
            CFGWhile whileNode1 = new CFGNode(&rootStub);
            whileNode1.addStmtNumber(2);

            Assert::IsFalse(cfgStorage.isWhileNode(&ifNode1));
            Assert::IsFalse(cfgStorage.isWhileNode(&ifNode2));
            Assert::IsFalse(cfgStorage.isWhileNode(&node1));
            Assert::IsTrue(cfgStorage.isWhileNode(&whileNode1));
        };

        TEST_METHOD(testIsNodeVisited) {
            CFGRoot rootStub = CFGRoot("stub");

            std::unordered_set<int> visitedWhileSet;
            CFGWhile whileNode1 = new CFGNode(&rootStub);
            whileNode1.addStmtNumber(2);

            Assert::IsFalse(cfgStorage.isNodeVisited(&whileNode1, visitedWhileSet));

            visitedWhileSet.insert(whileNode1.getStmts().front());
            Assert::IsTrue(cfgStorage.isNodeVisited(&whileNode1, visitedWhileSet));
        };

        TEST_METHOD(testGetRoot) {
            CFGRoot* root1 = (CFGRoot*)cfgStorage.getRoot(1);
            Assert::IsTrue(root1->getProcedureName() == "proc1");

            CFGRoot* root2 = (CFGRoot*)cfgStorage.getRoot(10);
            Assert::IsTrue(root2->getProcedureName() == "proc1");

            CFGRoot* root3 = (CFGRoot*)cfgStorage.getRoot(11);
            Assert::IsTrue(root3->getProcedureName() == "proc2");

            CFGRoot* root4 = (CFGRoot*)cfgStorage.getRoot(17);
            Assert::IsTrue(root4->getProcedureName() == "proc2");

            CFGRoot* root5 = (CFGRoot*)cfgStorage.getRoot(18);
            Assert::IsTrue(root5 == NULL);

        }

        TEST_METHOD(testFindNode) {
            for (int i = 1; i <= 17; i++) {
                CFGAbstract* nodeStub = cfgStorage.findNode(i);
                Assert::IsTrue(nodeStub != NULL);
                std::vector<int> stmts = nodeStub->getStmts();
                auto iter = std::find(stmts.begin(), stmts.end(), i);
                Assert::IsTrue(iter != stmts.end());
            }
        }
    };

};

