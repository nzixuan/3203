#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "WhileStorage.h"
#include "common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(WhileStorageTest) {

        WhileStorage whileStorage = WhileStorage();

        TEST_METHOD_INITIALIZE(initializeAssignStorage) {
            whileStorage.addControlVariable(3, "x");
            whileStorage.addControlVariable(6, "y");
            whileStorage.addControlVariable(6, "z");
            whileStorage.addControlVariable(15, "t");
            whileStorage.addControlVariable(29, "x");
            whileStorage.addControlVariable(300, "x");
            whileStorage.addControlVariable(298, "veryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableNameveryLongVariableName");
        }

        TEST_METHOD_CLEANUP(destroyAssignStorage) {
            whileStorage.clear();
        }

        TEST_METHOD(getPatternWildcardTest) {
            std::list<PKBEntity> result = whileStorage.getPatternWildcard();
            Assert::IsTrue(result.size() == 7);

        }

        TEST_METHOD(getPatternSynTest) {
            std::list<PKBEntityPair> result = whileStorage.getPatternSyn();
            Assert::IsTrue(result.size() == 7);

        }

        TEST_METHOD(getPatternConstTest) {
            std::list<PKBEntity> result = whileStorage.getPatternConst("x");
            Assert::IsTrue(result.size() == 3);

        }
    };

};

