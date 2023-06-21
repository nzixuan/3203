#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(ParentTStorageTest) {

        ParentTStorage parentStorage = ParentTStorage();
        SuchThatStorage* storage = &parentStorage;

        TEST_METHOD_INITIALIZE(initializeParentTStorageWithEntityPairs) {

            const PKBEntity stmt2 = { "2", EntityType::While };
            const PKBEntity stmt3 = { "3", EntityType::Print };
            const PKBEntity stmt4 = { "4", EntityType::If };
            const PKBEntity stmt5 = { "5", EntityType::Assign };
            const PKBEntity stmt6 = { "6", EntityType::If };
            const PKBEntity stmt7 = { "7", EntityType::While };
            const PKBEntity stmt8 = { "8", EntityType::Assign };
            const PKBEntity stmt9 = { "9", EntityType::Read };
            const PKBEntity stmt10 = { "10", EntityType::While };
            const PKBEntity stmt11 = { "11", EntityType::If };
            const PKBEntity stmt12 = { "12", EntityType::While };
            
            storage->set(stmt2, stmt3);
            storage->set(stmt2, stmt4);
            storage->set(stmt4, stmt5);
            storage->set(stmt2, stmt6);
            storage->set(stmt6, stmt7);
            storage->set(stmt7, stmt8);
            storage->set(stmt2, stmt9);
            storage->set(stmt2, stmt10);
            storage->set(stmt10, stmt11);
            storage->set(stmt11, stmt12);

        }

        TEST_METHOD_CLEANUP(destroyParentTStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddParentRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 6);
            Assert::IsTrue(storage->getRhsMap()->size() == 10);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 17);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::Assign);

            Assert::IsTrue(result1.size() == 3);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::If, EntityType::Assign);

            Assert::IsTrue(result2.size() == 2);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::If && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result3 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::While);

            Assert::IsTrue(result3.size() == 4);
            for (auto pair = result3.begin(); pair != result3.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::While);
            }

            std::list<PKBEntityPair> result4 = storage->getLhsSynRhsSyn(EntityType::If, EntityType::Print);
            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntityPair> result5 = storage->getLhsSynRhsSyn(EntityType::Print, EntityType::While);
            Assert::IsTrue(result5.size() == 0);
        };

        TEST_METHOD(testGetLhsSynRhsWildCard) {
            std::list<PKBEntity> result1 = storage->getLhsSynRhsWildcard(EntityType::While);

            Assert::IsTrue(result1.size() == 3);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsWildcard(EntityType::If);

            Assert::IsTrue(result2.size() == 3);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result3 = storage->getLhsSynRhsWildcard(EntityType::Assign);

            Assert::IsTrue(result3.size() == 0);
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = storage->getLhsSynRhsConstant(EntityType::While, { "12", QueryConstantType::Number });

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsConstant(EntityType::While, { "8", QueryConstantType::Number });

            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }


            std::list<PKBEntity> result3 = storage->getLhsSynRhsConstant(EntityType::If, { "8", QueryConstantType::Number });

            Assert::IsTrue(result3.size() == 1);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result4 = storage->getLhsSynRhsConstant(EntityType::If, { "3", QueryConstantType::Number });

            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntity> result5 = storage->getLhsSynRhsConstant(EntityType::While, { "10", QueryConstantType::Number });

            Assert::IsTrue(result5.size() == 1);
            for (auto entity = result5.begin(); entity != result5.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }
        };

        TEST_METHOD(testGetLhsWildcardRhsSyn) {
            std::list<PKBEntity> result1 = storage->getLhsWildcardRhsSyn(EntityType::Assign);

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage->getLhsWildcardRhsSyn(EntityType::While);

            Assert::IsTrue(result2.size() == 3);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage->getLhsWildcardRhsSyn(EntityType::If);

            Assert::IsTrue(result3.size() == 3);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result4 = storage->getLhsWildcardRhsSyn(EntityType::Print);

            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Print);
            }
        };

        TEST_METHOD(testGetLhsWildcardRhsWildcard) {
            bool result = storage->hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        };

        TEST_METHOD(testGetLhsWildcardRhsConstant) {
            bool result1 = storage->hasLhsWildcardRhsConstant({ "3", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsWildcardRhsConstant({ "4", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsWildcardRhsConstant({ "5", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsWildcardRhsConstant({ "6", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsWildcardRhsConstant({ "7",  QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsWildcardRhsConstant({ "8",  QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsWildcardRhsConstant({ "9", QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage->hasLhsWildcardRhsConstant({ "10", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = storage->hasLhsWildcardRhsConstant({ "11", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result10 = storage->hasLhsWildcardRhsConstant({ "12", QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool invalidResult1 = storage->hasLhsWildcardRhsConstant({ "1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsWildcardRhsConstant({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsWildcardRhsConstant({ "2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsWildcardRhsConstant({ "0", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);
        };

        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = storage->getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::Assign);

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage->getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::While);

            Assert::IsTrue(result2.size() == 3);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result3 = storage->getLhsConstantRhsSyn({ "2", QueryConstantType::Number }, EntityType::If);

            Assert::IsTrue(result3.size() == 3);
            for (auto entity = result3.begin(); entity != result3.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::If);
            }

            std::list<PKBEntity> result4 = storage->getLhsConstantRhsSyn({ "10", QueryConstantType::Number }, EntityType::While);

            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::While);
            }

            std::list<PKBEntity> result5 = storage->getLhsConstantRhsSyn({ "12", QueryConstantType::Number }, EntityType::While);

            Assert::IsTrue(result5.size() == 0);


        };

        TEST_METHOD(testGetLhsConstantRhsWildcard) {
            bool result1 = storage->hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsWildcard({ "6", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsWildcard({ "10",  QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsConstantRhsWildcard({ "11",  QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool invalidResult1 = storage->hasLhsConstantRhsWildcard({ "-1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsWildcard({ "20", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsConstantRhsWildcard({ "5", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage->hasLhsConstantRhsWildcard({ "8", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);

            bool invalidResult6 = storage->hasLhsConstantRhsWildcard({ "9", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult6);

            bool invalidResult7 = storage->hasLhsConstantRhsWildcard({ "12", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult7);
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "5", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "7", QueryConstantType::Number });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "8", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "9",QueryConstantType::Number });
            Assert::IsTrue(result7);

            bool result8 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "10",QueryConstantType::Number });
            Assert::IsTrue(result8);

            bool result9 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "11",QueryConstantType::Number });
            Assert::IsTrue(result9);

            bool result10 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "12",QueryConstantType::Number });
            Assert::IsTrue(result10);

            bool result11 = storage->hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "5",QueryConstantType::Number });
            Assert::IsTrue(result11);

            bool result12 = storage->hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "7",QueryConstantType::Number });
            Assert::IsTrue(result12);

            bool result13 = storage->hasLhsConstantRhsConstant({ "6", QueryConstantType::Number }, { "8",QueryConstantType::Number });
            Assert::IsTrue(result13);

            bool result14 = storage->hasLhsConstantRhsConstant({ "7", QueryConstantType::Number }, { "8",QueryConstantType::Number });
            Assert::IsTrue(result14);

            bool result15 = storage->hasLhsConstantRhsConstant({ "10", QueryConstantType::Number }, { "11",QueryConstantType::Number });
            Assert::IsTrue(result15);

            bool result16 = storage->hasLhsConstantRhsConstant({ "10", QueryConstantType::Number }, { "12",QueryConstantType::Number });
            Assert::IsTrue(result16);

            bool result17 = storage->hasLhsConstantRhsConstant({ "11", QueryConstantType::Number }, { "12",QueryConstantType::Number });
            Assert::IsTrue(result16);

            bool invalidResult1 = storage->hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsConstant({ "3", QueryConstantType::Identifier }, { "4", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "6", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "9", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage->hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "10", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult5);

            bool invalidResult6 = storage->hasLhsConstantRhsConstant({ "9", QueryConstantType::Number }, { "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult6);

            bool invalidResult7 = storage->hasLhsConstantRhsConstant({ "12", QueryConstantType::Number }, { "11", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult7);

            bool invalidResult8 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "13", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult8);
        };
    };

};

