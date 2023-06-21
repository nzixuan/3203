#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "IfStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(IfStorageTest) {

        IfStorage ifStorage = IfStorage();

        TEST_METHOD_INITIALIZE(initializeAssignStorage) {
            ifStorage.addControlVariable(3, "x");
            ifStorage.addControlVariable(6, "y");
            ifStorage.addControlVariable(6, "z");
            ifStorage.addControlVariable(15, "t");
            ifStorage.addControlVariable(29, "x");
            ifStorage.addControlVariable(300, "x");
            ifStorage.addControlVariable(298, "veryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableName");
        }

        TEST_METHOD_CLEANUP(destroyAssignStorage) {
            ifStorage.clear();
        }

        TEST_METHOD(getPatternWildcardTest) {
            std::list<PKBEntity> result = ifStorage.getPatternWildcard();
            Assert::IsTrue(result.size() == 7);

        }

        TEST_METHOD(getPatternSynTest) {
            std::list<PKBEntityPair> result = ifStorage.getPatternSyn();
            Assert::IsTrue(result.size() == 7);

        }

        TEST_METHOD(getPatternConstTest) {
            std::list<PKBEntity> result = ifStorage.getPatternConst("x");
            Assert::IsTrue(result.size() == 3);

        }
    };

};

