#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(CallsStorageTest) {
        CallsStorage callsStorage = CallsStorage();
        SuchThatStorage* storage = &callsStorage;

        TEST_METHOD_INITIALIZE(initializeCallsStorageWithEntityPairs) {

            const PKBEntity procedure1 = { "prod1", EntityType::Procedure };
            const PKBEntity procedure2 = { "prod2", EntityType::Procedure };
            const PKBEntity procedure3 = { "prod3", EntityType::Procedure };
            const PKBEntity procedure4 = { "prod4", EntityType::Procedure };
            const PKBEntity procedure5 = { "prod5", EntityType::Procedure };
            const PKBEntity procedure6 = { "prod6", EntityType::Procedure };
            const PKBEntity procedure7 = { "prod7", EntityType::Procedure };


            storage->set(procedure1, procedure2);
            storage->set(procedure1, procedure3);
            storage->set(procedure2, procedure4);
            storage->set(procedure4, procedure5);
            storage->set(procedure3, procedure6);
            storage->set(procedure1, procedure7);
            storage->set(procedure5, procedure7);

        }

        TEST_METHOD_CLEANUP(destroyCallsStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddCallsRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 5);
            Assert::IsTrue(storage->getRhsMap()->size() == 6);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 7);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::Procedure, EntityType::Procedure);
            Assert::IsTrue(result1.size() == 7);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Procedure && (pair->rhs).entityType == EntityType::Procedure);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::Procedure, EntityType::Stmt);
            Assert::IsTrue(result2.size() == 0);

            std::list<PKBEntityPair> result3 = storage->getLhsSynRhsSyn(EntityType::Stmt, EntityType::Procedure);
            Assert::IsTrue(result3.size() == 0);
        };

        TEST_METHOD(testGetLhsSynRhsWildCard) {

            std::list<PKBEntity> result1 = storage->getLhsSynRhsWildcard(EntityType::Procedure);
            Assert::IsTrue(result1.size() == 5);

            std::list<PKBEntity> result2 = storage->getLhsSynRhsWildcard(EntityType::Stmt);
            Assert::IsTrue(result2.size() == 0);

            std::list<PKBEntity> result3 = storage->getLhsSynRhsWildcard(EntityType::Variable);
            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage->getLhsSynRhsWildcard(EntityType::Constant);
            Assert::IsTrue(result4.size() == 0);
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {

            std::list<PKBEntity> result1 = storage->getLhsSynRhsConstant(EntityType::Procedure, { "prod7", QueryConstantType::Identifier });
            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsConstant(EntityType::Procedure, { "prod1", QueryConstantType::Identifier });
            Assert::IsTrue(result2.size() == 0);

            std::list<PKBEntity> result3 = storage->getLhsSynRhsConstant(EntityType::Procedure, { "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(result3.size() == 1);

            std::list<PKBEntity> result4 = storage->getLhsSynRhsConstant(EntityType::Procedure, { "prod7", QueryConstantType::Number });
            Assert::IsTrue(result4.size() == 0);

            std::list<PKBEntity> result5 = storage->getLhsSynRhsConstant(EntityType::Stmt, { "prod7", QueryConstantType::Identifier });
            Assert::IsTrue(result5.size() == 0);

        };

        TEST_METHOD(testGetLhsWildcardRhsSyn) {

            std::list<PKBEntity> result1 = storage->getLhsWildcardRhsSyn(EntityType::Procedure);
            Assert::IsTrue(result1.size() == 6);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            std::list<PKBEntity> result2 = storage->getLhsWildcardRhsSyn(EntityType::Stmt);
            Assert::IsTrue(result2.size() == 0);

        };

        TEST_METHOD(testGetLhsWildcardRhsWildcard) {
            bool result = storage->hasLhsWildcardRhsWildcard();
            Assert::IsTrue(result);
        };

        TEST_METHOD(testGetLhsWildcardRhsConstant) {
            bool result1 = storage->hasLhsWildcardRhsConstant({ "prod2", QueryConstantType::Identifier });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsWildcardRhsConstant({ "prod4", QueryConstantType::Identifier });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsWildcardRhsConstant({ "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsWildcardRhsConstant({ "prod3", QueryConstantType::Identifier });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsWildcardRhsConstant({ "prod6",  QueryConstantType::Identifier });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsWildcardRhsConstant({ "prod7",  QueryConstantType::Identifier });
            Assert::IsTrue(result6);

            bool invalidResult1 = storage->hasLhsWildcardRhsConstant({ "prod2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsWildcardRhsConstant({ "prod1", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult2);

        };

        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = storage->getLhsConstantRhsSyn({ "prod1", QueryConstantType::Identifier }, EntityType::Procedure);
            Assert::IsTrue(result1.size() == 3);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            std::list<PKBEntity> result2 = storage->getLhsConstantRhsSyn({ "prod5", QueryConstantType::Identifier }, EntityType::Procedure);
            Assert::IsTrue(result2.size() == 1);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            std::list<PKBEntity> result3 = storage->getLhsConstantRhsSyn({ "prod7", QueryConstantType::Identifier }, EntityType::Procedure);
            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage->getLhsConstantRhsSyn({ "prod1", QueryConstantType::Number }, EntityType::Procedure);
            Assert::IsTrue(result4.size() == 0);
        };

        TEST_METHOD(testGetLhsConstantRhsWildcard) {
            bool result1 = storage->hasLhsConstantRhsWildcard({ "prod1", QueryConstantType::Identifier });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsWildcard({ "prod2", QueryConstantType::Identifier });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsWildcard({ "prod3", QueryConstantType::Identifier });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsWildcard({ "prod4", QueryConstantType::Identifier });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsWildcard({ "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(result5);

            bool invalidResult1 = storage->hasLhsConstantRhsWildcard({ "prod6", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsWildcard({ "prod1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsWildcard({ "prod7", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Identifier }, { "prod2", QueryConstantType::Identifier });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Identifier }, { "prod3", QueryConstantType::Identifier });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Identifier }, { "prod7", QueryConstantType::Identifier });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsConstant({ "prod2", QueryConstantType::Identifier }, { "prod4", QueryConstantType::Identifier });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsConstant({ "prod4", QueryConstantType::Identifier }, { "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(result5);

            bool result6 = storage->hasLhsConstantRhsConstant({ "prod5", QueryConstantType::Identifier }, { "prod7", QueryConstantType::Identifier });
            Assert::IsTrue(result6);

            bool result7 = storage->hasLhsConstantRhsConstant({ "prod3", QueryConstantType::Identifier }, { "prod6", QueryConstantType::Identifier });
            Assert::IsTrue(result7);

            bool invalidResult1 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Identifier }, { "prod2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Number }, { "prod2", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsConstant({ "prod1", QueryConstantType::Identifier }, { "prod4", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsConstantRhsConstant({ "prod7", QueryConstantType::Identifier }, { "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult4);

            bool invalidResult5 = storage->hasLhsConstantRhsConstant({ "prod2", QueryConstantType::Identifier }, { "prod5", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult5);
        };
    };

};

