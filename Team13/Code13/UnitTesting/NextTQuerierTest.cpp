#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextStorage.h"
#include "NextTQuerier.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(NextTQuerierTest) {
        NextStorage storage = NextStorage();
        NextTQuerier nextTQuerier = NextTQuerier(&storage);

        TEST_METHOD_INITIALIZE(initializeNextStorageWithEntityPairs) {

            const PKBEntity stmt1 = { "1", EntityType::If };
            const PKBEntity stmt2 = { "2", EntityType::Assign };
            const PKBEntity stmt3 = { "3", EntityType::While };
            const PKBEntity stmt4 = { "4", EntityType::Assign };
            const PKBEntity stmt5 = { "5", EntityType::If };
            const PKBEntity stmt6 = { "6", EntityType::Call };
            const PKBEntity stmt7 = { "7", EntityType::Read };
            const PKBEntity stmt8 = { "8", EntityType::Assign };
            const PKBEntity stmt9 = { "9", EntityType::Print };
            const PKBEntity stmt10 = { "10", EntityType::Assign };

            storage.set(stmt1, stmt2);
            storage.set(stmt1, stmt3);
            storage.set(stmt3, stmt4);
            storage.set(stmt4, stmt5);
            storage.set(stmt5, stmt6);
            storage.set(stmt5, stmt7);
            storage.set(stmt6, stmt8);
            storage.set(stmt7, stmt8);
            storage.set(stmt8, stmt3);
            storage.set(stmt3, stmt9);
            storage.set(stmt2, stmt10);
            storage.set(stmt9, stmt10);

        }

        TEST_METHOD_CLEANUP(destroyNextStorage) {
            storage.clear();
        }

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = nextTQuerier.getLhsSynRhsSyn(EntityType::While, EntityType::Stmt);
            Assert::IsTrue(result1.size() == 8);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While);
            }

            std::list<PKBEntityPair> result2 = nextTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);
            Assert::IsTrue(result2.size() == 59);

            std::list<PKBEntityPair> result3 = nextTQuerier.getLhsSynRhsSyn(EntityType::Assign, EntityType::Assign);
            Assert::IsTrue(result3.size() == 7);
            for (auto pair = result3.begin(); pair != result3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result4 = nextTQuerier.getLhsSynRhsSyn(EntityType::Call, EntityType::Read);
            Assert::IsTrue(result4.size() == 1);
            for (auto pair = result4.begin(); pair != result4.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Call && (pair->rhs).entityType == EntityType::Read);
            }

            std::list<PKBEntityPair> result5 = nextTQuerier.getLhsSynRhsSyn(EntityType::Stmt, EntityType::Print);
            Assert::IsTrue(result5.size() == 7);
            for (auto pair = result5.begin(); pair != result5.end(); pair++) {
                Assert::IsTrue( (pair->rhs).entityType == EntityType::Print);
            }

        };

        TEST_METHOD(testGetLhsSynRhsWildcard) {
            std::list<PKBEntity> result1 = nextTQuerier.getLhsSynRhsWildcard(EntityType::Assign);
            Assert::IsTrue(result1.size() == 3);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = nextTQuerier.getLhsSynRhsWildcard(EntityType::Stmt);
            Assert::IsTrue(result2.size() == 9);

            std::list<PKBEntity> result3 = nextTQuerier.getLhsSynRhsWildcard(EntityType::While);
            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result4 = nextTQuerier.getLhsSynRhsWildcard(EntityType::If);
            Assert::IsTrue(result4.size() == 2);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {

            std::list<PKBEntity> result1 = nextTQuerier.getLhsSynRhsConstant(EntityType::Assign, {"10", QueryConstantType::Number});
            Assert::IsTrue(result1.size() == 3);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = nextTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "10", QueryConstantType::Number });
            Assert::IsTrue(result2.size() == 9);

            std::list<PKBEntity> result3 = nextTQuerier.getLhsSynRhsConstant(EntityType::Call, { "6", QueryConstantType::Number });
            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Call);
            }

            std::list<PKBEntity> result4 = nextTQuerier.getLhsSynRhsConstant(EntityType::Print, { "9", QueryConstantType::Number });
            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntity> result5 = nextTQuerier.getLhsSynRhsConstant(EntityType::Stmt, { "3", QueryConstantType::Number });
            Assert::IsTrue(result5.size() == 7);
        };

        TEST_METHOD(testGetLhsWildcardRhsSyn) {

            std::list<PKBEntity> result1 = nextTQuerier.getLhsWildcardRhsSyn(EntityType::Assign);
            Assert::IsTrue(result1.size() == 4);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = nextTQuerier.getLhsWildcardRhsSyn(EntityType::Stmt);
            Assert::IsTrue(result2.size() == 9);

            std::list<PKBEntity> result3 = nextTQuerier.getLhsWildcardRhsSyn(EntityType::While);
            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result4 = nextTQuerier.getLhsWildcardRhsSyn(EntityType::If);
            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }
        };
        
        TEST_METHOD(testHasLhsWildcardRhsWildcard) {
            bool result = nextTQuerier.hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        }

        TEST_METHOD(testHasLhsWildcardRhsConstant) {
            bool result1 = nextTQuerier.hasLhsWildcardRhsConstant({ "2", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = nextTQuerier.hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = nextTQuerier.hasLhsWildcardRhsConstant({ "4", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = nextTQuerier.hasLhsWildcardRhsConstant({ "5", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = nextTQuerier.hasLhsWildcardRhsConstant({ "6", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = nextTQuerier.hasLhsWildcardRhsConstant({ "7", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = nextTQuerier.hasLhsWildcardRhsConstant({ "8", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = nextTQuerier.hasLhsWildcardRhsConstant({ "9", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = nextTQuerier.hasLhsWildcardRhsConstant({ "10", QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool invalidResult1 = nextTQuerier.hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = nextTQuerier.hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = nextTQuerier.hasLhsWildcardRhsConstant({ "3", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = nextTQuerier.hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);
        }

        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = nextTQuerier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Assign);
            Assert::IsTrue(result1.size() == 4);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = nextTQuerier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result2.size() == 9);

            std::list<PKBEntity> result3 = nextTQuerier.getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::Stmt);
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = nextTQuerier.getLhsConstantRhsSyn({ "3", QueryConstantType::Number }, EntityType::If);
            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result5 = nextTQuerier.getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Print);
            Assert::IsTrue(result5.size() == 1);
            for (auto entity = result5.begin(); entity != result5.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Print);
            }
        }

        TEST_METHOD(testHasLhsConstantRhsWildcard) {
            bool result1 = nextTQuerier.hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = nextTQuerier.hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = nextTQuerier.hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = nextTQuerier.hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = nextTQuerier.hasLhsConstantRhsWildcard({ "5", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = nextTQuerier.hasLhsConstantRhsWildcard({ "6", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = nextTQuerier.hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = nextTQuerier.hasLhsConstantRhsWildcard({ "8", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = nextTQuerier.hasLhsConstantRhsWildcard({ "9", QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool invalidResult1 = nextTQuerier.hasLhsConstantRhsWildcard({ "10", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = nextTQuerier.hasLhsConstantRhsWildcard({ "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = nextTQuerier.hasLhsConstantRhsWildcard({ "3", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);
        }

        TEST_METHOD(testHasLhsConstantRhsConstant) {
            bool result1 = nextTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = nextTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = nextTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = nextTQuerier.hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = nextTQuerier.hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = nextTQuerier.hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = nextTQuerier.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "8",QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = nextTQuerier.hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "7",QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = nextTQuerier.hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "6",QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool result10 = nextTQuerier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "6",QueryConstantType::Number });
            Assert::IsTrue(result10);

            bool result11 = nextTQuerier.hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "7",QueryConstantType::Number });
            Assert::IsTrue(result11);

            bool result12 = nextTQuerier.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result12);

            bool result13 = nextTQuerier.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "5", QueryConstantType::Number });
            Assert::IsTrue(result13);

            bool result14 = nextTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result14);

            bool result15 = nextTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "9", QueryConstantType::Number });
            Assert::IsTrue(result15);

            bool result16 = nextTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(result15);

            bool invalidResult1 = nextTQuerier.hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = nextTQuerier.hasLhsConstantRhsConstant({ "2", QueryConstantType::Identifier }, { "3", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = nextTQuerier.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "5", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = nextTQuerier.hasLhsConstantRhsConstant({ "5", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = nextTQuerier.hasLhsConstantRhsConstant({ "8", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);
        }

        TEST_METHOD(testGetLhsRhsSameSynObj) {
            std::list<PKBEntity> result1 = nextTQuerier.getLhsRhsSameSynObj(EntityType::While);
            Assert::IsTrue(result1.size() == 1);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result2 = nextTQuerier.getLhsRhsSameSynObj(EntityType::Assign);
            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result3 = nextTQuerier.getLhsRhsSameSynObj(EntityType::Read);
            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Read);
            }

            std::list<PKBEntity> result4 = nextTQuerier.getLhsRhsSameSynObj(EntityType::If);
            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result5 = nextTQuerier.getLhsRhsSameSynObj(EntityType::Print);
            Assert::IsTrue(result5.size() == 0);
        };
    };
};

