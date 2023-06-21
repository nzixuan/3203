#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsTStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(FollowsTStorageTest) {

        FollowsTStorage storage = FollowsTStorage();

        TEST_METHOD_INITIALIZE(initializeFollowsTStorageWithEntityPairs) {

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

            storage.set(stmt1, stmt2);
            storage.set(stmt2, stmt3);
            storage.set(stmt5, stmt6);
            storage.set(stmt6, stmt7);
            storage.set(stmt7, stmt8);
            storage.set(stmt3, stmt9);
            storage.set(stmt10, stmt11);
            storage.set(stmt9, stmt13);
        }

        TEST_METHOD_CLEANUP(destroyFollowsTStorage) {
            storage.clear();
        }


        TEST_METHOD(testAddFollowRelationships) {
            Assert::IsTrue(storage.getLhsMap()->size() == 8);
            Assert::IsTrue(storage.getRhsMap()->size() == 8);
            Assert::IsTrue(storage.getEntityPairSet()->size() == 17);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage.getLhsSynRhsSyn(EntityType::Assign, EntityType::While);

            Assert::IsTrue(result1.size() == 5);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::While);
            }

            std::list<PKBEntityPair> result2 = storage.getLhsSynRhsSyn(EntityType::Assign, EntityType::Assign);

            Assert::IsTrue(result2.size() == 4);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result3 = storage.getLhsSynRhsSyn(EntityType::Read, EntityType::Call);

            Assert::IsTrue(result3.size() == 1);
            for (auto pair = result3.begin(); pair != result3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Read && (pair->rhs).entityType == EntityType::Call);
            }

            std::list<PKBEntityPair> result4 = storage.getLhsSynRhsSyn(EntityType::Call, EntityType::Read);
            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntityPair> result5 = storage.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);

            Assert::IsTrue(result5.size() == 17);

            std::list<PKBEntityPair> result6 = storage.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);

            Assert::IsTrue(result6.size() == 17);

            std::list<PKBEntityPair> result7 = storage.getLhsSynRhsSyn(EntityType::Assign, EntityType::Stmt);

            Assert::IsTrue(result7.size() == 9);

            std::list<PKBEntityPair> result8 = storage.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Assign);

            Assert::IsTrue(result8.size() == 10);
        };

        TEST_METHOD(testGetLhsSynRhsWildCard) {
            std::list<PKBEntity> result1 = storage.getLhsSynRhsWildcard(EntityType::Assign);

            Assert::IsTrue(result1.size() == 4);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage.getLhsSynRhsWildcard(EntityType::While);

            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage.getLhsSynRhsWildcard(EntityType::If);

            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage.getLhsSynRhsWildcard(EntityType::Stmt);

            Assert::IsTrue(result4.size() == 8);
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = storage.getLhsSynRhsConstant(EntityType::Assign, { "9", QueryConstantType::Number });

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage.getLhsSynRhsConstant(EntityType::While, { "13", QueryConstantType::Number });

            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage.getLhsSynRhsConstant(EntityType::Assign, { "12", QueryConstantType::Number });

            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage.getLhsSynRhsConstant(EntityType::Call, { "8", QueryConstantType::Number });

            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Call);
            }

            std::list<PKBEntity> result5 = storage.getLhsSynRhsConstant(EntityType::Stmt, { "13", QueryConstantType::Number });

            Assert::IsTrue(result5.size() == 4);

            std::list<PKBEntity> result6 = storage.getLhsSynRhsConstant(EntityType::Stmt, { "9", QueryConstantType::Number });

            Assert::IsTrue(result6.size() == 3);

            std::list<PKBEntity> result7 = storage.getLhsSynRhsConstant(EntityType::Stmt, { "8", QueryConstantType::Number });

            Assert::IsTrue(result7.size() == 3);
        };

        TEST_METHOD(testGetLhsWildcardRhsSyn) {
            std::list<PKBEntity> result1 = storage.getLhsWildcardRhsSyn(EntityType::Assign);

            Assert::IsTrue(result1.size() == 4);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage.getLhsWildcardRhsSyn(EntityType::While);

            Assert::IsTrue(result2.size() == 3);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage.getLhsWildcardRhsSyn(EntityType::Call);

            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Call);
            }

            std::list<PKBEntity> result4 = storage.getLhsWildcardRhsSyn(EntityType::If);

            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntity> result5 = storage.getLhsWildcardRhsSyn(EntityType::Stmt);

            Assert::IsTrue(result5.size() == 8);
        };

        TEST_METHOD(testGetLhsWildcardRhsWildcard) {
            bool result = storage.hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        };

        TEST_METHOD(testGetLhsWildcardRhsConstant) {
            bool result1 = storage.hasLhsWildcardRhsConstant({ "2", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage.hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage.hasLhsWildcardRhsConstant({ "6", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage.hasLhsWildcardRhsConstant({ "7", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage.hasLhsWildcardRhsConstant({ "8",  QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage.hasLhsWildcardRhsConstant({ "9",  QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage.hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage.hasLhsWildcardRhsConstant({ "13", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool invalidResult1 = storage.hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage.hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage.hasLhsWildcardRhsConstant({ "4", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage.hasLhsWildcardRhsConstant({ "5", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage.hasLhsWildcardRhsConstant({ "10", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);

            bool invalidResult6 = storage.hasLhsWildcardRhsConstant({ "12", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult6);
        };

        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = storage.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Assign );

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage.getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::While );

            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage.getLhsConstantRhsSyn({ "6", QueryConstantType::Number }, EntityType::Read );

            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage.getLhsConstantRhsSyn({ "10", QueryConstantType::Number }, EntityType::While );

            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result5 = storage.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Stmt);

            Assert::IsTrue(result5.size() == 4);

            std::list<PKBEntity> result6 = storage.getLhsConstantRhsSyn({ "13", QueryConstantType::Number }, EntityType::Stmt);

            Assert::IsTrue(result6.size() == 0);
        };

        TEST_METHOD(testGetLhsConstantRhsWildcard) {
            bool result1 = storage.hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage.hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage.hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage.hasLhsConstantRhsWildcard({ "5", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage.hasLhsConstantRhsWildcard({ "6",  QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage.hasLhsConstantRhsWildcard({ "7",  QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage.hasLhsConstantRhsWildcard({ "9", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage.hasLhsConstantRhsWildcard({ "10", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool invalidResult1 = storage.hasLhsConstantRhsWildcard({ "-1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage.hasLhsConstantRhsWildcard({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage.hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage.hasLhsConstantRhsWildcard({ "8", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage.hasLhsConstantRhsWildcard({ "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);

            bool invalidResult6 = storage.hasLhsConstantRhsWildcard({ "12", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult6);

            bool invalidResult7 = storage.hasLhsConstantRhsWildcard({ "13", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult7);
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "8", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "9",QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "9",QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = storage.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "13",QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool result10 = storage.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "13",QueryConstantType::Number });
            Assert::IsTrue(result10);

            bool invalidResult1 = storage.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage.hasLhsConstantRhsConstant({ "3", QueryConstantType::Identifier }, { "4", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "5", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage.hasLhsConstantRhsConstant({ "8", QueryConstantType::Number }, { "9", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage.hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);

            bool invalidResult6 = storage.hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "12", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult6);

            bool invalidResult7 = storage.hasLhsConstantRhsConstant({ "12", QueryConstantType::Number }, { "13", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult7);

            bool invalidResult8 = storage.hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult8);
        };
    };

};

