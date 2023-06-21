#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesStorage.h"
#include "common.h"
#include "UsesStorageTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(UsesStorageTest) {

        UsesStorage usesStorage = UsesStorage();
        SuchThatStorage* storage = &usesStorage;

        TEST_METHOD_INITIALIZE(initializeUsesStorageWithEntityPairs) {

            const PKBEntity p = { "procedure", EntityType::Procedure };
            const PKBEntity v1 = { "v1", EntityType::Variable };
            const PKBEntity v2 = { "v2", EntityType::Variable };

            // To be removed once pattern storage is up
            const PKBEntity constant = { "4", EntityType::Constant };
            const PKBEntity a1 = { "1", EntityType::Assign };
            const PKBEntity pn = { "2", EntityType::Print };
            const PKBEntity a2 = { "3", EntityType::Assign };
            const PKBEntity c = { "4", EntityType::Call };


            storage->set(a1, v1);

            storage->set(a1, v2);
            storage->set(pn, v1);
            storage->set(a2, v2);
            storage->set(a2, constant);
            storage->set(c, v2);
            storage->set(p, v1);
            storage->set(p, v2);

        }

        TEST_METHOD_CLEANUP(destroyUsesStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddUsesRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 5);
            Assert::IsTrue(storage->getRhsMap()->size() == 3);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 8);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::Assign, EntityType::Variable);
         
            Assert::IsTrue(result1.size() == 3);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Variable);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::Print, EntityType::Variable);

            Assert::IsTrue(result2.size() == 1);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Print && (pair->rhs).entityType == EntityType::Variable);
            }

            std::list<PKBEntityPair> result3 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::Variable);

            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntityPair> result4 = storage->getLhsSynRhsSyn(EntityType::Procedure, EntityType::Variable);

            Assert::IsTrue(result4.size() == 2);
            for (auto pair = result4.begin(); pair != result4.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Procedure && (pair->rhs).entityType == EntityType::Variable);
            }

            std::list<PKBEntityPair> result5 = storage->getLhsSynRhsSyn(EntityType::Call, EntityType::Variable);

            Assert::IsTrue(result5.size() == 1);
            for (auto pair = result5.begin(); pair != result5.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Call && (pair->rhs).entityType == EntityType::Variable);
            }

            // To
            std::list<PKBEntityPair> result6 = storage->getLhsSynRhsSyn(EntityType::Assign, EntityType::Constant);

            Assert::IsTrue(result6.size() == 1);
            for (auto pair = result6.begin(); pair != result6.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Constant);
            }
        };

        TEST_METHOD(testGetLhsSynRhsWildCard) {

            std::list<PKBEntity> result1 = storage->getLhsSynRhsWildcard(EntityType::Assign);

            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsWildcard(EntityType::Print);

            Assert::IsTrue(result2.size() == 1);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Print);
            }

            std::list<PKBEntity> result3= storage->getLhsSynRhsWildcard(EntityType::Variable);

            Assert::IsTrue(result3.size() == 0);

            std::list<PKBEntity> result4 = storage->getLhsSynRhsWildcard(EntityType::Procedure);

            Assert::IsTrue(result4.size() == 1);
            for (auto entity = result4.begin(); entity != result4.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }

            std::list<PKBEntity> result5 = storage->getLhsSynRhsWildcard(EntityType::Call);

            Assert::IsTrue(result5.size() == 1);
            for (auto entity = result5.begin(); entity != result5.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Call);
            }
        };

        TEST_METHOD(testGetLhsConstantRhsWildcard) {
            bool result1 = storage->hasLhsConstantRhsWildcard({ "1", QueryConstantType::Number });
            Assert::IsTrue(result1);
            
            bool result3 = storage->hasLhsConstantRhsWildcard({ "2", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsWildcard({ "3", QueryConstantType::Number });
            Assert::IsTrue(result4);

            bool result6 = storage->hasLhsConstantRhsWildcard({ "4", QueryConstantType::Number });
            Assert::IsTrue(result6);

            bool result5 = storage->hasLhsConstantRhsWildcard({ "procedure", QueryConstantType::Identifier });
            Assert::IsTrue(result5);

            bool invalidResult1 = storage->hasLhsConstantRhsWildcard({ "1", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsWildcard({ "7", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);

            bool invalidResult3 = storage->hasLhsConstantRhsWildcard({ "v", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult3);

            bool invalidResult4 = storage->hasLhsConstantRhsWildcard({ "v2", QueryConstantType::Identifier });
            Assert::IsTrue(!invalidResult4);
        };


        TEST_METHOD(testGetLhsConstantRhsSyn) {
            std::list<PKBEntity> result1 = storage->getLhsConstantRhsSyn({ "procedure", QueryConstantType::Identifier }, EntityType::Variable);
            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Variable);
            }

            std::list<PKBEntity> result2 = storage->getLhsConstantRhsSyn({ "1", QueryConstantType::Number }, EntityType::Variable);
            Assert::IsTrue(result2.size() == 2);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Variable);
            }

            std::list<PKBEntity> result3 = storage->getLhsConstantRhsSyn({ "procedure", QueryConstantType::Number }, EntityType::Variable);
            Assert::IsTrue(result3.size() == 0);
        };

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = storage->getLhsSynRhsConstant(EntityType::Assign, {"4", QueryConstantType::Identifier});
            Assert::IsTrue(result1.size() == 1);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }
        };


        
    };

};

