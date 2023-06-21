#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifiesStorage.h"
#include "common.h"
#include "UsesStorageTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(ModifiesStorageTest) {

        ModifiesStorage modifiesStorage = ModifiesStorage();
        SuchThatStorage* storage = &modifiesStorage;

        TEST_METHOD_INITIALIZE(initializeModifiesStorageWithEntityPairs) {

            const PKBEntity p = { "procedure", EntityType::Procedure };
            const PKBEntity v1 = { "v1", EntityType::Variable };
            const PKBEntity v2 = { "v2", EntityType::Variable };
            const PKBEntity a1 = { "1", EntityType::Assign };
            const PKBEntity re = { "2", EntityType::Read };
            const PKBEntity a2 = { "3", EntityType::Assign };
            const PKBEntity c = { "4", EntityType::Call };

            storage->set(a1, v1);
            storage->set(re, v1);
            storage->set(a2, v2);
            storage->set(c, v2);
            storage->set(p, v1);
            storage->set(p, v2);

        }

        TEST_METHOD_CLEANUP(destroyModifiesStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddModifiesRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 5);
            Assert::IsTrue(storage->getRhsMap()->size() == 2);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 6);
        };

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::Assign, EntityType::Variable);

            Assert::IsTrue(result1.size() == 2);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Assign && (pair->rhs).entityType == EntityType::Variable);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::Read, EntityType::Variable);

            Assert::IsTrue(result2.size() == 1);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::Read && (pair->rhs).entityType == EntityType::Variable);
            }

            std::list<PKBEntityPair> result3 = storage->getLhsSynRhsSyn(EntityType::If, EntityType::Variable);

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

            std::list<PKBEntityPair> result6 = storage->getLhsSynRhsSyn(EntityType::Stmt, EntityType::Variable);
            Assert::IsTrue(result6.size() == 4);

            std::list<PKBEntityPair> result7 = storage->getLhsSynRhsSyn(EntityType::Stmt, EntityType::Stmt);
            Assert::IsTrue(result7.size() == 0);
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage->hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "v1", QueryConstantType::Identifier });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "v2", QueryConstantType::Identifier });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsConstant({ "4", QueryConstantType::Number }, { "v2", QueryConstantType::Identifier });
            Assert::IsTrue(result3);

            bool result4 = storage->hasLhsConstantRhsConstant({ "procedure", QueryConstantType::Identifier }, { "v1", QueryConstantType::Identifier });
            Assert::IsTrue(result4);

            bool result5 = storage->hasLhsConstantRhsConstant({ "procedure", QueryConstantType::Identifier }, { "v2", QueryConstantType::Identifier });
            Assert::IsTrue(result5);

            bool invalidResult1 = storage->hasLhsConstantRhsConstant({ "2", QueryConstantType::Number }, { "v2", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult1);

            bool invalidResult2 = storage->hasLhsConstantRhsConstant({ "4", QueryConstantType::Identifier }, { "v1", QueryConstantType::Number });
            Assert::IsTrue(!invalidResult2);
        };

        TEST_METHOD(testGetLhsSynRhsWildcard) {
            std::list<PKBEntity> result1 = storage->getLhsSynRhsWildcard(EntityType::Assign);
            Assert::IsTrue(result1.size() == 2);
            for (auto entity = result1.begin(); entity != result1.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Assign);
            }

            std::list<PKBEntity> result2 = storage->getLhsSynRhsWildcard(EntityType::Procedure);
            Assert::IsTrue(result2.size() == 1);
            for (auto entity = result2.begin(); entity != result2.end(); entity++) {
                Assert::IsTrue(entity->entityType == EntityType::Procedure);
            }
        
            std::list<PKBEntity> result3 = storage->getLhsSynRhsWildcard(EntityType::While);
            Assert::IsTrue(result3.size() == 0);


            std::list<PKBEntity> result4 = storage->getLhsSynRhsWildcard(EntityType::Stmt);
            Assert::IsTrue(result4.size() == 4);
        }

        TEST_METHOD(testGetLhsSynRhsConstant) {
            std::list<PKBEntity> result1 = storage->getLhsSynRhsConstant(EntityType::Stmt, { "v1", QueryConstantType::Identifier });
            Assert::IsTrue(result1.size() == 2);

            std::list<PKBEntity> result2 = storage->getLhsSynRhsConstant(EntityType::Stmt, { "v2", QueryConstantType::Identifier });
            Assert::IsTrue(result2.size() == 2);

            std::list<PKBEntity> result3 = storage->getLhsSynRhsConstant(EntityType::Stmt, { "1", QueryConstantType::Identifier });
            Assert::IsTrue(result3.size() == 0);
        }

    };

};

