#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(NextStorageTest) {
        NextStorage nextStorage = NextStorage();
        SuchThatStorage* storage = &nextStorage;

        TEST_METHOD_INITIALIZE(initializeNextStorageWithEntityPairs) {

            const PKBEntity stmt1 = { "1", EntityType::Assign };
            const PKBEntity stmt2 = { "2", EntityType::Assign };
            const PKBEntity stmt3 = { "3", EntityType::While };
            const PKBEntity stmt4 = { "4", EntityType::If };
            const PKBEntity stmt5 = { "5", EntityType::Read };
            const PKBEntity stmt6 = { "6", EntityType::Call };
            const PKBEntity stmt7 = { "7", EntityType::Assign };
            const PKBEntity stmt8 = { "8", EntityType::Assign };
            const PKBEntity stmt9 = { "9", EntityType::While };
            const PKBEntity stmt10 = { "10", EntityType::Assign };
            const PKBEntity stmt11 = { "11", EntityType::While };
            const PKBEntity stmt12 = { "12", EntityType::Assign };
            const PKBEntity stmt13 = { "13", EntityType::Assign };

            storage->set(stmt1, stmt2);
            storage->set(stmt2, stmt3);
            storage->set(stmt3, stmt4);
            storage->set(stmt3, stmt7);
            storage->set(stmt4, stmt5);
            storage->set(stmt4, stmt6);
            storage->set(stmt5, stmt3);
            storage->set(stmt6, stmt3);
            storage->set(stmt7, stmt8);
            storage->set(stmt8, stmt9);
            storage->set(stmt9, stmt10);
            storage->set(stmt9, stmt11);
            storage->set(stmt10, stmt9);
            storage->set(stmt11, stmt12);
            storage->set(stmt12, stmt13);
            storage->set(stmt13, stmt11);
        }

        TEST_METHOD_CLEANUP(destroyNextStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddNextRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 13);
            Assert::IsTrue(storage->getRhsMap()->size() == 12);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 16);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::Assign, EntityType::Assign);

            Assert::IsTrue(result1.size() == 3);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::Assign);

            Assert::IsTrue(result2.size() == 3);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result3 = storage->getLhsSynRhsSyn(EntityType::Stmt, EntityType::While);

            Assert::IsTrue(result3.size() == 7);
        };

        TEST_METHOD(testGetLhsSynRhsWildCard) {

            std::list<PKBEntity> result1 = storage->getLhsSynRhsWildcard(EntityType::Stmt);

            Assert::IsTrue(result1.size() == 13);

            std::list<PKBEntity> result2 = storage->getLhsSynRhsWildcard(EntityType::While);

            Assert::IsTrue(result2.size() == 3);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage->getLhsSynRhsWildcard(EntityType::If);

            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {

            std::list<PKBEntity> result1 = storage->getLhsSynRhsConstant(EntityType::If, { "5", QueryConstantType::Number });

            Assert::IsTrue(result1.size() == 1);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsConstant(EntityType::Read, { "3", QueryConstantType::Number });

            Assert::IsTrue(result2.size() == 1);

            std::list<PKBEntity> result3 = storage->getLhsSynRhsConstant(EntityType::Call, { "3", QueryConstantType::Number });

            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = storage->getLhsSynRhsConstant(EntityType::Stmt, { "3", QueryConstantType::Number });

            Assert::IsTrue(result4.size() == 3);

        };

        TEST_METHOD(testGetLhsWildcardRhsSyn) {

            std::list<PKBEntity> result1 = storage->getLhsWildcardRhsSyn(EntityType::While);
            Assert::IsTrue(result1.size() == 3);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }


            std::list<PKBEntity> result2 = storage->getLhsWildcardRhsSyn(EntityType::Assign);
            Assert::IsTrue(result2.size() == 6);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }
        };

        TEST_METHOD(testGetLhsWildcardRhsWildcard) {
            bool result = storage->hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        };

        TEST_METHOD(testGetLhsWildcardRhsConstant) {
            bool result1 = storage->hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsWildcardRhsConstant({ "13", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsWildcardRhsConstant({ "7", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsWildcardRhsConstant({ "6",  QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsWildcardRhsConstant({ "2",  QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsWildcardRhsConstant({ "9", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = storage->hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsWildcardRhsConstant({ "3", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);
        };

        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = storage->getLhsConstantRhsSyn({ "3", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 2);

            std::list<PKBEntity> result2 = storage->getLhsConstantRhsSyn({ "3", QueryConstantType::Number }, EntityType::If);
            Assert::IsTrue(result2.size() == 1);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result3 = storage->getLhsConstantRhsSyn({ "9", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result3.size() == 2);

            std::list<PKBEntity> result4 = storage->getLhsConstantRhsSyn({ "9", QueryConstantType::Number }, EntityType::While);
            Assert::IsTrue(result4.size() == 1);

            std::list<PKBEntity> result5 = storage->getLhsConstantRhsSyn({ "11", QueryConstantType::Number }, EntityType::While);
            Assert::IsTrue(result5.size() == 0);
        };

        TEST_METHOD(testGetLhsConstantRhsWildcard) {
            bool result1 = storage->hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsWildcard({ "10", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsWildcard({ "6", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsConstantRhsWildcard({ "5", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsConstantRhsWildcard({ "13", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool invalidResult1 = storage->hasLhsConstantRhsWildcard({ "1", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsWildcard({ "13", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsWildcard({ "81", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage->hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "8", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "11",QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage->hasLhsConstantRhsConstant({ "10", QueryConstantType::Number }, { "9",QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = storage->hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "12",QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool result10 = storage->hasLhsConstantRhsConstant({ "13", QueryConstantType::Number }, { "11",QueryConstantType::Number });
            Assert::IsTrue(result10);

            bool invalidResult1 = storage->hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsConstant({ "13", QueryConstantType::Identifier }, { "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsConstant({ "10", QueryConstantType::Number }, { "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsConstantRhsConstant({ "12", QueryConstantType::Number }, { "13", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage->hasLhsConstantRhsConstant({ "12", QueryConstantType::Number }, { "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);
        };

    };

};

