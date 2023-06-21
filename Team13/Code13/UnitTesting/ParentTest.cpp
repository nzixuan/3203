#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentStorage.h"
#include "common.h"
#include "UsesStorageTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(ParentStorageTest) {

        ParentStorage parentStorage = ParentStorage();
        SuchThatStorage* storage = &parentStorage;

        TEST_METHOD_INITIALIZE(initializeParentStorageWithEntityPairs) {


            const PKBEntity s1 = { "1", EntityType::While };
            const PKBEntity s2 = { "2", EntityType::Assign };
            const PKBEntity s3 = { "3", EntityType::While };
            const PKBEntity s4 = { "4", EntityType::Assign };
          

            storage->set(s1, s2);
            storage->set(s1, s3);
            storage->set(s3, s4);
            

        }

        TEST_METHOD_CLEANUP(destroyParentStorage) {
            storage->clear();
        }


        TEST_METHOD(testAddParentRelationships) {
            Assert::IsTrue(storage->getLhsMap()->size() == 2);
            Assert::IsTrue(storage->getRhsMap()->size() == 3);
            Assert::IsTrue(storage->getEntityPairSet()->size() == 3);
        }

        TEST_METHOD(testGetLhsSynRhsSyn) {
            std::list<PKBEntityPair> result1 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::Assign);

            Assert::IsTrue(result1.size() == 2);
            for (auto pair = result1.begin(); pair != result1.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::Assign);
            }

            std::list<PKBEntityPair> result2 = storage->getLhsSynRhsSyn(EntityType::While, EntityType::While);

            Assert::IsTrue(result2.size() == 1);
            for (auto pair = result2.begin(); pair != result2.end(); pair++) {
                Assert::IsTrue((pair->lhs).entityType == EntityType::While && (pair->rhs).entityType == EntityType::While);
            }
        };

        TEST_METHOD(testGetLhsConstantRhsConstant) {
            bool result1 = storage->hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "2", QueryConstantType::Number });
            Assert::IsTrue(result1);

            bool result2 = storage->hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "3", QueryConstantType::Number });
            Assert::IsTrue(result2);

            bool result3 = storage->hasLhsConstantRhsConstant({ "3", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsTrue(result3);

            bool wrongResult1 = storage->hasLhsConstantRhsConstant({ "1", QueryConstantType::Number }, { "4", QueryConstantType::Number });
            Assert::IsTrue(!wrongResult1);
        }

    };

};
